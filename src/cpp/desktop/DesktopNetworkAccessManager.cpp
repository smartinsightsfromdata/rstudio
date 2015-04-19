/*
 * DesktopNetworkAccessManager.cpp
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

#include "DesktopNetworkAccessManager.hpp"

#include <core/FilePath.hpp>

#include <QTimer>

#include "DesktopNetworkReply.hpp"
#include "DesktopNetworkIOService.hpp"
#include "DesktopOptions.hpp"

using namespace rstudio::core;
using namespace rstudio::desktop;

NetworkAccessManager::NetworkAccessManager(QString secret, QObject *parent) :
    QNetworkAccessManager(parent), secret_(secret)
{
   setProxy(QNetworkProxy::NoProxy);

   QTimer* pTimer = new QTimer(this);
   connect(pTimer, SIGNAL(timeout()), SLOT(pollForIO()));
   pTimer->start(25);
}

QNetworkReply* NetworkAccessManager::createRequest(
      Operation op,
      const QNetworkRequest& req,
      QIODevice* outgoingData)
{ 
   if (req.url().scheme() == QString::fromUtf8("http") &&
       (req.url().host() == QString::fromUtf8("127.0.0.1") ||
        req.url().host() == QString::fromUtf8("localhost")) &&
        req.url().port() == options().portNumber().toInt())
   {
      return new NetworkReply(
            options().localPeer(),
            secret_,
            op,
            req,
            outgoingData,
            this);
   }
   else
   {
      return QNetworkAccessManager::createRequest(op, req, outgoingData);
   }
}


void NetworkAccessManager::pollForIO()
{
   ioServicePoll();
}
