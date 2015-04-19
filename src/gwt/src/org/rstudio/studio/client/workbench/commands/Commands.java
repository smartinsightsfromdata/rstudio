/*
 * Commands.java
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
package org.rstudio.studio.client.workbench.commands;

import org.rstudio.core.client.command.AppCommand;
import org.rstudio.core.client.command.CommandBundle;
import org.rstudio.core.client.command.MenuCallback;

public abstract class
      Commands extends CommandBundle
{
   public abstract void mainMenu(MenuCallback callback);

   // Workbench
   public abstract AppCommand setWorkingDir();
   
   // Source
   public abstract AppCommand reformatCode();
   public abstract AppCommand newSourceDoc();
   public abstract AppCommand newTextDoc();
   public abstract AppCommand newCppDoc();
   public abstract AppCommand newSweaveDoc();
   public abstract AppCommand newRMarkdownDoc();
   public abstract AppCommand newRHTMLDoc();
   public abstract AppCommand newRDocumentationDoc();
   public abstract AppCommand newRPresentationDoc();
   public abstract AppCommand openSourceDoc();
   public abstract AppCommand reopenSourceDocWithEncoding();
   public abstract AppCommand saveSourceDoc();
   public abstract AppCommand saveSourceDocAs();
   public abstract AppCommand saveSourceDocWithEncoding();
   public abstract AppCommand saveAllSourceDocs();
   public abstract AppCommand closeSourceDoc();
   public abstract AppCommand closeAllSourceDocs();
   public abstract AppCommand executeAllCode();
   public abstract AppCommand sourceFile();
   public abstract AppCommand sourceActiveDocument();
   public abstract AppCommand sourceActiveDocumentWithEcho();
   public abstract AppCommand executeCode();
   public abstract AppCommand executeCodeWithoutMovingCursor();
   public abstract AppCommand executeCodeWithoutFocus();
   public abstract AppCommand executeToCurrentLine();
   public abstract AppCommand executeFromCurrentLine();
   public abstract AppCommand executeCurrentFunction();
   public abstract AppCommand executeCurrentSection();
   public abstract AppCommand executeLastCode();
   public abstract AppCommand insertChunk();
   public abstract AppCommand insertSection();
   public abstract AppCommand executePreviousChunks();
   public abstract AppCommand executeCurrentChunk();
   public abstract AppCommand executeNextChunk();
   public abstract AppCommand goToHelp();
   public abstract AppCommand rcppHelp();
   public abstract AppCommand goToFunctionDefinition();
   public abstract AppCommand sourceNavigateBack();
   public abstract AppCommand sourceNavigateForward();
   public abstract AppCommand markdownHelp();
   public abstract AppCommand usingRMarkdownHelp();
   public abstract AppCommand authoringRPresentationsHelp();
   public abstract AppCommand knitDocument();
   public abstract AppCommand previewHTML();
   public abstract AppCommand publishHTML();
   public abstract AppCommand compilePDF();
   public abstract AppCommand compileNotebook();
   public abstract AppCommand synctexSearch();
   public abstract AppCommand activateSource();
   public abstract AppCommand printSourceDoc();
   public abstract AppCommand vcsFileLog();
   public abstract AppCommand vcsFileDiff();
   public abstract AppCommand vcsFileRevert();
   public abstract AppCommand popoutDoc();
   public abstract AppCommand findReplace();
   public abstract AppCommand findNext();
   public abstract AppCommand findPrevious();
   public abstract AppCommand findSelectAll();
   public abstract AppCommand findFromSelection();
   public abstract AppCommand replaceAndFind();
   public abstract AppCommand findInFiles();
   public abstract AppCommand fold();
   public abstract AppCommand unfold();
   public abstract AppCommand foldAll();
   public abstract AppCommand unfoldAll();
   public abstract AppCommand jumpToMatching();
   public abstract AppCommand selectToMatching();
   public abstract AppCommand expandToMatching();
   public abstract AppCommand extractFunction();
   public abstract AppCommand extractLocalVariable();
   public abstract AppCommand commentUncomment();
   public abstract AppCommand reindent();
   public abstract AppCommand reflowComment();
   public abstract AppCommand setWorkingDirToActiveDoc();
   public abstract AppCommand codeCompletion();
   public abstract AppCommand findUsages();
   public abstract AppCommand editRmdFormatOptions();
   public abstract AppCommand insertRoxygenSkeleton();
   public abstract AppCommand insertSnippet();
 
   // Projects
   public abstract AppCommand newProject();
   public abstract AppCommand openProject();
   public abstract AppCommand openProjectInNewWindow();
   public abstract AppCommand projectMru0();
   public abstract AppCommand projectMru1();
   public abstract AppCommand projectMru2();
   public abstract AppCommand projectMru3();
   public abstract AppCommand projectMru4();
   public abstract AppCommand projectMru5();
   public abstract AppCommand projectMru6();
   public abstract AppCommand projectMru7();
   public abstract AppCommand projectMru8();
   public abstract AppCommand projectMru9();
   public abstract AppCommand clearRecentProjects();
   public abstract AppCommand closeProject();
   public abstract AppCommand projectOptions();
   public abstract AppCommand projectSweaveOptions();
   public abstract AppCommand setWorkingDirToProjectDir();

   // Console
   public abstract AppCommand consoleClear();
   public abstract AppCommand interruptR();
   public abstract AppCommand restartR();
   public abstract AppCommand terminateR();
   public abstract AppCommand activateConsole();

   // Files
   public abstract AppCommand newFolder();
   public abstract AppCommand uploadFile();
   public abstract AppCommand copyFile();
   public abstract AppCommand moveFiles();
   public abstract AppCommand exportFiles();
   public abstract AppCommand renameFile();
   public abstract AppCommand deleteFiles();
   public abstract AppCommand refreshFiles();
   public abstract AppCommand activateFiles();
   public abstract AppCommand goToWorkingDir();
   public abstract AppCommand setAsWorkingDir();
   public abstract AppCommand setWorkingDirToFilesPane();
   public abstract AppCommand showFolder();
 
   // VCS
   public abstract AppCommand vcsDiff();
   public abstract AppCommand vcsCommit();
   public abstract AppCommand vcsRevert();
   public abstract AppCommand vcsViewOnGitHub();
   public abstract AppCommand vcsBlameOnGitHub();
   public abstract AppCommand vcsShowHistory();
   public abstract AppCommand vcsRefresh();
   public abstract AppCommand vcsRefreshNoError();
   public abstract AppCommand vcsOpen();
   public abstract AppCommand vcsIgnore();
   public abstract AppCommand vcsPull();
   public abstract AppCommand vcsPush();
   public abstract AppCommand vcsCleanup();
   public abstract AppCommand vcsAddFiles();
   public abstract AppCommand vcsRemoveFiles();
   public abstract AppCommand activateVcs();
   public abstract AppCommand vcsResolve();
   
   // PDF
   public abstract AppCommand showPdfExternal();
   
   // HTML preview
   public abstract AppCommand openHtmlExternal();
   public abstract AppCommand saveHtmlPreviewAsLocalFile();
   public abstract AppCommand saveHtmlPreviewAs();
   public abstract AppCommand refreshHtmlPreview();
   public abstract AppCommand showHtmlPreviewLog();
   
   // Presentation
   public abstract AppCommand refreshPresentation();
   public abstract AppCommand presentationFullscreen();
   public abstract AppCommand presentationHome();
   public abstract AppCommand presentationNext();
   public abstract AppCommand presentationPrev();
   public abstract AppCommand presentationEdit();
   public abstract AppCommand presentationViewInBrowser();
   public abstract AppCommand presentationSaveAsStandalone();
   public abstract AppCommand presentationPublishToRpubs();
   public abstract AppCommand activatePresentation();
   public abstract AppCommand tutorialFeedback();
   public abstract AppCommand clearPresentationCache();
   
   // View
   public abstract AppCommand showToolbar();
   public abstract AppCommand hideToolbar();
   public abstract AppCommand zoomActualSize();
   public abstract AppCommand zoomIn();
   public abstract AppCommand zoomOut();
   public abstract AppCommand jumpTo();
   public abstract AppCommand goToFileFunction();
   public abstract AppCommand switchToTab();
   public abstract AppCommand previousTab();
   public abstract AppCommand nextTab();
   public abstract AppCommand firstTab();
   public abstract AppCommand lastTab();
   public abstract AppCommand goToLine();
   public abstract AppCommand toggleFullScreen();

   // History
   public abstract AppCommand historySendToSource();
   public abstract AppCommand historySendToConsole();
   public abstract AppCommand searchHistory();
   public abstract AppCommand loadHistory();
   public abstract AppCommand saveHistory();
   public abstract AppCommand historyRemoveEntries();
   public abstract AppCommand clearHistory();
   public abstract AppCommand historyDismissResults();
   public abstract AppCommand historyShowContext();
   public abstract AppCommand historyDismissContext();
   public abstract AppCommand activateHistory();

   // Workspace
   public abstract AppCommand clearWorkspace();
   public abstract AppCommand refreshWorkspace();
   public abstract AppCommand saveWorkspace();
   public abstract AppCommand loadWorkspace();
   public abstract AppCommand importDatasetFromFile();
   public abstract AppCommand importDatasetFromURL();

   // Environment
   public abstract AppCommand activateEnvironment();
   public abstract AppCommand refreshEnvironment();
 
   // Plots
   public abstract AppCommand nextPlot();
   public abstract AppCommand previousPlot();
   public abstract AppCommand savePlotAsImage();
   public abstract AppCommand savePlotAsPdf();
   public abstract AppCommand copyPlotToClipboard();
   public abstract AppCommand publishPlotToRPubs();
   public abstract AppCommand zoomPlot();
   public abstract AppCommand removePlot();
   public abstract AppCommand clearPlots();
   public abstract AppCommand refreshPlot();
   public abstract AppCommand activatePlots();
   public abstract AppCommand showManipulator();

   // Packages
   public abstract AppCommand installPackage();
   public abstract AppCommand updatePackages();
   public abstract AppCommand refreshPackages();
   public abstract AppCommand activatePackages();
   
   // // packrat
   public abstract AppCommand packratBootstrap();
   public abstract AppCommand packratOptions();
   public abstract AppCommand packratBundle();
   public abstract AppCommand packratHelp();
   public abstract AppCommand packratClean();

   // Version control
   public abstract AppCommand versionControlHelp();
   public abstract AppCommand versionControlShowRsaKey();
   public abstract AppCommand versionControlProjectSetup();
   
   // Profiler
   public abstract AppCommand showProfiler();
   public abstract AppCommand startProfiler();
   public abstract AppCommand stopProfiler();
   
   // Tools
   public abstract AppCommand showShellDialog();
   public abstract AppCommand macPreferences();
   public abstract AppCommand showOptions();

   // Help
   public abstract AppCommand helpBack();
   public abstract AppCommand helpForward();
   public abstract AppCommand helpHome();
   public abstract AppCommand printHelp();
   public abstract AppCommand clearHelpHistory();
   public abstract AppCommand helpPopout();
   public abstract AppCommand refreshHelp();
   public abstract AppCommand raiseException();
   public abstract AppCommand raiseException2();
   public abstract AppCommand activateHelp();
   public abstract AppCommand showAboutDialog();
   public abstract AppCommand checkForUpdates();
   public abstract AppCommand helpUsingRStudio();
   public abstract AppCommand helpKeyboardShortcuts();
   public abstract AppCommand showRequestLog();
   public abstract AppCommand logFocusedElement();
   public abstract AppCommand debugDumpContents();
   public abstract AppCommand debugImportDump();
   public abstract AppCommand refreshSuperDevMode();
   public abstract AppCommand viewShortcuts();
   
   // Viewer
   public abstract AppCommand viewerPopout();
   public abstract AppCommand viewerBack(); 
   public abstract AppCommand viewerForward();
   public abstract AppCommand viewerZoom();
   public abstract AppCommand viewerRefresh();
   public abstract AppCommand viewerSaveAllAndRefresh();
   public abstract AppCommand viewerStop();
   public abstract AppCommand viewerClear();
   public abstract AppCommand viewerClearAll();
   public abstract AppCommand viewerSaveAsImage();
   public abstract AppCommand viewerSaveAsWebPage();
   public abstract AppCommand viewerCopyToClipboard();
   public abstract AppCommand viewerPublishToRPubs();

   // Application
   public abstract AppCommand quitSession();
   public abstract AppCommand updateCredentials();
   public abstract AppCommand diagnosticsReport();
   public abstract AppCommand showLogFiles();
   public abstract AppCommand rstudioSupport();
   public abstract AppCommand rstudioAgreement();

   public abstract AppCommand showWarningBar();
 
   // Build
   public abstract AppCommand buildAll();
   public abstract AppCommand devtoolsLoadAll();
   public abstract AppCommand rebuildAll();
   public abstract AppCommand cleanAll();
   public abstract AppCommand buildSourcePackage();
   public abstract AppCommand buildBinaryPackage();
   public abstract AppCommand roxygenizePackage();
   public abstract AppCommand checkPackage();
   public abstract AppCommand testPackage();
   public abstract AppCommand stopBuild();
   public abstract AppCommand buildToolsProjectSetup();
   public abstract AppCommand activateBuild();
   
   // Clipboard placeholders
   public abstract AppCommand undoDummy();
   public abstract AppCommand redoDummy();
   public abstract AppCommand cutDummy();
   public abstract AppCommand copyDummy();
   public abstract AppCommand pasteDummy();

   public abstract AppCommand mru0();
   public abstract AppCommand mru1();
   public abstract AppCommand mru2();
   public abstract AppCommand mru3();
   public abstract AppCommand mru4();
   public abstract AppCommand mru5();
   public abstract AppCommand mru6();
   public abstract AppCommand mru7();
   public abstract AppCommand mru8();
   public abstract AppCommand mru9();
   public abstract AppCommand clearRecentFiles();

   // Debugging
   public abstract AppCommand debugBreakpoint();
   public abstract AppCommand debugClearBreakpoints();
   public abstract AppCommand debugContinue();
   public abstract AppCommand debugStop();
   public abstract AppCommand debugStep();
   public abstract AppCommand debugStepInto();
   public abstract AppCommand debugFinish();
   public abstract AppCommand debugHelp();   
   public abstract AppCommand errorsMessage();
   public abstract AppCommand errorsTraceback();
   public abstract AppCommand errorsBreak();
   public abstract AppCommand showDiagnosticsActiveDocument();
   public abstract AppCommand showDiagnosticsProject();
   
   // Shiny IDE features
   public abstract AppCommand reloadShinyApp();
   public abstract AppCommand shinyRunInPane();
   public abstract AppCommand shinyRunInViewer();
   public abstract AppCommand shinyRunInBrowser();
   
   // RSConnect connectivity
   public abstract AppCommand rsconnectDeploy();
   public abstract AppCommand rsconnectConfigure();
   public abstract AppCommand rsconnectManageAccounts();

   // Other
   public abstract AppCommand checkSpelling();   
   public abstract AppCommand maximizeConsole();
}
