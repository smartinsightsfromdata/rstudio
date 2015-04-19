/*
 * SessionSourceDatabase.hpp
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

#ifndef SESSION_SOURCE_DATABASE_HPP
#define SESSION_SOURCE_DATABASE_HPP

#include <string>
#include <vector>

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals.hpp>

#include <core/FilePath.hpp>
#include <core/json/Json.hpp>

namespace rstudio {
namespace core {
   class Error;
   class FilePath;
}
}
 
namespace rstudio {
namespace session {
namespace source_database {
   
class SourceDocument : boost::noncopyable
{
public:
   SourceDocument(const std::string& type = std::string());
   virtual ~SourceDocument() {}
   // COPYING: via compiler

   // accessors
   const std::string& id() const { return id_; }
   const std::string& path() const { return path_; }
   const std::string& type() const { return type_; }
   const std::string& contents() const { return contents_; }
   const std::string& hash() const { return hash_; }
   const std::string& encoding() const { return encoding_; }
   bool dirty() const { return dirty_; }
   double created() const { return created_; }
   bool sourceOnSave() const { return sourceOnSave_; }
   int relativeOrder() const { return relativeOrder_; } 
   const core::json::Object& properties() const { return properties_; }
   const std::string& folds() const { return folds_; }
   std::string getProperty(const std::string& name) const;

   // is this an untitled document?
   bool isUntitled() const;

   // set contents from string
   void setContents(const std::string& contents);

   // set contents from file
   core::Error setPathAndContents(const std::string& path,
                                  bool allowSubstChars = true);

   core::Error updateDirty();

   // set dirty
   void setDirty(bool dirty)
   {
      dirty_ = dirty;
   }

   // set source on save
   void setSourceOnSave(bool sourceOnSave)
   {
      sourceOnSave_ = sourceOnSave;
   }

   void setEncoding(const std::string& encoding)
   {
      encoding_ = encoding;
   }

   void setFolds(const std::string& folds)
   {
      folds_ = folds;
   }

   void setRelativeOrder(int order) 
   {
      relativeOrder_ = order;
   }

   void checkForExternalEdit(std::time_t* pTime);

   void updateLastKnownWriteTime();

   // applies the values in the given properties object to the document's property
   // bag. this does NOT replace all of the doc's properties on the server; any
   // properties that already exist but are not present in the given object are
   // left unchanged. if an entry in the given object has a null value, that
   // property should be removed.
   void editProperties(core::json::Object& properties);

   void setType(const std::string& type)
   {
      type_ = type;
   }
   
   // is this an R, or potentially R-containing, source file?
   // TODO: Export these types as an 'enum' and provide converters.
   bool canContainRCode()
   {
      return type_.size() > 0 && (
               type_ == SourceDocumentTypeSweave ||
               type_ == SourceDocumentTypeRSource ||
               type_ == SourceDocumentTypeRMarkdown ||
               type_ == SourceDocumentTypeRHTML ||
               type_ == SourceDocumentTypeCpp);
   }

   core::Error readFromJson(core::json::Object* pDocJson);
   void writeToJson(core::json::Object* pDocJson) const;

   core::Error writeToFile(const core::FilePath& filePath) const;

private:
   void editProperty(const core::json::Object::value_type& property);

private:
   std::string id_;
   std::string path_;
   std::string type_;
   std::string contents_;
   std::string hash_;
   std::string encoding_;
   std::string folds_;
   std::time_t lastKnownWriteTime_;
   bool dirty_;
   double created_;
   bool sourceOnSave_;
   int relativeOrder_;
   core::json::Object properties_;
   
public:
   
   static const char * const SourceDocumentTypeSweave;
   static const char * const SourceDocumentTypeRSource;
   static const char * const SourceDocumentTypeRMarkdown;
   static const char * const SourceDocumentTypeRHTML;
   static const char * const SourceDocumentTypeCpp;
   
};

bool sortByCreated(const boost::shared_ptr<SourceDocument>& pDoc1,
                   const boost::shared_ptr<SourceDocument>& pDoc2);
bool sortByRelativeOrder(const boost::shared_ptr<SourceDocument>& pDoc1,
                         const boost::shared_ptr<SourceDocument>& pDoc2);


core::FilePath path();
core::Error get(const std::string& id, boost::shared_ptr<SourceDocument> pDoc);
core::Error getDurableProperties(const std::string& path,
                                 core::json::Object* pProperties);
core::Error list(std::vector<boost::shared_ptr<SourceDocument> >* pDocs);
core::Error put(boost::shared_ptr<SourceDocument> pDoc);
core::Error remove(const std::string& id);
core::Error removeAll();

// source database events
struct Events : boost::noncopyable
{
   boost::signal<void(boost::shared_ptr<SourceDocument>)> onDocUpdated;
   boost::signal<void(const std::string&)>                onDocRemoved;
   boost::signal<void()>                                  onRemoveAll;
};

Events& events();

core::Error initialize();

} // namespace source_database
} // namesapce session
} // namespace rstudio

#endif // SESSION_SOURCE_DATABASE_HPP
