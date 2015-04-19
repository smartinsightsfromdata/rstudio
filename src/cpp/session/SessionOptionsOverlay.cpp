/*
 * SessionOptionsOverlay.cpp
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

#include <session/SessionOptions.hpp>

using namespace rstudio::core ;

namespace rstudio {
namespace session {

void Options::addOverlayOptions(
                           boost::program_options::options_description* pOpt)
{
}

bool Options::validateOverlayOptions(std::string* pErrMsg)
{
   return true;
}

void Options::resolveOverlayOptions()
{
}

bool Options::allowOverlay() const
{
   return true;
}

} // namespace session
} // namespace rstudio
