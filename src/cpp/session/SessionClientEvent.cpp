/*
 * SessionClientEvent.cpp
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

#include <session/SessionClientEvent.hpp>

#include <boost/lexical_cast.hpp>

#include <core/Error.hpp>
#include <core/Log.hpp>
#include <core/FilePath.hpp>
#include <core/SafeConvert.hpp>
#include <core/FileSerializer.hpp>
#include <core/system/System.hpp>

using namespace rstudio::core ;

namespace rstudio {
namespace session {

namespace client_events {
   
const int kBusy = 1;  
const int kConsolePrompt = 2;
const int kConsoleWriteOutput = 3;
const int kConsoleWriteError = 4;
const int kShowErrorMessage = 5;
const int kShowHelp = 6;
const int kBrowseUrl = 7;
const int kShowEditor = 11;
const int kChooseFile = 13;
const int kAbendWarning = 14;
const int kQuit = 15;
const int kSuicide = 16;
const int kFileChanged = 17;
const int kWorkingDirChanged = 18;
const int kPlotsStateChanged = 19;
const int kViewData = 20;
const int kPackageStatusChanged = 21;
const int kPackageStateChanged = 22;
const int kLocator = 23;
const int kConsoleResetHistory = 25;
const int kSessionSerialization = 26;
const int kHistoryEntriesAdded = 27;
const int kQuotaStatus = 29;
const int kFileEdit = 32;
const int kShowContent = 33;
const int kShowData = 34;
const int kAsyncCompletion = 35;
const int kSaveActionChanged = 36;
const int kConsoleWritePrompt = 37;
const int kConsoleWriteInput = 38;
const int kShowWarningBar = 39;
const int kOpenProjectError = 40;
const int kVcsRefresh = 41;
const int kAskPass = 42;
const int kConsoleProcessOutput = 43;
const int kConsoleProcessExit = 44;
const int kListChanged = 45;
const int kConsoleProcessCreated = 46;
const int kUiPrefsChanged = 47;
const int kHandleUnsavedChanges = 48;
const int kConsoleProcessPrompt = 49;
const int kHTMLPreviewStartedEvent = 51;
const int kHTMLPreviewOutputEvent = 52;
const int kHTMLPreviewCompletedEvent = 53;
const int kCompilePdfStartedEvent = 54;
const int kCompilePdfOutputEvent = 55;
const int kCompilePdfErrorsEvent = 56;
const int kCompilePdfCompletedEvent = 57;
const int kSynctexEditFile = 58;
const int kFindResult = 59;
const int kFindOperationEnded = 60;
const int kRPubsUploadStatus = 61;
const int kBuildStarted = 62;
const int kBuildOutput = 63;
const int kBuildCompleted = 64;
const int kBuildErrors = 65;
const int kDirectoryNavigate = 66;
const int kDeferredInitCompleted = 67;
const int kPlotsZoomSizeChanged = 68;
const int kSourceCppStarted = 69;
const int kSourceCppCompleted = 70;
const int kLoadedPackageUpdates = 71;
const int kActivatePane = 72;
const int kShowPresentationPane = 73;
const int kEnvironmentRefresh = 74;
const int kContextDepthChanged = 75;
const int kEnvironmentAssigned = 76;
const int kEnvironmentRemoved = 77;
const int kBrowserLineChanged = 78;
const int kPackageLoaded = 79;
const int kPackageUnloaded = 80;
const int kPresentationPaneRequestCompleted = 81;
const int kUnhandledError = 82;
const int kErrorHandlerChanged = 83;
const int kViewerNavigate = 84;
const int kSourceExtendedTypeDetected = 85;
const int kShinyViewer = 86;
const int kDebugSourceCompleted = 87;
const int kRmdRenderStarted = 88;
const int kRmdRenderOutput = 89;
const int kRmdRenderCompleted = 90;
const int kRmdTemplateDiscovered = 91;
const int kRmdTemplateDiscoveryCompleted = 92;
const int kRmdShinyDocStarted = 93;
const int kRmdRSConnectDeploymentOutput = 94;
const int kRmdRSConnectDeploymentCompleted = 95;
const int kUserPrompt = 96;
const int kInstallRtools = 97;
const int kInstallShiny = 98;
const int kSuspendAndRestart = 99;
const int kDataViewChanged = 100;
const int kViewFunction = 101;
const int kMarkersChanged = 102;
const int kEnableRStudioConnect = 103;
const int kUpdateGutterMarkers = 104;
const int kSnippetsChanged = 105;
}

void ClientEvent::init(int type, const json::Value& data)
{
   type_ = type;
   data_ = data;
   id_ = core::system::generateUuid();
}
   
void ClientEvent::asJsonObject(int id, json::Object* pObject) const
{
   json::Object& object = *pObject;
   object["id"] = id;
   object["type"] = typeName(); 
   object["data"] = data();
}
   
std::string ClientEvent::typeName() const 
{
   switch(type_)
   {
      case client_events::kBusy:
         return "busy";  
      case client_events::kConsolePrompt:
         return "console_prompt";
      case client_events::kConsoleWriteOutput:
         return "console_output";
      case client_events::kConsoleWriteError: 
         return "console_error";
      case client_events::kShowErrorMessage: 
         return "show_error_message";
      case client_events::kShowHelp: 
         return "show_help";
      case client_events::kBrowseUrl: 
         return "browse_url";
      case client_events::kShowEditor: 
         return "show_editor";
      case client_events::kChooseFile: 
         return "choose_file";
      case client_events::kAbendWarning:
         return "abend_warning";
      case client_events::kQuit:
         return "quit";
      case client_events::kSuicide: 
         return "suicide";
      case client_events::kFileChanged:
         return "file_changed";
      case client_events::kWorkingDirChanged: 
         return "working_dir_changed";
      case client_events::kPlotsStateChanged: 
         return "plots_state_changed";
      case client_events::kViewData: 
         return "view_data";
      case client_events::kPackageStatusChanged: 
         return "package_status_changed";
      case client_events::kPackageStateChanged: 
         return "package_state_changed";
      case client_events::kLocator:
         return "locator";
      case client_events::kConsoleResetHistory:
         return "console_reset_history";
      case client_events::kSessionSerialization:
         return "session_serialization";
      case client_events::kHistoryEntriesAdded:
         return "history_entries_added";
      case client_events::kQuotaStatus:
         return "quota_status";
      case client_events::kFileEdit:
         return "file_edit";
      case client_events::kShowContent:
         return "show_content";
      case client_events::kShowData:
         return "show_data";
      case client_events::kAsyncCompletion:
         return "async_completion";
      case client_events::kSaveActionChanged:
         return "save_action_changed";
      case client_events::kConsoleWritePrompt:
         return "console_write_prompt";
      case client_events::kConsoleWriteInput:
         return "console_write_input";
      case client_events::kShowWarningBar:
         return "show_warning_bar";
      case client_events::kOpenProjectError:
         return "open_project_error";
      case client_events::kVcsRefresh:
         return "vcs_refresh";
      case client_events::kAskPass:
         return "ask_pass";
      case client_events::kConsoleProcessOutput:
         return "console_process_output";
      case client_events::kConsoleProcessExit:
         return "console_process_exit";
      case client_events::kListChanged:
         return "list_changed";
      case client_events::kUiPrefsChanged:
         return "ui_prefs_changed";
      case client_events::kHandleUnsavedChanges:
         return "handle_unsaved_changes";
      case client_events::kConsoleProcessPrompt:
         return "console_process_prompt";
      case client_events::kConsoleProcessCreated:
         return "console_process_created";
      case client_events::kHTMLPreviewStartedEvent:
         return "html_preview_started_event";
      case client_events::kHTMLPreviewOutputEvent:
         return "html_preview_output_event";
      case client_events::kHTMLPreviewCompletedEvent:
         return "html_preview_completed_event";
      case client_events::kCompilePdfStartedEvent:
         return "compile_pdf_started_event";
      case client_events::kCompilePdfOutputEvent:
         return "compile_pdf_output_event";
      case client_events::kCompilePdfErrorsEvent:
         return "compile_pdf_errors_event";
      case client_events::kCompilePdfCompletedEvent:
         return "compile_pdf_completed_event";
      case client_events::kSynctexEditFile:
         return "synctex_edit_file";
      case client_events::kFindResult:
         return "find_result";
      case client_events::kFindOperationEnded:
         return "find_operation_ended";
      case client_events::kRPubsUploadStatus:
         return "rpubs_upload_status";
      case client_events::kBuildStarted:
         return "build_started";
      case client_events::kBuildOutput:
         return "build_output";
      case client_events::kBuildCompleted:
         return "build_completed";
      case client_events::kBuildErrors:
         return "build_errors";
      case client_events::kDirectoryNavigate:
         return "directory_navigate";
      case client_events::kDeferredInitCompleted:
         return "deferred_init_completed";
      case client_events::kPlotsZoomSizeChanged:
         return "plots_zoom_size_changed";
      case client_events::kSourceCppStarted:
         return "source_cpp_started";
      case client_events::kSourceCppCompleted:
         return "source_cpp_completed";
      case client_events::kLoadedPackageUpdates:
         return "loaded_package_updates";
      case client_events::kActivatePane:
         return "activate_pane";
      case client_events::kShowPresentationPane:
         return "show_presentation_pane";
      case client_events::kEnvironmentRefresh:
         return "environment_refresh";   
      case client_events::kContextDepthChanged:
         return "context_depth_changed";
      case client_events::kEnvironmentAssigned:
         return "environment_assigned";
      case client_events::kEnvironmentRemoved:
         return "environment_removed";
      case client_events::kBrowserLineChanged:
         return "browser_line_changed";
      case client_events::kPackageLoaded:
         return "package_loaded";
      case client_events::kPackageUnloaded:
         return "package_unloaded";
      case client_events::kPresentationPaneRequestCompleted:
         return "presentation_pane_request_completed";
      case client_events::kUnhandledError:
         return "unhandled_error";
      case client_events::kErrorHandlerChanged:
         return "error_handler_changed";
      case client_events::kViewerNavigate:
         return "viewer_navigate";
      case client_events::kSourceExtendedTypeDetected:
         return "source_extended_type_detected";
      case client_events::kShinyViewer:
         return "shiny_viewer";
      case client_events::kDebugSourceCompleted:
         return "debug_source_completed";
      case client_events::kRmdRenderStarted:
         return "rmd_render_started";
      case client_events::kRmdRenderOutput:
         return "rmd_render_output";
      case client_events::kRmdRenderCompleted:
         return "rmd_render_completed";
      case client_events::kRmdTemplateDiscovered:
         return "rmd_template_discovered";
      case client_events::kRmdTemplateDiscoveryCompleted:
         return "rmd_template_discovery_completed";
      case client_events::kRmdShinyDocStarted:
         return "rmd_shiny_doc_started";
      case client_events::kRmdRSConnectDeploymentOutput:
         return "rsconnect_deployment_output";
      case client_events::kRmdRSConnectDeploymentCompleted:
         return "rsconnect_deployment_completed";
      case client_events::kUserPrompt:
         return "user_prompt";
      case client_events::kInstallRtools:
         return "install_r_tools";
      case client_events::kInstallShiny:
         return "install_shiny";
      case client_events::kSuspendAndRestart:
         return "suspend_and_restart";
      case client_events::kDataViewChanged:
         return "data_view_changed";
      case client_events::kViewFunction:
         return "view_function";
      case client_events::kMarkersChanged:
         return "markers_changed";
      case client_events::kEnableRStudioConnect:
         return "enable_rstudio_connect";
      case client_events::kUpdateGutterMarkers:
         return "update_gutter_markers";
      case client_events::kSnippetsChanged:
         return "snippets_changed";
      default:
         LOG_WARNING_MESSAGE("unexpected event type: " + 
                             safe_convert::numberToString(type_));
         return "";
   }
}

ClientEvent showEditorEvent(const std::string& content,
                            bool isRCode,
                            bool lineWrapping)
{
   json::Object data;
   data["content"] = content;
   data["is_r_code"] = isRCode;
   data["line_wrapping"] = lineWrapping;
   return ClientEvent(client_events::kShowEditor, data);
}

ClientEvent browseUrlEvent(const std::string& url, const std::string& window)
{
   json::Object browseURLInfo;
   browseURLInfo["url"] = url;
   browseURLInfo["window"] = window;
   return ClientEvent(client_events::kBrowseUrl, browseURLInfo);
}
    
   
ClientEvent showErrorMessageEvent(const std::string& title,
                                  const std::string& message)
{
   json::Object errorMessage ;
   errorMessage["title"] = title;
   errorMessage["message"] = message;
   return ClientEvent(client_events::kShowErrorMessage, errorMessage);
}


   
   
} // namespace session
} // namespace rstudio
