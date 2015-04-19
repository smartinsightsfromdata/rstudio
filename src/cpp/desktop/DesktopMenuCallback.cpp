/*
 * DesktopMenuCallback.cpp
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

#include "DesktopMenuCallback.hpp"
#include <QDebug>
#include <QApplication>
#include "DesktopCommandInvoker.hpp"
#include "DesktopSubMenu.hpp"

#ifdef Q_OS_MAC
#include <ApplicationServices/ApplicationServices.h>
#endif

namespace rstudio {
namespace desktop {

MenuCallback::MenuCallback(QObject *parent) :
    QObject(parent)
{
}

void MenuCallback::beginMainMenu()
{
   pMainMenu_ = new QMenuBar();
}

void MenuCallback::beginMenu(QString label)
{
#ifdef Q_OS_MAC
   if (label == QString::fromUtf8("&Help"))
   {
      pMainMenu_->addMenu(new WindowMenu(pMainMenu_));
   }
#endif

   SubMenu* pMenu = new SubMenu(label, pMainMenu_);

   connect(pMenu, SIGNAL(manageCommandVisibility(QString,QAction*)),
           this, SIGNAL(manageCommandVisibility(QString,QAction*)));

   if (menuStack_.count() == 0)
      pMainMenu_->addMenu(pMenu);
   else
      menuStack_.top()->addMenu(pMenu);

   menuStack_.push(pMenu);
}

QAction* MenuCallback::addCustomAction(QString commandId,
                                       QString label,
                                       QString tooltip)
{

   QAction* pAction = NULL;
   if (commandId == QString::fromUtf8("zoomIn"))
   {
      pAction = menuStack_.top()->addAction(QIcon(),
                                            label,
                                            this,
                                            SIGNAL(zoomIn()),
                                            QKeySequence::ZoomIn);
   }
   else if (commandId == QString::fromUtf8("zoomOut"))
   {
      pAction = menuStack_.top()->addAction(QIcon(),
                                            label,
                                            this,
                                            SIGNAL(zoomOut()),
                                            QKeySequence::ZoomOut);
   }
#ifdef Q_OS_LINUX
   else if (commandId == QString::fromUtf8("nextTab"))
   {
      pAction = menuStack_.top()->addAction(QIcon(),
                                            label,
                                            this,
                                            SLOT(actionInvoked()),
                                            QKeySequence(Qt::CTRL +
                                                         Qt::Key_PageDown));
   }
   else if (commandId == QString::fromUtf8("previousTab"))
   {
      pAction = menuStack_.top()->addAction(QIcon(),
                                            label,
                                            this,
                                            SLOT(actionInvoked()),
                                            QKeySequence(Qt::CTRL +
                                                         Qt::Key_PageUp));
   }
#endif

   if (pAction != NULL)
   {
      pAction->setData(commandId);
      pAction->setToolTip(tooltip);
      return pAction;
   }
   else
   {
      return NULL;
   }
}


void MenuCallback::addCommand(QString commandId,
                              QString label,
                              QString tooltip,
                              QString shortcut,
                              bool checkable)
{
   shortcut = shortcut.replace(QString::fromUtf8("Enter"), QString::fromUtf8("\n"));

   QKeySequence keySequence(shortcut);
#ifndef Q_OS_MAC
   if (shortcut.contains(QString::fromUtf8("\n")))
   {
      int value = (keySequence[0] & Qt::MODIFIER_MASK) + Qt::Key_Enter;
      keySequence = QKeySequence(value);
   }
#endif

   // allow custom action handlers first shot
   QAction* pAction = addCustomAction(commandId, label, tooltip);

   // if there was no custom handler then do stock command-id processing
   if (pAction == NULL)
   {
      pAction = menuStack_.top()->addAction(QIcon(),
                                            label,
                                            this,
                                            SLOT(actionInvoked()),
                                            keySequence);
      pAction->setData(commandId);
      pAction->setToolTip(tooltip);
      if (checkable)
         pAction->setCheckable(true);

      MenuActionBinder* pBinder = new MenuActionBinder(menuStack_.top(), pAction);
      connect(pBinder, SIGNAL(manageCommand(QString,QAction*)),
              this, SIGNAL(manageCommand(QString,QAction*)));
   }
}

void MenuCallback::actionInvoked()
{
   QAction* action = qobject_cast<QAction*>(sender());
   QString commandId = action->data().toString();
   commandInvoked(commandId);
}

void MenuCallback::addSeparator()
{
   if (menuStack_.count() > 0)
      menuStack_.top()->addSeparator();
}

void MenuCallback::endMenu()
{
   menuStack_.pop();
}

void MenuCallback::endMainMenu()
{
   menuBarCompleted(pMainMenu_);
}

MenuActionBinder::MenuActionBinder(QMenu* pMenu, QAction* pAction) : QObject(pAction)
{
   connect(pMenu, SIGNAL(aboutToShow()), this, SLOT(onShowMenu()));
   connect(pMenu, SIGNAL(aboutToHide()), this, SLOT(onHideMenu()));
   pAction_ = pAction;
   keySequence_ = pAction->shortcut();
   pAction->setShortcut(QKeySequence());
}

void MenuActionBinder::onShowMenu()
{
   QString commandId = pAction_->data().toString();
   manageCommand(commandId, pAction_);
   pAction_->setShortcut(keySequence_);
}

void MenuActionBinder::onHideMenu()
{
   pAction_->setShortcut(QKeySequence());
}

WindowMenu::WindowMenu(QWidget *parent) : QMenu(QString::fromUtf8("&Window"), parent)
{
   pMinimize_ = addAction(QString::fromUtf8("Minimize"));
   pMinimize_->setShortcut(QKeySequence(QString::fromUtf8("Meta+M")));
   connect(pMinimize_, SIGNAL(triggered()),
           this, SLOT(onMinimize()));

   pZoom_ = addAction(QString::fromUtf8("Zoom"));
   connect(pZoom_, SIGNAL(triggered()),
           this, SLOT(onZoom()));

   addSeparator();

   pWindowPlaceholder_ = addAction(QString::fromUtf8("__PLACEHOLDER__"));
   pWindowPlaceholder_->setVisible(false);

   addSeparator();

   pBringAllToFront_ = addAction(QString::fromUtf8("Bring All to Front"));
   connect(pBringAllToFront_, SIGNAL(triggered()),
           this, SLOT(onBringAllToFront()));

   connect(this, SIGNAL(aboutToShow()),
           this, SLOT(onAboutToShow()));
   connect(this, SIGNAL(aboutToHide()),
           this, SLOT(onAboutToHide()));
}

void WindowMenu::onMinimize()
{
   QWidget* pWin = QApplication::activeWindow();
   if (pWin)
   {
      pWin->setWindowState(Qt::WindowMinimized);
   }
}

void WindowMenu::onZoom()
{
   QWidget* pWin = QApplication::activeWindow();
   if (pWin)
   {
      pWin->setWindowState(pWin->windowState() ^ Qt::WindowMaximized);
   }
}

void WindowMenu::onBringAllToFront()
{
#ifdef Q_OS_MAC
   CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
   if (appUrlRef)
   {
      LSOpenCFURLRef(appUrlRef, NULL);
      CFRelease(appUrlRef);
   }
#endif
}

void WindowMenu::onAboutToShow()
{
   QWidget* win = QApplication::activeWindow();
   pMinimize_->setEnabled(win);
   pZoom_->setEnabled(win && win->maximumSize() != win->minimumSize());
   pBringAllToFront_->setEnabled(win);


   for (int i = windows_.size() - 1; i >= 0; i--)
   {
      QAction* pAction = windows_[i];
      removeAction(pAction);
      windows_.removeAt(i);
      pAction->deleteLater();
   }

   QWidgetList topLevels = QApplication::topLevelWidgets();
   for (int i = 0; i < topLevels.size(); i++)
   {
      QWidget* pWindow = topLevels.at(i);
      if (!pWindow->isVisible())
         continue;

      // construct with no parent (we free it manually)
      QAction* pAction = new QAction(pWindow->windowTitle(), NULL);
      pAction->setData(QVariant::fromValue(pWindow));
      pAction->setCheckable(true);
      if (pWindow->isActiveWindow())
         pAction->setChecked(true);
      insertAction(pWindowPlaceholder_, pAction);
      connect(pAction, SIGNAL(triggered()),
              this, SLOT(showWindow()));

      windows_.append(pAction);
   }
}

void WindowMenu::onAboutToHide()
{
}

void WindowMenu::showWindow()
{
   QAction* pAction = qobject_cast<QAction*>(sender());
   if (!pAction)
      return;
   QWidget* pWidget = pAction->data().value<QWidget*>();
   if (!pWidget)
      return;
   if (pWidget->isMinimized())
      pWidget->setWindowState(pWidget->windowState() & ~Qt::WindowMinimized);
   pWidget->activateWindow();
}

} // namespace desktop
} // namespace rstudio
