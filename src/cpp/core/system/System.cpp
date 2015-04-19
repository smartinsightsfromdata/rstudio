/*
 * System.cpp
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

#include <core/system/System.hpp>

#include <core/Hash.hpp>
#include <core/Log.hpp>
#include <core/FilePath.hpp>

#include <core/system/Environment.hpp>

namespace rstudio {
namespace core {
namespace system {
     
#ifdef _WIN32
#define kPathSeparator ";"
#else
#define kPathSeparator ":"
#endif


bool realPathsEqual(const FilePath& a, const FilePath& b)
{
   FilePath aReal, bReal;

   Error error = realPath(a, &aReal);
   if (error)
   {
      LOG_ERROR(error);
      return false;
   }

   error = realPath(b, &bReal);
   if (error)
   {
      LOG_ERROR(error);
      return false;
   }

   return aReal == bReal;
}

void addToSystemPath(const FilePath& path, bool prepend)
{
   std::string systemPath = system::getenv("PATH");
   if (prepend)
      systemPath = path.absolutePath() + kPathSeparator + systemPath;
   else
      systemPath = systemPath + kPathSeparator + path.absolutePath();
   system::setenv("PATH", systemPath);
}


int exitFailure(const Error& error, const ErrorLocation& loggedFromLocation)
{
   core::log::logError(error, loggedFromLocation);
   return EXIT_FAILURE;
}

int exitFailure(const std::string& errMsg,
                const ErrorLocation& loggedFromLocation)
{
   core::log::logErrorMessage(errMsg, loggedFromLocation);
   return EXIT_FAILURE;
}
   
int exitSuccess()
{
   return EXIT_SUCCESS;
}

std::string generateShortenedUuid()
{
   std::string uuid = core::system::generateUuid(false);
   return core::hash::crc32HexHash(uuid);
}


} // namespace system
} // namespace core
} // namespace rstudio

