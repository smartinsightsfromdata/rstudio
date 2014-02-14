/*
 * RmdPreviewParams.java
 *
 * Copyright (C) 2009-14 by RStudio, Inc.
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
package org.rstudio.studio.client.rmarkdown.model;

import org.rstudio.core.client.Size;

import com.google.gwt.core.client.JavaScriptObject;

public class RmdPreviewParams extends JavaScriptObject
{
   protected RmdPreviewParams()
   {
   }
   
   public static native RmdPreviewParams create(RmdRenderResult result, 
                                                int scrollPosition, 
                                                String anchor) /*-{
      return {
         'result': result,
         'scroll_position': scrollPosition,
         'anchor': anchor
      };
   }-*/;
   
   public native final RmdRenderResult getResult() /*-{
      return this.result;
   }-*/;

   public native final String getOutputFile() /*-{
      return this.result.output_file;
   }-*/;
   
   public native final String getOutputUrl() /*-{
      return this.result.output_url;
   }-*/;
   
   public native final int getScrollPosition() /*-{
      return this.scroll_position;
   }-*/;
   
   public native final void setScrollPosition(int scrollPosition) /*-{
      this.scroll_position = scrollPosition;
   }-*/;
   
   public native final String getAnchor() /*-{
      return this.anchor;
   }-*/;
   
   public native final void setAnchor(String anchor) /*-{
      this.anchor = anchor;
   }-*/;
   
   public final Size getPreferredSize()
   {
      int chromeHeight = 100;
      String format = getResult().getOutputFormat();
      if (format.equals(RmdRenderResult.OUTPUT_IOSLIDES_PRESENTATION))
         return new Size(1100, 900 + chromeHeight);
      if (format.equals(RmdRenderResult.OUTPUT_REVEALJS_PRESENTATION))
         return new Size(960, 700 + chromeHeight);
      
      // default size (html_document and others)
      return new Size(960, 1000 + chromeHeight);
   }
}