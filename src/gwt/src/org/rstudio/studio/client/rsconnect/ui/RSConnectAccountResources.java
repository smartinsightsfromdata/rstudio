/*
 * RSConnectAccountResources.java
 *
 * Copyright (C) 2009-15 by RStudio, Inc.
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
package org.rstudio.studio.client.rsconnect.ui;

import com.google.gwt.core.client.GWT;
import com.google.gwt.resources.client.ClientBundle;
import com.google.gwt.resources.client.ImageResource;

public interface RSConnectAccountResources extends ClientBundle
{
   static RSConnectAccountResources INSTANCE = 
                  (RSConnectAccountResources)GWT.create(RSConnectAccountResources.class);
   
   @Source("localAccountIconSmall.png")
   ImageResource localAccountIconSmall();

   @Source("localAccountIcon.png")
   ImageResource localAccountIcon();

   @Source("localAccountIconLarge.png")
   ImageResource localAccountIconLarge();

   @Source("cloudAccountIconSmall.png")
   ImageResource cloudAccountIconSmall();

   @Source("cloudAccountIcon.png")
   ImageResource cloudAccountIcon();

   @Source("cloudAccountIconLarge.png")
   ImageResource cloudAccountIconLarge();
   
   @Source("publishIcon.png")
   ImageResource publishIcon();

   @Source("publishIconLarge.png")
   ImageResource publishIconLarge();
}