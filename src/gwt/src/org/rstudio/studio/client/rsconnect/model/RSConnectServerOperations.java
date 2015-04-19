/*
 * RSConnectServerOperations.java
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
package org.rstudio.studio.client.rsconnect.model;

import java.util.ArrayList;

import org.rstudio.studio.client.server.ServerRequestCallback;
import org.rstudio.studio.client.server.Void;

import com.google.gwt.core.client.JsArray;

public interface RSConnectServerOperations
{
   void removeRSConnectAccount(String accountName, String server,
               ServerRequestCallback<Void> requestCallback);

   void getRSConnectAccountList(
               ServerRequestCallback<JsArray<RSConnectAccount>> requestCallback);

   void connectRSConnectAccount(String command, 
               ServerRequestCallback<Void> requestCallback);

   void getRSConnectAppList(String accountName, String server,
               ServerRequestCallback<JsArray<RSConnectApplicationInfo>> requestCallback);
   
   void getRSConnectDeployments(String target, 
               ServerRequestCallback<JsArray<RSConnectDeploymentRecord>> requestCallback); 
   
   void getDeploymentFiles (String target, 
               ServerRequestCallback<RSConnectDeploymentFiles> requestCallback);
   
   void deployShinyApp(String dir, ArrayList<String> deployFiles, String file, 
               String account, String server, String appName, 
               ServerRequestCallback<Boolean> requestCallback);

   void validateServerUrl (String url, 
               ServerRequestCallback<RSConnectServerInfo> requestCallback);
   
   void getPreAuthToken(String serverName, 
               ServerRequestCallback<RSConnectPreAuthToken> requestCallback);
   
   void getUserFromToken(String url, RSConnectPreAuthToken token,
               ServerRequestCallback<RSConnectAuthUser> requestCallback);
   
   void registerUserToken(String serverName, String accountName, int userId, 
                RSConnectPreAuthToken token, 
                ServerRequestCallback<Void> requestCallback);
   
   void getLintResults(String target,
                ServerRequestCallback<RSConnectLintResults> resultCallback);
}
