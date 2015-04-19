/*
 * ClientEvent.java
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
package org.rstudio.studio.client.server.remote;

import com.google.gwt.core.client.JavaScriptObject;

class ClientEvent extends JavaScriptObject
{   
   public static final String Busy = "busy";
   public static final String ConsolePrompt = "console_prompt";
   public static final String ConsoleOutput = "console_output" ;
   public static final String ConsoleError = "console_error";
   public static final String ConsoleWritePrompt = "console_write_prompt";
   public static final String ConsoleWriteInput = "console_write_input";
   public static final String ShowErrorMessage = "show_error_message";
   public static final String ShowHelp = "show_help" ;
   public static final String BrowseUrl = "browse_url";
   public static final String ShowEditor = "show_editor";
   public static final String ChooseFile = "choose_file";
   public static final String AbendWarning = "abend_warning";
   public static final String Quit = "quit";
   public static final String Suicide = "suicide";
   public static final String FileChanged = "file_changed";
   public static final String WorkingDirChanged = "working_dir_changed";
   public static final String PlotsStateChanged = "plots_state_changed";
   public static final String ViewData = "view_data";
   public static final String PackageStatusChanged = "package_status_changed";
   public static final String PackageStateChanged = "package_state_changed";
   public static final String Locator = "locator";
   public static final String ConsoleResetHistory = "console_reset_history";
   public static final String SessionSerialization = "session_serialization";
   public static final String HistoryEntriesAdded = "history_entries_added";
   public static final String QuotaStatus = "quota_status";
   public static final String FileEdit = "file_edit";
   public static final String ShowContent = "show_content";
   public static final String ShowData = "show_data";
   public static final String AsyncCompletion = "async_completion";
   public static final String SaveActionChanged = "save_action_changed";
   public static final String ShowWarningBar = "show_warning_bar";
   public static final String OpenProjectError = "open_project_error";
   public static final String VcsRefresh = "vcs_refresh";
   public static final String AskPass = "ask_pass";
   public static final String ConsoleProcessOutput = "console_process_output";
   public static final String ConsoleProcessExit = "console_process_exit";
   public static final String ListChanged = "list_changed";
   public static final String UiPrefsChanged = "ui_prefs_changed";
   public static final String HandleUnsavedChanges = "handle_unsaved_changes";
   public static final String PosixShellOutput = "posix_shell_output";
   public static final String PosixShellExit = "posix_shell_exit";
   public static final String ConsoleProcessPrompt = "console_process_prompt";
   public static final String ConsoleProcessCreated = "console_process_created";
   public static final String HTMLPreviewStartedEvent = "html_preview_started_event";
   public static final String HTMLPreviewOutputEvent = "html_preview_output_event";
   public static final String HTMLPreviewCompletedEvent = "html_preview_completed_event";
   public static final String CompilePdfStartedEvent = "compile_pdf_started_event";
   public static final String CompilePdfOutputEvent = "compile_pdf_output_event";
   public static final String CompilePdfErrorsEvent = "compile_pdf_errors_event";
   public static final String CompilePdfCompletedEvent = "compile_pdf_completed_event";
   public static final String SynctexEditFile = "synctex_edit_file";
   public static final String FindResult = "find_result";
   public static final String FindOperationEnded = "find_operation_ended";
   public static final String RPubsUploadStatus = "rpubs_upload_status";
   public static final String BuildStarted = "build_started";
   public static final String BuildOutput = "build_output";
   public static final String BuildCompleted = "build_completed";
   public static final String BuildErrors = "build_errors";
   public static final String DirectoryNavigate = "directory_navigate";
   public static final String DeferredInitCompleted = "deferred_init_completed";
   public static final String PlotsZoomSizeChanged = "plots_zoom_size_changed";
   public static final String SourceCppStarted = "source_cpp_started";
   public static final String SourceCppCompleted = "source_cpp_completed";
   public static final String LoadedPackageUpdates = "loaded_package_updates";
   public static final String ActivatePane = "activate_pane";
   public static final String ShowPresentationPane = "show_presentation_pane";
   public static final String EnvironmentRefresh = "environment_refresh";
   public static final String ContextDepthChanged = "context_depth_changed";
   public static final String EnvironmentAssigned = "environment_assigned";
   public static final String EnvironmentRemoved = "environment_removed";
   public static final String BrowserLineChanged = "browser_line_changed";
   public static final String PackageLoaded = "package_loaded";
   public static final String PackageUnloaded = "package_unloaded";
   public static final String PresentationPaneRequestCompleted = "presentation_pane_request_completed";
   public static final String UnhandledError = "unhandled_error";
   public static final String ErrorHandlerChanged = "error_handler_changed";
   public static final String ViewerNavigate = "viewer_navigate";
   public static final String UpdateCheck = "update_check";
   public static final String SourceExtendedTypeDetected = "source_extended_type_detected";
   public static final String ShinyViewer = "shiny_viewer";
   public static final String DebugSourceCompleted = "debug_source_completed";
   public static final String RmdRenderStarted = "rmd_render_started";
   public static final String RmdRenderOutput = "rmd_render_output";
   public static final String RmdRenderCompleted = "rmd_render_completed";
   public static final String RmdTemplateDiscovered = "rmd_template_discovered";
   public static final String RmdTemplateDiscoveryCompleted = "rmd_template_discovery_completed";
   public static final String RmdShinyDocStarted = "rmd_shiny_doc_started";
   public static final String RSConnectDeploymentOutput = "rsconnect_deployment_output";
   public static final String RSConnectDeploymentCompleted = "rsconnect_deployment_completed";
   public static final String UserPrompt = "user_prompt";
   public static final String InstallRtools = "install_r_tools";
   public static final String InstallShiny = "install_shiny";
   public static final String SuspendAndRestart = "suspend_and_restart";
   public static final String PackratRestoreNeeded = "packrat_restore_needed";
   public static final String DataViewChanged = "data_view_changed";
   public static final String ViewFunction = "view_function";
   public static final String MarkersChanged = "markers_changed";
   public static final String EnableRStudioConnect = "enable_rstudio_connect";
   public static final String UpdateGutterMarkers = "update_gutter_markers";
   public static final String SnippetsChanged = "snippets_changed";
   
   protected ClientEvent()
   {
   }
   
   public final native int getId() /*-{
      return this.id;
   }-*/;
   
   public final native String getType() /*-{
      return this.type;
   }-*/;
   
   public final native <T> T getData() /*-{
      return this.data;
   }-*/;
}
