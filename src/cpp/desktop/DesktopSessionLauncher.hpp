/*
 * DesktopSessionLauncher.hpp
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

#ifndef DESKTOP_SESSION_LAUNCHER_HPP
#define DESKTOP_SESSION_LAUNCHER_HPP

#include <string>

#include <boost/utility.hpp>

#include <core/Error.hpp>
#include <core/FilePath.hpp>

#include "DesktopApplicationLaunch.hpp"
#include "DesktopMainWindow.hpp"

namespace rstudio {
namespace desktop {

class SessionLauncher : public QObject
{
   Q_OBJECT
public:
   SessionLauncher(const core::FilePath& sessionPath,
                   const core::FilePath& confPath)
      : confPath_(confPath),
        sessionPath_(sessionPath),
        pAppLaunch_(NULL),
        pMainWindow_(NULL),
        pRSessionProcess_(NULL)
   {
   }

   core::Error launchFirstSession(const QString& filename,
                                  ApplicationLaunch* pAppLaunch);

   core::Error launchNextSession(bool reload);

   QString launchFailedErrorMessage() const;

   void cleanupAtExit();

public slots:
   void onRSessionExited(int exitCode, QProcess::ExitStatus exitStatus);
   void onReloadFrameForNextSession();

private:

   QString collectAbendLogMessage() const;

   void closeAllSatillites();

   core::Error launchSession(const QStringList& argList,
                             QProcess** ppRSessionProcess);

   void buildLaunchContext(QString* pHost,
                           QString* pPort,
                           QStringList* pArgList,
                           QUrl* pUrl) const;


private:
   core::FilePath confPath_;
   core::FilePath sessionPath_;
   ApplicationLaunch* pAppLaunch_;
   MainWindow* pMainWindow_;
   QProcess* pRSessionProcess_;
   QUrl nextSessionUrl_;
};

} // namespace desktop
} // namespace rstudio

#endif // DESKTOP_SESSION_LAUNCHER_HPP
