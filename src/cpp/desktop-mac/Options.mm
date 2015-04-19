/*
 * Options.mm
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


#include <core/FilePath.hpp>
#include <core/Random.hpp>
#include <core/SafeConvert.hpp>
#include <core/system/System.hpp>

#import <Foundation/NSString.h>
#import <Foundation/NSUserDefaults.h>
#import <Foundation/NSDictionary.h>

#import "Options.hpp"

#define kScreenFontSubstitutionKey @"NSFontDefaultScreenFontSubstitutionEnabled"

using namespace rstudio;
using namespace rstudio::core;

namespace rstudio {
namespace desktop {
   
Options& options()
{
   static Options instance;
   return instance;
}
   
Options::Options()
   : runDiagnostics_(false)
{
   // initialize user defaults
   NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
   NSDictionary* defs = [NSDictionary dictionaryWithObjectsAndKeys:
                             @"Lucida Grande", @"font.proportional",
                             @"Monaco", @"font.fixedwidth",
                             @"0", @"view.maczoomlevel",
                             [NSArray array], @"updates.ignored",
                             nil];
   
   [prefs registerDefaults:defs];

   // If no default has been established for the screen font substitution,
   // enable it. This allows characters in monospaced fonts to be coerced to
   // integer widths, which corrects some problems with ACE cursor positioning
   // when CJK characters are present.
   NSData* fontSub = [prefs dataForKey: kScreenFontSubstitutionKey];
   if (fontSub == nil)
   {
      [prefs setBool:YES forKey: kScreenFontSubstitutionKey];
   }
}
   
void Options::initFromCommandLine(NSArray* arguments)
{
   for (NSString* arg in arguments)
   {
      if ([arg isEqualToString: @"--run-diagnostics"])
         runDiagnostics_ = true;
   }
}

std::string Options::portNumber() const
{
   // lookup / generate on demand
   if (portNumber_.empty())
   {
      // Use a random-ish port number to avoid collisions between different
      // instances of rdesktop-launched rsessions
      int base = std::abs(random::uniformRandomInteger<int>());
      int port = (base % 40000) + 8080;
      portNumber_ = safe_convert::numberToString(port);
   }
   return portNumber_;
}
   
std::string Options::newPortNumber()
{
   portNumber_.clear();
   return portNumber();
}

std::string Options::proportionalFont() const
{
   NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
   return [[prefs stringForKey: @"font.proportional"] UTF8String];
}

std::string Options::fixedWidthFont() const
{
   NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
   return [[prefs stringForKey: @"font.fixedwidth"] UTF8String];
}

void Options::setFixedWidthFont(std::string font)
{
   NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
   [prefs setObject: [NSString stringWithUTF8String: font.c_str()]
             forKey: @"font.fixedwidth"];
}

int Options::zoomLevel() const
{
   NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
   NSString* zoomLevel = [prefs stringForKey: @"view.maczoomlevel"];
   return [zoomLevel intValue];
}
   
void Options::setZoomLevel(int zoomLevel)
{
   // enforce min and max
   if (zoomLevel > 6)
      zoomLevel = 6;
   else if (zoomLevel < -3)
      zoomLevel = -3;
   
   NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
   NSNumber* zoom = [NSNumber numberWithInt: zoomLevel];
   [prefs setObject: [zoom stringValue] forKey: @"view.maczoomlevel"];
}

FilePath Options::scriptsPath() const
{
   return scriptsPath_;
}
   
void Options::setScriptsPath(const FilePath& scriptsPath)
{
   scriptsPath_ = scriptsPath;
}

core::FilePath Options::executablePath() const
{
   if (executablePath_.empty())
   {
      Error error = core::system::executablePath(NULL, &executablePath_);
      if (error)
         LOG_ERROR(error);
   }
   return executablePath_;
}
   
FilePath Options::supportingFilePath() const
{
   if (supportingFilePath_.empty())
   {
      // default to install path
      core::system::installPath("..", NULL, &supportingFilePath_);
      
      // adapt for OSX resource bundles
      if (supportingFilePath_.complete("Info.plist").exists())
         supportingFilePath_ = supportingFilePath_.complete("Resources");
   }
   return supportingFilePath_;
}

FilePath Options::wwwDocsPath() const
{
   FilePath supportingPath = desktop::options().supportingFilePath();
   FilePath wwwDocsPath = supportingPath.complete("www/docs");
   if (!wwwDocsPath.exists())
      wwwDocsPath = supportingPath.complete("../../../../../../gwt/www/docs");
   return wwwDocsPath;
}

std::vector<std::string> Options::ignoredUpdateVersions() const
{
   NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
   NSArray* ignored = [prefs objectForKey: @"updates.ignored"];
   std::vector<std::string> ignoredUpdates;
   for (NSString* ver in ignored)
      ignoredUpdates.push_back([ver UTF8String]);
   return ignoredUpdates;
}
   
void Options::setIgnoredUpdateVersions(const std::vector<std::string>& ignored)
{
   NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
   NSMutableArray* ign = [NSMutableArray array];
   for (size_t i = 0; i<ignored.size(); i++)
      [ign addObject: [NSString stringWithUTF8String: ignored[i].c_str()]];
   [prefs setObject: ign forKey: @"updates.ignored"];
}


} // namespace desktop
} // namespace rstudio

