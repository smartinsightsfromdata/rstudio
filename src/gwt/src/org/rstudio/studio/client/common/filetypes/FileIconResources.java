/*
 * FileIconResources.java
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
package org.rstudio.studio.client.common.filetypes;

import com.google.gwt.core.client.GWT;
import com.google.gwt.resources.client.ClientBundle;
import com.google.gwt.resources.client.ImageResource;

public interface FileIconResources extends ClientBundle
{
   public static final FileIconResources INSTANCE =
                                           GWT.create(FileIconResources.class);

   ImageResource iconCsv();
   ImageResource iconFolder();
   ImageResource iconPublicFolder();
   ImageResource iconUpFolder();
   ImageResource iconPdf();
   ImageResource iconPng();
   ImageResource iconRdata();
   ImageResource iconRproject();
   ImageResource iconRdoc();
   ImageResource iconRhistory();
   ImageResource iconRprofile();
   ImageResource iconTex();
   ImageResource iconText();
   ImageResource iconPython();
   ImageResource iconSql();
   ImageResource iconSh();
   ImageResource iconYaml();
   ImageResource iconXml();
   ImageResource iconMarkdown();
   ImageResource iconMermaid();
   ImageResource iconGraphviz();
   ImageResource iconH();
   ImageResource iconC();
   ImageResource iconHpp();
   ImageResource iconCpp();
   ImageResource iconHTML();
   ImageResource iconCss();
   ImageResource iconJavascript();
   ImageResource iconRsweave();
   ImageResource iconRd();
   ImageResource iconRhtml();
   ImageResource iconRmarkdown();
   ImageResource iconRpresentation();
   ImageResource iconSourceViewer();
   ImageResource iconProfiler();
   ImageResource iconWord();
   
   // Ace modes
   ImageResource iconClojure();
   ImageResource iconCoffee();
   ImageResource iconCsharp();
   ImageResource iconGitignore();
   ImageResource iconGo();
   ImageResource iconGroovy();
   ImageResource iconHaskell();
   ImageResource iconHaxe();
   ImageResource iconJava();
   ImageResource iconJulia();
   ImageResource iconLisp();
   ImageResource iconLua();
   ImageResource iconMatlab();
   ImageResource iconPerl();
   ImageResource iconRuby();
   ImageResource iconRust();
   ImageResource iconScala();
   ImageResource iconSnippets();
   
   ImageResource iconStan();
   
   
}
