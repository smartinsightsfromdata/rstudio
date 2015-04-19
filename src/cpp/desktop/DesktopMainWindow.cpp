/*
 * DesktopMainWindow.cpp
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

#include "DesktopMainWindow.hpp"

#include <algorithm>

#include <QtGui>
#include <QtWebKit>
#include <QToolBar>
#include <QWebFrame>

#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <core/FilePath.hpp>
#include <core/system/System.hpp>

#include "DesktopGwtCallback.hpp"
#include "DesktopMenuCallback.hpp"
#include "DesktopWebView.hpp"
#include "DesktopOptions.hpp"
#include "DesktopSlotBinders.hpp"
#include "DesktopUtils.hpp"
#include "DesktopSessionLauncher.hpp"

using namespace rstudio::core;

namespace rstudio {
namespace desktop {

MainWindow::MainWindow(QUrl url) :
      GwtWindow(false, false, QString(), url, NULL),
      menuCallback_(this),
      gwtCallback_(this, this),
      pSessionLauncher_(NULL),
      pCurrentSessionProcess_(NULL)
{
   quitConfirmed_ = false;
   pToolbar_->setVisible(false);

   // initialize zoom levels
   zoomLevels_.push_back(1.0);
   zoomLevels_.push_back(1.1);
   zoomLevels_.push_back(1.20);
   zoomLevels_.push_back(1.30);
   zoomLevels_.push_back(1.40);
   zoomLevels_.push_back(1.50);
   zoomLevels_.push_back(1.75);
   zoomLevels_.push_back(2.00);

   // Dummy menu bar to deal with the fact that
   // the real menu bar isn't ready until well
   // after startup.
   QMenuBar* pMainMenuStub = new QMenuBar(this);
   pMainMenuStub->addMenu(QString::fromUtf8("File"));
   pMainMenuStub->addMenu(QString::fromUtf8("Edit"));
   pMainMenuStub->addMenu(QString::fromUtf8("Code"));
   pMainMenuStub->addMenu(QString::fromUtf8("View"));
   pMainMenuStub->addMenu(QString::fromUtf8("Plots"));
   pMainMenuStub->addMenu(QString::fromUtf8("Session"));
   pMainMenuStub->addMenu(QString::fromUtf8("Build"));
   pMainMenuStub->addMenu(QString::fromUtf8("Debug"));
   pMainMenuStub->addMenu(QString::fromUtf8("Tools"));
   pMainMenuStub->addMenu(QString::fromUtf8("Help"));
   setMenuBar(pMainMenuStub);

   connect(&menuCallback_, SIGNAL(menuBarCompleted(QMenuBar*)),
           this, SLOT(setMenuBar(QMenuBar*)));
   connect(&menuCallback_, SIGNAL(commandInvoked(QString)),
           this, SLOT(invokeCommand(QString)));
   connect(&menuCallback_, SIGNAL(manageCommand(QString,QAction*)),
           this, SLOT(manageCommand(QString,QAction*)));
   connect(&menuCallback_, SIGNAL(manageCommandVisibility(QString,QAction*)),
           this, SLOT(manageCommandVisibility(QString,QAction*)));

   connect(&menuCallback_, SIGNAL(zoomIn()), this, SLOT(zoomIn()));
   connect(&menuCallback_, SIGNAL(zoomOut()), this, SLOT(zoomOut()));

   connect(&gwtCallback_, SIGNAL(workbenchInitialized()),
           this, SIGNAL(firstWorkbenchInitialized()));
   connect(&gwtCallback_, SIGNAL(workbenchInitialized()),
           this, SLOT(onWorkbenchInitialized()));

   connect(webView(), SIGNAL(onCloseWindowShortcut()),
           this, SLOT(onCloseWindowShortcut()));

   connect(qApp, SIGNAL(commitDataRequest(QSessionManager&)),
           this, SLOT(commitDataRequest(QSessionManager&)),
           Qt::DirectConnection);

   setWindowIcon(QIcon(QString::fromUtf8(":/icons/RStudio.ico")));

   setWindowTitle(QString::fromUtf8("RStudio"));

#ifdef Q_OS_MAC
   QMenuBar* pDefaultMenu = new QMenuBar();
   pDefaultMenu->addMenu(new WindowMenu());
#endif

   desktop::enableFullscreenMode(this, true);

   //setContentsMargins(10000, 0, -10000, 0);
   setStyleSheet(QString::fromUtf8("QMainWindow { background: #e1e2e5; }"));
}

QString MainWindow::getSumatraPdfExePath()
{
   QWebFrame* pMainFrame = webView()->page()->mainFrame();
   QString sumatraPath = pMainFrame->evaluateJavaScript(QString::fromUtf8(
                    "window.desktopHooks.getSumatraPdfExePath()")).toString();
   return sumatraPath;
}

void MainWindow::launchSession(bool reload)
{
   Error error = pSessionLauncher_->launchNextSession(reload);
   if (error)
   {
      LOG_ERROR(error);

      showMessageBox(QMessageBox::Critical,
                     this,
                     QString::fromUtf8("RStudio"),
                     QString::fromUtf8("The R session failed to start."));

      quit();
   }
}

void MainWindow::onCloseWindowShortcut()
{
   QWebFrame* pMainFrame = webView()->page()->mainFrame();

   bool closeSourceDocEnabled = pMainFrame->evaluateJavaScript(
      QString::fromUtf8(
         "window.desktopHooks.isCommandEnabled('closeSourceDoc')")).toBool();

   if (!closeSourceDocEnabled)
      close();
}


void MainWindow::onWorkbenchInitialized()
{
   //QTimer::singleShot(300, this, SLOT(resetMargins()));

   // reset state (in case this occurred in response to a manual reload
   // or reload for a new project context)
   quitConfirmed_ = false;

   // see if there is a project dir to display in the titlebar
   // if there are unsaved changes then resolve them before exiting
   QVariant vProjectDir = webView()->page()->mainFrame()->evaluateJavaScript(
         QString::fromUtf8("window.desktopHooks.getActiveProjectDir()"));
   QString projectDir = vProjectDir.toString();
   if (projectDir.length() > 0)
      setWindowTitle(projectDir + QString::fromUtf8(" - RStudio"));
   else
      setWindowTitle(QString::fromUtf8("RStudio"));

   avoidMoveCursorIfNecessary();
}

void MainWindow::resetMargins()
{
   setContentsMargins(0, 0, 0, 0);
}

// this notification occurs when windows or X11 is shutting
// down -- in this case we want to be a good citizen and just
// exit right away so we notify the gwt callback that a legit
// quit and exit is on the way and we set the quitConfirmed_
// flag so no prompting occurs (note that source documents
// have already been auto-saved so will be restored next time
// the current project context is opened)
void MainWindow::commitDataRequest(QSessionManager &manager)
{
   gwtCallback_.setPendingQuit(PendingQuitAndExit);
   quitConfirmed_ = true;
}

void MainWindow::loadUrl(const QUrl& url)
{
   webView()->setBaseUrl(url);
   webView()->load(url);
}

void MainWindow::quit()
{
   quitConfirmed_ = true;
   close();
}

void MainWindow::onJavaScriptWindowObjectCleared()
{
   double zoomLevel = options().zoomLevel();
   if (zoomLevel != webView()->dpiAwareZoomFactor())
      webView()->setDpiAwareZoomFactor(zoomLevel);

   webView()->page()->mainFrame()->addToJavaScriptWindowObject(
         QString::fromUtf8("desktop"),
         &gwtCallback_,
         QWebFrame::QtOwnership);
   webView()->page()->mainFrame()->addToJavaScriptWindowObject(
         QString::fromUtf8("desktopMenuCallback"),
         &menuCallback_,
         QWebFrame::QtOwnership);
}

void MainWindow::invokeCommand(QString commandId)
{
   webView()->page()->mainFrame()->evaluateJavaScript(
         QString::fromUtf8("window.desktopHooks.invokeCommand('") + commandId + QString::fromUtf8("');"));
}

void MainWindow::zoomIn()
{
   // get next greatest value
   std::vector<double>::const_iterator it = std::upper_bound(
            zoomLevels_.begin(), zoomLevels_.end(), options().zoomLevel());
   if (it != zoomLevels_.end())
   {
      options().setZoomLevel(*it);
      webView()->reload();
   }
}

void MainWindow::zoomOut()
{
   // get next smallest value
   std::vector<double>::const_iterator it = std::lower_bound(
            zoomLevels_.begin(), zoomLevels_.end(), options().zoomLevel());
   if (it != zoomLevels_.begin() && it != zoomLevels_.end())
   {
      options().setZoomLevel(*(it-1));
      webView()->reload();
   }
}

void MainWindow::manageCommand(QString cmdId, QAction* action)
{
   QWebFrame* pMainFrame = webView()->page()->mainFrame();
   action->setVisible(pMainFrame->evaluateJavaScript(
         QString::fromUtf8("window.desktopHooks.isCommandVisible('") + cmdId + QString::fromUtf8("')")).toBool());
   action->setEnabled(pMainFrame->evaluateJavaScript(
         QString::fromUtf8("window.desktopHooks.isCommandEnabled('") + cmdId + QString::fromUtf8("')")).toBool());
   action->setText(pMainFrame->evaluateJavaScript(
         QString::fromUtf8("window.desktopHooks.getCommandLabel('") + cmdId + QString::fromUtf8("')")).toString());
   if (action->isCheckable())
   {
      action->setChecked(pMainFrame->evaluateJavaScript(
         QString::fromUtf8("window.desktopHooks.isCommandChecked('") + cmdId + QString::fromUtf8("')")).toBool());
   }
}

// a faster version of the above that just checks and sets the command's
// visibility state (to trigger visibility of menus containing the command)
void MainWindow::manageCommandVisibility(QString cmdId, QAction* action)
{
   QWebFrame* pMainFrame = webView()->page()->mainFrame();
   action->setVisible(pMainFrame->evaluateJavaScript(
         QString::fromUtf8("window.desktopHooks.isCommandVisible('") + cmdId + QString::fromUtf8("')")).toBool());
}

void MainWindow::evaluateJavaScript(QString jsCode)
{
   QWebFrame* pMainFrame = webView()->page()->mainFrame();
   pMainFrame->evaluateJavaScript(jsCode);
}

void MainWindow::closeEvent(QCloseEvent* pEvent)
{
   QWebFrame* pFrame = webView()->page()->mainFrame();
   if (!pFrame)
   {
       pEvent->accept();
       return;
   }

   QVariant hasQuitR = pFrame->evaluateJavaScript(QString::fromUtf8("!!window.desktopHooks"));

   if (quitConfirmed_
       || !hasQuitR.toBool()
       || pCurrentSessionProcess_ == NULL
       || pCurrentSessionProcess_->state() != QProcess::Running)
   {
      pEvent->accept();
   }
   else
   {
      pFrame->evaluateJavaScript(QString::fromUtf8("window.desktopHooks.quitR()"));
      pEvent->ignore();
   }
}

void MainWindow::setMenuBar(QMenuBar *pMenubar)
{
   delete menuBar();
   this->QMainWindow::setMenuBar(pMenubar);
}

void MainWindow::openFileInRStudio(QString path)
{
   QFileInfo fileInfo(path);
   if (!fileInfo.isAbsolute() || !fileInfo.exists() || !fileInfo.isFile())
      return;

   path = path.replace(QString::fromUtf8("\\"), QString::fromUtf8("\\\\"))
          .replace(QString::fromUtf8("\""), QString::fromUtf8("\\\""))
          .replace(QString::fromUtf8("\n"), QString::fromUtf8("\\n"));

   webView()->page()->mainFrame()->evaluateJavaScript(
         QString::fromUtf8("window.desktopHooks.openFile(\"") + path + QString::fromUtf8("\")"));
}

void MainWindow::onPdfViewerClosed(QString pdfPath)
{
   webView()->page()->mainFrame()->evaluateJavaScript(
            QString::fromUtf8("window.synctexNotifyPdfViewerClosed(\"") +
                                         pdfPath + QString::fromUtf8("\")"));
}

void MainWindow::onPdfViewerSyncSource(QString srcFile, int line, int column)
{
   boost::format fmt("window.desktopSynctexInverseSearch(\"%1%\", %2%, %3%)");
   std::string js = boost::str(fmt % srcFile.toStdString() % line % column);

   webView()->page()->mainFrame()->evaluateJavaScript(
                                                   QString::fromStdString(js));
}

// private interface for SessionLauncher

void MainWindow::setSessionLauncher(SessionLauncher* pSessionLauncher)
{
   pSessionLauncher_ = pSessionLauncher;
}

void MainWindow::setSessionProcess(QProcess* pSessionProcess)
{
   pCurrentSessionProcess_ = pSessionProcess;
}

// allow SessionLauncher to collect restart requests from GwtCallback
int MainWindow::collectPendingQuitRequest()
{
   return gwtCallback_.collectPendingQuitRequest();
}

bool MainWindow::desktopHooksAvailable()
{
   return webView()->page()->mainFrame()->evaluateJavaScript(
                        QString::fromUtf8("!!window.desktopHooks")).toBool();
}

void MainWindow::onActivated()
{
   if (desktopHooksAvailable())
      invokeCommand(QString::fromUtf8("vcsRefreshNoError"));
}

} // namespace desktop
} // namespace rstudio
