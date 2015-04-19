/*
 * SourceMarkerListResources.java
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
package org.rstudio.studio.client.common.sourcemarkers;


import com.google.gwt.core.client.GWT;
import com.google.gwt.resources.client.ClientBundle;
import com.google.gwt.resources.client.CssResource;
import com.google.gwt.resources.client.ImageResource;

public interface SourceMarkerListResources extends ClientBundle
{  
   public static interface Styles extends CssResource
   {
      String table();
      String headerRow();
      String selectedRow();
      String iconCell();
      String errorIcon();
      String warningIcon();
      String boxIcon();
      String infoIcon();
      String styleIcon();
      String noIcon();
      String lineCell();
      String messageCell();
      String disclosure();
   }

   @Source("images/error.png")
   ImageResource error();
  
   @Source("org/rstudio/core/client/theme/res/warningSmall.png")
   ImageResource warning();

   @Source("images/badbox.png")
   ImageResource badbox();
   
   @Source("images/info.png")
   ImageResource info();
   
   @Source("images/style.png")
   ImageResource style();

   @Source("SourceMarkerList.css")
   Styles styles();
   
   @Source("images/logContextButton.png")
   ImageResource logContextButton();
    
   public static SourceMarkerListResources INSTANCE = 
      (SourceMarkerListResources)GWT.create(SourceMarkerListResources.class) ;
  
}