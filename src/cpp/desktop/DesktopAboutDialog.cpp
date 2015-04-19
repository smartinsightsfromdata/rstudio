/*
 * DesktopAboutDialog.cpp
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

#include "DesktopAboutDialog.hpp"
#include "ui_DesktopAboutDialog.h"

#include <core/FilePath.hpp>
#include <core/FileSerializer.hpp>
#include <core/system/System.hpp>

#include <QPushButton>

#include "DesktopOptions.hpp"
#include "desktop-config.h"

using namespace rstudio::core;
using namespace rstudio::desktop;

AboutDialog::AboutDialog(QWidget *parent) :
      QDialog(parent, Qt::Dialog),
      ui(new Ui::AboutDialog())
{
   ui->setupUi(this);

   ui->buttonBox->addButton(new QPushButton(QString::fromUtf8("OK")),
                            QDialogButtonBox::AcceptRole);
   ui->lblIcon->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/freedesktop/icons/64x64/rstudio.png")));
   ui->lblVersion->setText(QString::fromUtf8(
             "Version " RSTUDIO_VERSION " - © 2009-2012 RStudio, Inc."));

   setWindowModality(Qt::ApplicationModal);

   // read notice file
   FilePath supportingFilePath = options().supportingFilePath();
   FilePath noticePath = supportingFilePath.complete("NOTICE");
   std::string notice;
   Error error = readStringFromFile(noticePath, &notice);
   if (!error)
   {
      ui->textBrowser->setFontFamily(options().fixedWidthFont());
#ifdef Q_OS_MAC
      ui->textBrowser->setFontPointSize(11);
#else
      ui->textBrowser->setFontPointSize(9);
#endif
      ui->textBrowser->setText(QString::fromUtf8(notice.c_str()));
   }
}

AboutDialog::~AboutDialog()
{
   delete ui;
}
