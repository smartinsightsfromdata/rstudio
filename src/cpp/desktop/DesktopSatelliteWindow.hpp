/*
 * DesktopSatelliteWindow.hpp
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

#ifndef DESKTOP_SATELLITE_WINDOW_HPP
#define DESKTOP_SATELLITE_WINDOW_HPP

#include <QMainWindow>
#include <QtWebKit>
#include "DesktopGwtWindow.hpp"

#include "DesktopGwtCallback.hpp"

namespace rstudio {
namespace desktop {

class MainWindow;

class SatelliteWindow : public GwtWindow
{
    Q_OBJECT
public:
    SatelliteWindow(MainWindow* pMainWindow, QString name);

signals:

public slots:


protected slots:
   void onCloseWindowShortcut();
   void finishLoading(bool ok);
   void onJavaScriptWindowObjectCleared();

protected:
   virtual void closeEvent(QCloseEvent *event);

   virtual QSize printDialogMinimumSize()
   {
      return this->size();
   }

private:
   virtual void onActivated();


private:
   GwtCallback gwtCallback_;
};

} // namespace desktop
} // namespace rstudio

#endif // DESKTOP_SATELLITE_WINDOW_HPP
