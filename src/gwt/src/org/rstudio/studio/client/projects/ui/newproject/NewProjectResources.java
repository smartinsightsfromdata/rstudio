/*
 * NewProjectResources.java
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
package org.rstudio.studio.client.projects.ui.newproject;



import com.google.gwt.core.client.GWT;
import com.google.gwt.resources.client.ClientBundle;
import com.google.gwt.resources.client.CssResource;
import com.google.gwt.resources.client.ImageResource;


public interface NewProjectResources extends ClientBundle
{
   static NewProjectResources INSTANCE = 
                  (NewProjectResources)GWT.create(NewProjectResources.class);
   
   
   ImageResource newProjectDirectoryIcon();
   ImageResource newProjectDirectoryIconLarge();
   ImageResource packageIcon();
   ImageResource packageIconLarge();
   ImageResource shinyAppIcon();
   ImageResource shinyAppIconLarge();
   ImageResource existingDirectoryIcon();
   ImageResource existingDirectoryIconLarge();
   ImageResource projectFromRepositoryIcon();
   ImageResource projectFromRepositoryIconLarge();
   
   ImageResource gitIcon();
   ImageResource gitIconLarge();
   ImageResource svnIcon();
   ImageResource svnIconLarge();
   
   static interface Styles extends CssResource
   {
      String wizardWidget();
      String wizardMainColumn();
      String wizardTextEntryLabel();
      String wizardSpacer();
      String vcsSelectorDesktop();
      String wizardCheckbox();
      String vcsNotInstalledWidget();
      String vcsHelpLink();
      String newProjectDirectoryName();
      String codeFilesListButton();
      String codeFilesListBox();
      String invalidPkgName();
   }
   
   @Source("NewProjectWizard.css")
   Styles styles();
}
