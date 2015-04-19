/*
 * DesktopGwtWindow.cpp
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

#include "DesktopGwtWindow.hpp"



namespace rstudio {
namespace desktop {

GwtWindow::GwtWindow(bool showToolbar,
                     bool adjustTitle,
                     QString name,
                     QUrl baseUrl,
                     QWidget* pParent) :
   BrowserWindow(showToolbar, adjustTitle, name, baseUrl, pParent)
{
}

bool GwtWindow::event(QEvent* pEvent)
{
   if (pEvent->type() == QEvent::WindowActivate)
      onActivated();

   return BrowserWindow::event(pEvent);
}


} // namespace desktop
} // namespace rstudio
