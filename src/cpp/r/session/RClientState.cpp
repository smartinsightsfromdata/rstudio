/*
 * RClientState.cpp
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

#include <r/session/RClientState.hpp>

#include <algorithm>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <core/Log.hpp>
#include <core/Error.hpp>
#include <core/FilePath.hpp>
#include <core/FileSerializer.hpp>

using namespace rstudio::core;

namespace rstudio {
namespace r {
namespace session {
    
namespace {
  
const char * const kTemporaryExt = ".temporary";
const char * const kPersistentExt = ".persistent";
const char * const kProjPersistentExt = ".pper";
   
void putState(const std::string& scope, 
              const json::Object::value_type& entry,
              json::Object* pStateContainer)
{
   // get the scope object (create if it doesn't exist)
   json::Object::iterator pos = pStateContainer->find(scope);
   if (pos == pStateContainer->end())
   {
      json::Object newScopeObject;
      pStateContainer->insert(std::make_pair(scope, newScopeObject));
   }
   json::Value& scopeValue = pStateContainer->operator[](scope); 
   json::Object& scopeObject = scopeValue.get_obj();
   
   // insert the value into the scope
   scopeObject[entry.first] = entry.second;
}
   
void mergeStateScope(const json::Object::value_type& scopePair,
                     json::Object* pTargetState)
{
   const std::string& scope = scopePair.first;
   const json::Value& value = scopePair.second;
   if ( value.type() == json::ObjectType )
   {
      const json::Object& stateObject = value.get_obj();
      std::for_each(stateObject.begin(),
                    stateObject.end(),
                    boost::bind(putState, scope, _1, pTargetState));
   }
   else
   {
      LOG_WARNING_MESSAGE("set_client_state call sent non json object data");
   }
}
                     
   
void mergeState(const json::Object& sourceState,
                json::Object* pTargetState)
{
   std::for_each(sourceState.begin(), 
                 sourceState.end(),
                 boost::bind(mergeStateScope, _1, pTargetState));
}

void commitState(const json::Object& stateContainer,
                 const std::string& fileExt,
                 const core::FilePath& stateDir)
{
   for (json::Object::const_iterator
        it = stateContainer.begin(); it != stateContainer.end(); ++it)
   {
      // generate json
      std::ostringstream ostr ;
      json::writeFormatted(it->second, ostr);
      
      // write to file
      FilePath stateFile = stateDir.complete(it->first + fileExt);
      Error error = writeStringToFile(stateFile, ostr.str());
      if (error)
         LOG_ERROR(error);   
   }
}
   
void restoreState(const core::FilePath& stateFilePath,
                  json::Object* pStateContainer)
{
   // read the contents of the file
   std::string contents ;
   Error error = readStringFromFile(stateFilePath, &contents);
   if (error)
   {
      LOG_ERROR(error);
      return;
   }
   
   // parse the json
   json::Value value;
   if ( !json::parse(contents, &value) )
   {
      LOG_ERROR_MESSAGE("Error parsing client state");
      return;
   }
   
   // write to the container 
   pStateContainer->insert(std::make_pair(stateFilePath.stem(), value));
}

Error removeAndRecreateStateDir(const FilePath& stateDir)
{
   Error error = stateDir.removeIfExists();
   if (error)
      return error;
   return stateDir.ensureDirectory();
}

Error restoreStateFiles(const FilePath& sourceDir,
                        boost::function<void(const FilePath&)> restoreFunc)
{
   // ignore if the directory doesn't exist
   if (!sourceDir.exists())
      return Success();

   // list the files
   std::vector<FilePath> childPaths ;
   Error error = sourceDir.children(&childPaths);
   if (error)
      return error ;

   // restore files
   std::for_each(childPaths.begin(), childPaths.end(), restoreFunc);
   return Success();
}

void appendAndValidateState(const json::Object& sourceState,
                            json::Object* pTargetState)
{
   // append (log warning if there are dupes)
   for (json::Object::const_iterator it = sourceState.begin();
        it != sourceState.end();
        ++it)
   {
      if (pTargetState->find(it->first) != pTargetState->end())
         LOG_WARNING_MESSAGE("duplicate state key: " + it->first);
      else
         pTargetState->insert(*it);
   }
}


} // anonymous namespace
   
// singleton
ClientState& clientState()
{
   static ClientState instance;
   return instance;
}

   
ClientState::ClientState()
{
}

void ClientState::restoreGlobalState(const FilePath& stateFile)
{
   if (stateFile.extension() == kTemporaryExt)
      restoreState(stateFile, &temporaryState_);
   else if (stateFile.extension() == kPersistentExt)
      restoreState(stateFile, &persistentState_);
}

void ClientState::restoreProjectState(const FilePath& stateFile)
{
   if (stateFile.extension() == kProjPersistentExt)
      restoreState(stateFile, &projectPersistentState_);
}
   
void ClientState::clear()  
{
   temporaryState_.clear();
   persistentState_.clear();
   projectPersistentState_.clear();
}
 
void ClientState::putTemporary(const std::string& scope, 
                               const std::string& name,
                               const json::Value& value)
{
   json::Object stateContainer ;
   putState(scope, std::make_pair(name, value), &stateContainer);
   putTemporary(stateContainer);
}
   
void ClientState::putTemporary(const json::Object& temporaryState)
{
   mergeState(temporaryState, &temporaryState_);
}

void ClientState::putPersistent(const std::string& scope, 
                                const std::string& name,
                                const json::Value& value)
{
   json::Object stateContainer;
   putState(scope, std::make_pair(name, value), &stateContainer);
   putPersistent(stateContainer);
}

void ClientState::putPersistent(const json::Object& persistentState)
{
   mergeState(persistentState, &persistentState_);
}

void ClientState::putProjectPersistent(const std::string& scope,
                                       const std::string& name,
                                       const json::Value& value)
{
   json::Object stateContainer;
   putState(scope, std::make_pair(name, value), &stateContainer);
   putProjectPersistent(stateContainer);
}

json::Value ClientState::getProjectPersistent(std::string scope,
                                              std::string name)
{
   json::Object::iterator i = projectPersistentState_.find(scope);
   if (i == projectPersistentState_.end())
   {
      return json::Value();
   }
   else
   {
      if (!json::isType<core::json::Object>(i->second))
         return json::Value();
      json::Object& scopeObject = (i->second).get_obj();
      return scopeObject[name];
   }
}

void ClientState::putProjectPersistent(
                              const json::Object& projectPersistentState)
{
   mergeState(projectPersistentState, &projectPersistentState_);
}


Error ClientState::commit(ClientStateCommitType commitType, 
                          const core::FilePath& stateDir,
                          const core::FilePath& projectStateDir)
{
   // remove and re-create the stateDirs
   Error error = removeAndRecreateStateDir(stateDir);
   if (error)
      return error;
   error = removeAndRecreateStateDir(projectStateDir);
   if (error)
      return error;

   // always commit persistent state
   commitState(persistentState_, kPersistentExt, stateDir);
   commitState(projectPersistentState_, kProjPersistentExt, projectStateDir);
  
   // commit all state if requested
   if (commitType == ClientStateCommitAll)
      commitState(temporaryState_, kTemporaryExt, stateDir);
   else
      temporaryState_.clear();
   
   return Success();
}
   
Error ClientState::restore(const FilePath& stateDir,
                           const FilePath& projectStateDir)
{
   // clear existing values
   clear();
   
   // restore global state
   Error error = restoreStateFiles(
                  stateDir,
                  boost::bind(&ClientState::restoreGlobalState, this, _1));
   if (error)
      return error;

   // restore project state
   return restoreStateFiles(
                  projectStateDir,
                  boost::bind(&ClientState::restoreProjectState, this, _1));
}

// generate current state by merging temporary and persistent states
void ClientState::currentState(json::Object* pCurrentState) const
{
   // start with copy of persistent state
   pCurrentState->clear();
   pCurrentState->insert(persistentState_.begin(), persistentState_.end());
   
   // add and validate other state collections
   appendAndValidateState(projectPersistentState_, pCurrentState);
   appendAndValidateState(temporaryState_, pCurrentState);
}

} // namespace session
} // namespace r
} // namespace rstudio

