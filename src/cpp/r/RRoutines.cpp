/*
 * RRoutines.cpp
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

#include <r/RRoutines.hpp>
#include <r/RExec.hpp>

#include <algorithm>

namespace rstudio {
namespace r {
namespace routines {
 
namespace { 
   std::vector<R_CMethodDef> s_cMethods;
   std::vector<R_CallMethodDef> s_callMethods; 
}
   
void addCMethod(const R_CMethodDef method)
{
   s_cMethods.push_back(method);
}
   
void addCallMethod(const R_CallMethodDef method)
{
   s_callMethods.push_back(method);
}

void registerCallMethod(const char* name,
                        DL_FUNC fun,
                        int numArgs)
{
   R_CallMethodDef callMethodDef;
   callMethodDef.name = name;
   callMethodDef.fun = fun;
   callMethodDef.numArgs = numArgs;
   addCallMethod(callMethodDef);
}

void registerAll()
{
   // c methods
   R_CMethodDef* pCMethods = NULL;
   if (s_cMethods.size() > 0)
   {
      R_CMethodDef nullMethodDef ;
      nullMethodDef.name = NULL ;
      nullMethodDef.fun = NULL ;
      nullMethodDef.numArgs = 0 ;
      nullMethodDef.types = NULL;
      nullMethodDef.styles = NULL;
      s_cMethods.push_back(nullMethodDef);
      pCMethods = &s_cMethods[0];
   }
   
   // call methods
   R_CallMethodDef* pCallMethods = NULL;
   if (s_callMethods.size() > 0)
   {
      R_CallMethodDef nullMethodDef ;
      nullMethodDef.name = NULL ;
      nullMethodDef.fun = NULL ;
      nullMethodDef.numArgs = 0 ;
      s_callMethods.push_back(nullMethodDef);
      pCallMethods = &s_callMethods[0];
   }
   
   DllInfo *info = R_getEmbeddingDllInfo() ;
   R_registerRoutines(info, pCMethods, pCallMethods, NULL, NULL);
   
   exec::RFunction registerNativeRoutines(".rs.registerNativeRoutines");
   core::Error error = registerNativeRoutines.call();
   if (error)
      LOG_ERROR(error);
}
   
   
} // namespace routines   
} // namespace r
} // namespace rstudio



