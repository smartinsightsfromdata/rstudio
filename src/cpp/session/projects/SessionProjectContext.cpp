/*
 * SessionProjectContext.cpp
 *
 * Copyright (C) 2009-12 by RStudio, Inc.
 *
 * Unless you have received this program directly from RStudio pursuant
 * to the terms of a commercial license agreement with RStudio, then
 * this program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */

#include <session/projects/SessionProjects.hpp>

#include <map>

#include <boost/format.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <core/FileSerializer.hpp>
#include <core/r_util/RProjectFile.hpp>
#include <core/r_util/RSessionContext.hpp>

#include <core/system/FileMonitor.hpp>

#include <r/RExec.hpp>
#include <r/RRoutines.hpp>

#include <session/SessionUserSettings.hpp>
#include <session/SessionModuleContext.hpp>

#include "SessionProjectFirstRun.hpp"

using namespace rstudio::core;

namespace rstudio {
namespace session {
namespace projects {

namespace {

bool canWriteToProjectDir(const FilePath& projectDirPath)
{
   FilePath testFile = projectDirPath.complete(core::system::generateUuid());
   Error error = core::writeStringToFile(testFile, "test");
   if (error)
   {
      return false;
   }
   else
   {
      error = testFile.removeIfExists();
      if (error)
         LOG_ERROR(error);

      return true;
   }
}

// access to project settings
std::string readSetting(const char * const settingName);
void writeSetting(const char * const settingName, const std::string& value);

}  // anonymous namespace


Error computeScratchPath(const FilePath& projectFile, FilePath* pScratchPath)
{
   // ensure project user dir
   FilePath projectUserDir = projectFile.parent().complete(".Rproj.user");
   if (!projectUserDir.exists())
   {
      // create
      Error error = projectUserDir.ensureDirectory();
      if (error)
         return error;

      // mark hidden if we are on win32
#ifdef _WIN32
      error = core::system::makeFileHidden(projectUserDir);
      if (error)
         return error;
#endif
   }

   // now add context id to form scratch path
   FilePath scratchPath = projectUserDir.complete(userSettings().contextId());
   Error error = scratchPath.ensureDirectory();
   if (error)
      return error;

   // return the path
   *pScratchPath = scratchPath;
   return Success();
}

FilePath ProjectContext::oldScratchPath() const
{
   // start from the standard .Rproj.user dir
   FilePath projectUserDir = directory().complete(".Rproj.user");
   if (!projectUserDir.exists())
      return FilePath();

   // add username if we can get one
   std::string username = core::system::username();
   if (!username.empty())
      projectUserDir = projectUserDir.complete(username);

   // if this path doesn't exist then bail
   if (!projectUserDir.exists())
      return FilePath();

   // see if an old scratch path using the old contextId is present
   // and if so return it
   FilePath oldPath = projectUserDir.complete(userSettings().oldContextId());
   if (oldPath.exists())
      return oldPath;
   else
      return FilePath();
}

// NOTE: this function is called very early in the process lifetime (from
// session::projects::startup) so can only have limited dependencies.
// specifically, it can rely on userSettings() being available, but can
// definitely NOT rely on calling into R. For initialization related tasks
// that need to run after R is available use the implementation of the
// initialize method (below)
Error ProjectContext::startup(const FilePath& projectFile,
                              std::string* pUserErrMsg)
{
   // test for project file existence
   if (!projectFile.exists())
   {
      *pUserErrMsg = "the project file does not exist";
      return pathNotFoundError(projectFile.absolutePath(), ERROR_LOCATION);
   }

   // test for writeabilty of parent
   if (!canWriteToProjectDir(projectFile.parent()))
   {
      *pUserErrMsg = "the project directory is not writeable";
      return systemError(boost::system::errc::permission_denied,
                         ERROR_LOCATION);
   }

   // calculate project scratch path
   FilePath scratchPath;
   Error error = computeScratchPath(projectFile, &scratchPath);
   if (error)
   {
      *pUserErrMsg = "unable to initialize project - " + error.summary();
      return error;
   }

   // read project file config
   bool providedDefaults;
   r_util::RProjectConfig config;
   error = r_util::readProjectFile(projectFile,
                                   defaultConfig(),
                                   buildDefaults(),
                                   &config,
                                   &providedDefaults,
                                   pUserErrMsg);
   if (error)
      return error;

   // update package install args with new defaults (one time only)
   const char * kUpdatePackageInstallDefault = "update-pkg-install-default";
   if (readSetting(kUpdatePackageInstallDefault).empty())
   {
      writeSetting(kUpdatePackageInstallDefault, "1");
      if (r_util::updateSetPackageInstallArgsDefault(&config))
         providedDefaults = true;
   }

   // if we provided defaults then re-write the project file
   // with the defaults
   if (providedDefaults)
   {
      error = r_util::writeProjectFile(projectFile, buildDefaults(), config);
      if (error)
         LOG_ERROR(error);
   }

   // initialize members
   file_ = projectFile;
   directory_ = file_.parent();
   scratchPath_ = scratchPath;
   config_ = config;

   // assume true so that the initial files pane listing doesn't register
   // a duplicate monitor. if it turns out to be false then this can be
   // repaired by a single refresh of the files pane
   hasFileMonitor_ = config_.enableCodeIndexing;

   // return success
   return Success();

}

void ProjectContext::augmentRbuildignore()
{
   if (r_util::isPackageDirectory(directory()))
   {
      // constants
      const char * const kIgnoreRproj = "^.*\\.Rproj$";
      const char * const kIgnoreRprojUser = "^\\.Rproj\\.user$";

      // create the file if it doesn't exists
      FilePath rbuildIgnorePath = directory().childPath(".Rbuildignore");
      if (!rbuildIgnorePath.exists())
      {
         Error error = writeStringToFile(rbuildIgnorePath,
                                         kIgnoreRproj + std::string("\n") +
                                         kIgnoreRprojUser + std::string("\n"),
                                         string_utils::LineEndingNative);
         if (error)
            LOG_ERROR(error);
      }
      else
      {
         // if .Rbuildignore exists, add *.Rproj and .Rproj.user unless
         // they are already there

         std::string strIgnore;
         Error error = core::readStringFromFile(
                                             rbuildIgnorePath,
                                             &strIgnore,
                                             string_utils::LineEndingPosix);
         if (error)
         {
            LOG_ERROR(error);
            return;
         }

         // NOTE: we don't search for the full kIgnoreRproj to account
         // for previous less precisely specified .Rproj entries
         bool hasRProj = strIgnore.find("\\.Rproj$") != std::string::npos;
         bool hasRProjUser = strIgnore.find(kIgnoreRprojUser) != std::string::npos;

         if (hasRProj && hasRProjUser)
            return;

         bool addExtraNewline = strIgnore.size() > 0
                                && strIgnore[strIgnore.size() - 1] != '\n';

         if (addExtraNewline)
            strIgnore += "\n";
         if (!hasRProj)
            strIgnore += kIgnoreRproj + std::string("\n");
         if (!hasRProjUser)
            strIgnore += kIgnoreRprojUser + std::string("\n");
         error = core::writeStringToFile(rbuildIgnorePath,
                                         strIgnore,
                                         string_utils::LineEndingNative);
         if (error)
            LOG_ERROR(error);
      }
   }
}

SEXP rs_getProjectDirectory()
{
   SEXP absolutePathSEXP = R_NilValue;
   if (projectContext().hasProject())
   {
      r::sexp::Protect protect;
      absolutePathSEXP = r::sexp::create(
               projectContext().directory().absolutePath(), &protect);
   }
   return absolutePathSEXP;
}

SEXP rs_hasFileMonitor()
{
   r::sexp::Protect protect;
   return r::sexp::create(projectContext().hasFileMonitor(), &protect);
}

Error ProjectContext::initialize()
{
   r::routines::registerCallMethod(
            "rs_getProjectDirectory",
            (DL_FUNC) rs_getProjectDirectory,
            0);
   
   r::routines::registerCallMethod(
            "rs_hasFileMonitor",
            (DL_FUNC) rs_hasFileMonitor,
            0);
   
   if (hasProject())
   {
      // read build options for the side effect of updating buildOptions_
      RProjectBuildOptions buildOptions;
      Error error = readBuildOptions(&buildOptions);
      if (error)
         LOG_ERROR(error);

      // compute the build target path
      updateBuildTargetPath();

      // update package info
      updatePackageInfo();

      // compute the default encoding
      updateDefaultEncoding();

      // augmewnt .Rbuildignore if this is a package
      augmentRbuildignore();

      // subscribe to deferred init (for initializing our file monitor)
      if (config().enableCodeIndexing)
      {
         module_context::events().onDeferredInit.connect(
                      boost::bind(&ProjectContext::onDeferredInit, this, _1));
      }
   }

   return Success();
}


namespace {

// NOTE: the HttpConnectionListener relies on this path as well as the
// kNextSessionProject constant in order to write the next session project
// in the case of a forced abort (the two implementations are synchronized
// using constants so that the connection listener doesn't call into modules
// that are single threaded by convention
FilePath settingsPath()
{
   return r_util::projectsSettingsPath(session::options().userScratchPath());
}

std::string readSetting(const char * const settingName)
{
   return r_util::readProjectsSetting(settingsPath(), settingName);
}

void writeSetting(const char * const settingName, const std::string& value)
{
   r_util::writeProjectsSetting(settingsPath(), settingName, value);
}

} // anonymous namespace

std::string ProjectContext::nextSessionProject() const
{
   return readSetting(kNextSessionProject);
}

void ProjectContext::setNextSessionProject(
                           const std::string& nextSessionProject)
{
   writeSetting(kNextSessionProject, nextSessionProject);
}


FilePath ProjectContext::lastProjectPath() const
{
   std::string path = readSetting(kLastProjectPath);
   if (!path.empty())
      return FilePath(path);
   else
      return FilePath();
}

void ProjectContext::setLastProjectPath(const FilePath& lastProjectPath)
{
   if (!lastProjectPath.empty())
      writeSetting(kLastProjectPath, lastProjectPath.absolutePath());
   else
      writeSetting(kLastProjectPath, "");
}


void ProjectContext::onDeferredInit(bool newSession)
{
   // kickoff file monitoring for this directory
   using namespace boost;
   core::system::file_monitor::Callbacks cb;
   cb.onRegistered = bind(&ProjectContext::fileMonitorRegistered,
                          this, _1, _2);
   cb.onRegistrationError = bind(&ProjectContext::fileMonitorTermination,
                                 this, _1);
   cb.onMonitoringError = bind(&ProjectContext::fileMonitorTermination,
                               this, _1);
   cb.onFilesChanged = bind(&ProjectContext::fileMonitorFilesChanged,
                            this, _1);
   cb.onUnregistered = bind(&ProjectContext::fileMonitorTermination,
                            this, Success());
   core::system::file_monitor::registerMonitor(
                                         directory(),
                                         true,
                                         module_context::fileListingFilter,
                                         cb);
}

void ProjectContext::fileMonitorRegistered(
                              core::system::file_monitor::Handle handle,
                              const tree<core::FileInfo>& files)
{
   // update state
   hasFileMonitor_ = true;

   // notify subscribers
   onMonitoringEnabled_(files);
}

void ProjectContext::fileMonitorFilesChanged(
                   const std::vector<core::system::FileChangeEvent>& events)
{
   // notify client (gwt)
   module_context::enqueFileChangedEvents(directory(), events);

   // notify subscribers
   onFilesChanged_(events);
}

void ProjectContext::fileMonitorTermination(const Error& error)
{
   // always log error
   if (error)
      LOG_ERROR(error);

   // if we have a file monitor then unwind it
   if (hasFileMonitor_)
   {
      // do this only once
      hasFileMonitor_ = false;

      // notify end-user if this was an error condition
      if (error)
      {
         // base error message
         boost::system::error_code ec = error.code();
         std::string dir = module_context::createAliasedPath(directory());
         boost::format fmt(
          "\nWarning message:\n"
          "File monitoring failed for project at \"%1%\"\n"
          "Error %2% (%3%)");
         std::string msg = boost::str(fmt % dir % ec.value() % ec.message());

         // enumeration of affected features
         if (!monitorSubscribers_.empty())
            msg.append("\nFeatures disabled:");
         for(std::size_t i=0; i<monitorSubscribers_.size(); ++i)
         {
            if (i > 0)
               msg.append(",");
            msg.append(" ");
            msg.append(monitorSubscribers_[i]);
         }

         // write to console
         module_context::consoleWriteError(msg);
      }

      // notify subscribers
      onMonitoringDisabled_();
   }
}

bool ProjectContext::isMonitoringDirectory(const FilePath& dir) const
{
   return hasProject() && hasFileMonitor() && dir.isWithin(directory());
}

void ProjectContext::subscribeToFileMonitor(const std::string& featureName,
                                            const FileMonitorCallbacks& cb)
{
   if (!featureName.empty())
      monitorSubscribers_.push_back(featureName);

   if (cb.onMonitoringEnabled)
      onMonitoringEnabled_.connect(cb.onMonitoringEnabled);
   if (cb.onFilesChanged)
      onFilesChanged_.connect(cb.onFilesChanged);
   if (cb.onMonitoringDisabled)
      onMonitoringDisabled_.connect(cb.onMonitoringDisabled);
}

std::string ProjectContext::defaultEncoding() const
{
   return defaultEncoding_;
}

void ProjectContext::updateDefaultEncoding()
{
   defaultEncoding_.clear();
   Error error = r::exec::RFunction(
                     ".rs.validateAndNormalizeEncoding",
                     config().encoding).call(&defaultEncoding_);
   if (error)
      LOG_ERROR(error);

   // if the default encoding is empty then change to UTF-8 and
   // and enque a warning
   if (defaultEncoding_.empty())
   {
      // fallback
      defaultEncoding_ = "UTF-8";

      // enque a warning
      json::Object msgJson;
      msgJson["severe"] = false;
      boost::format fmt(
        "Project text encoding '%1%' not available (using UTF-8). "
        "You can specify an alternate text encoding via Project Options.");
      msgJson["message"] = boost::str(fmt % config().encoding);
      ClientEvent event(client_events::kShowWarningBar, msgJson);
      module_context::enqueClientEvent(event);
   }
}

void ProjectContext::updateBuildTargetPath()
{
   if (config().buildType == r_util::kBuildTypeNone)
   {
      buildTargetPath_ = FilePath();
   }
   else
   {
      // determine the relative build target
      std::string buildTarget;
      if (config().buildType == r_util::kBuildTypePackage)
         buildTarget = config().packagePath;
      else if (config().buildType == r_util::kBuildTypeMakefile)
         buildTarget = config().makefilePath;
      else if (config().buildType == r_util::kBuildTypeCustom)
         buildTarget = config().customScriptPath;

      // determine the path
      if (boost::algorithm::starts_with(buildTarget, "~/") ||
               FilePath::isRootPath(buildTarget))
      {
         buildTargetPath_ = module_context::resolveAliasedPath(buildTarget);
      }
      else
      {
         buildTargetPath_=  projects::projectContext().directory().childPath(
                                                                  buildTarget);
      }
   }
}

void ProjectContext::updatePackageInfo()
{
   if (config().buildType == r_util::kBuildTypePackage)
   {
      Error error = packageInfo_.read(buildTargetPath());
      if (error)
         LOG_ERROR(error);
   }
}

json::Object ProjectContext::uiPrefs() const
{
   json::Object uiPrefs;
   uiPrefs["use_spaces_for_tab"] = config_.useSpacesForTab;
   uiPrefs["num_spaces_for_tab"] = config_.numSpacesForTab;
   uiPrefs["auto_append_newline"] = config_.autoAppendNewline;
   uiPrefs["strip_trailing_whitespace"] = config_.stripTrailingWhitespace;
   uiPrefs["default_encoding"] = defaultEncoding();
   uiPrefs["default_sweave_engine"] = config_.defaultSweaveEngine;
   uiPrefs["default_latex_program"] = config_.defaultLatexProgram;
   uiPrefs["root_document"] = config_.rootDocument;
   uiPrefs["use_roxygen"] = !config_.packageRoxygenize.empty();
   return uiPrefs;
}

json::Array ProjectContext::openDocs() const
{
   json::Array openDocsJson;
   std::vector<std::string> docs = projects::collectFirstRunDocs(file());
   BOOST_FOREACH(const std::string& doc, docs)
   {
      FilePath docPath = directory().childPath(doc);
      openDocsJson.push_back(module_context::createAliasedPath(docPath));
   }
   return openDocsJson;
}

r_util::RProjectBuildDefaults ProjectContext::buildDefaults()
{
   r_util::RProjectBuildDefaults buildDefaults;
   buildDefaults.useDevtools = userSettings().useDevtools();
   return buildDefaults;
}

r_util::RProjectConfig ProjectContext::defaultConfig()
{
   // setup defaults for project file
   r_util::RProjectConfig defaultConfig;
   defaultConfig.rVersion = r_util::RVersionInfo(kRVersionDefault);
   defaultConfig.useSpacesForTab = userSettings().useSpacesForTab();
   defaultConfig.numSpacesForTab = userSettings().numSpacesForTab();
   defaultConfig.autoAppendNewline = userSettings().autoAppendNewline();
   defaultConfig.stripTrailingWhitespace =
                              userSettings().stripTrailingWhitespace();
   if (!userSettings().defaultEncoding().empty())
      defaultConfig.encoding = userSettings().defaultEncoding();
   else
      defaultConfig.encoding = "UTF-8";
   defaultConfig.defaultSweaveEngine = userSettings().defaultSweaveEngine();
   defaultConfig.defaultLatexProgram = userSettings().defaultLatexProgram();
   defaultConfig.rootDocument = std::string();
   defaultConfig.buildType = std::string();
   defaultConfig.tutorialPath = std::string();
   defaultConfig.packageUseDevtools = userSettings().useDevtools();
   return defaultConfig;
}


namespace {

const char * const kVcsOverride = "activeVcsOverride";

} // anonymous namespace

FilePath ProjectContext::vcsOptionsFilePath() const
{
   return scratchPath().childPath("vcs_options");
}

Error ProjectContext::buildOptionsFile(Settings* pOptionsFile) const
{
   return pOptionsFile->initialize(scratchPath().childPath("build_options"));
}


Error ProjectContext::readVcsOptions(RProjectVcsOptions* pOptions) const
{
   core::Settings settings;
   Error error = settings.initialize(vcsOptionsFilePath());
   if (error)
      return error;

   std::string vcsOverride = settings.get(kVcsOverride);

   pOptions->vcsOverride = module_context::normalizeVcsOverride(vcsOverride);

   return Success();
}

Error ProjectContext::writeVcsOptions(const RProjectVcsOptions& options) const
{
   core::Settings settings;
   Error error = settings.initialize(vcsOptionsFilePath());
   if (error)
      return error;

   settings.beginUpdate();
   settings.set(kVcsOverride, options.vcsOverride);
   settings.endUpdate();

   return Success();
}

Error ProjectContext::readBuildOptions(RProjectBuildOptions* pOptions)
{
   core::Settings optionsFile;
   Error error = buildOptionsFile(&optionsFile);
   if (error)
      return error;

   pOptions->makefileArgs = optionsFile.get("makefile_args");
   pOptions->autoRoxygenizeForCheck = optionsFile.getBool(
                                       "auto_roxygenize_for_check",
                                       true);
   pOptions->autoRoxygenizeForBuildPackage = optionsFile.getBool(
                                       "auto_roxygenize_for_build_package",
                                       true);
   pOptions->autoRoxygenizeForBuildAndReload = optionsFile.getBool(
                                       "auto_roxygenize_for_build_and_reload",
                                       false);

   // opportunistically sync in-memory representation to what we read from disk
   buildOptions_ = *pOptions;

   return Success();
}

Error ProjectContext::writeBuildOptions(const RProjectBuildOptions& options)
{
   core::Settings optionsFile;
   Error error = buildOptionsFile(&optionsFile);
   if (error)
      return error;

   optionsFile.beginUpdate();
   optionsFile.set("makefile_args", options.makefileArgs);
   optionsFile.set("auto_roxygenize_for_check",
                   options.autoRoxygenizeForCheck);
   optionsFile.set("auto_roxygenize_for_build_package",
                   options.autoRoxygenizeForBuildPackage);
   optionsFile.set("auto_roxygenize_for_build_and_reload",
                   options.autoRoxygenizeForBuildAndReload);
   optionsFile.endUpdate();

   // opportunistically sync in-memory representation to what we wrote to disk
   buildOptions_ = options;

   return Success();
}

bool ProjectContext::isPackageProject()
{
   return r_util::isPackageDirectory(directory());
}


} // namespace projects
} // namespace session
} // namespace rstudio

