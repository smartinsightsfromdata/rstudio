/*
 * SessionTcpIpHttpConnectionListener.hpp
 *
 * Copyright (C) 2009-11 by RStudio, Inc.
 *
 * This program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */

#include <boost/algorithm/string/predicate.hpp>

#include <core/Error.hpp>

#include <core/http/TcpIpSocketUtils.hpp>

#include "SessionHttpConnectionUtils.hpp"
#include "SessionHttpConnectionListenerImpl.hpp"

using namespace rstudio::core ;

namespace rstudio {
namespace session {

// implementation of local stream http connection listener
class TcpIpHttpConnectionListener :
    public HttpConnectionListenerImpl<boost::asio::ip::tcp>
{
public:
   TcpIpHttpConnectionListener(const std::string& address,
                               const std::string& port,
                               const std::string& sharedSecret)

      : address_(address), port_(port), secret_(sharedSecret)
   {
   }

protected:

   bool authenticate(boost::shared_ptr<HttpConnection> ptrConnection)
   {
      return connection::authenticate(ptrConnection, secret_);
   }

private:

   virtual Error initializeAcceptor(
      http::SocketAcceptorService<boost::asio::ip::tcp>* pAcceptor)
   {
      return http::initTcpIpAcceptor(*pAcceptor, address_, port_);
   }

   virtual bool validateConnection(
      boost::shared_ptr<HttpConnectionImpl<boost::asio::ip::tcp> > ptrConnection)
   {
      return true;
   }


   virtual Error cleanup()
   {
      return Success();
   }


private:
   std::string address_;
   std::string port_;
   std::string secret_;
};

} // namespace session
} // namespace rstudio
