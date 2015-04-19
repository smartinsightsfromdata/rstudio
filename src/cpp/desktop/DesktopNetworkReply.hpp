/*
 * DesktopNetworkReply.hpp
 *
 * Copyright (C) 2009-12 by RStudio, Inc.
 *
 * This program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */

#ifndef DESKTOPNETWORKREPLY_HPP
#define DESKTOPNETWORKREPLY_HPP

#include <boost/scoped_ptr.hpp>

#include <core/FilePath.hpp>

#include <QNetworkReply>

namespace rstudio {
namespace core {
   class Error;
   class FilePath;
   namespace http {
      class Request;
      class Response;
   }
}
}

namespace rstudio {
namespace desktop {

class NetworkReply : public QNetworkReply
{   
   Q_OBJECT
public:
   NetworkReply(const std::string& localPeer,
                const QString& secret,
                QNetworkAccessManager::Operation op,
                const QNetworkRequest& req,
                QIODevice* outgoingData,
                QObject *parent = 0);
   virtual ~NetworkReply();

signals:
   
public slots:

public:

   qint64 bytesAvailable() const;
   bool isSequential() const;
   void abort();

protected:
   qint64 readData(char *data, qint64 maxSize);

private:
   void onResponse(const core::http::Response& response);
   void onError(const core::Error& error);

   void executeRequest(const core::http::Request& request);

private:
   struct Impl;
   boost::scoped_ptr<Impl> pImpl_;
   std::string localPeer_;
   QString secret_;
};

} // namespace desktop
} // namespace rstudio

#endif // DESKTOPNETWORKREPLY_HPP
