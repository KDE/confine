/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2016  Gilbert Assaf <gassaf@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "configdialog.h"
#include <QDebug>

ConfigDialog::ConfigDialog(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.groupList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(displayConfigEntries(QListWidgetItem*)));
    connect(ui.closeButton, SIGNAL(released()), this, SLOT(close()));
    connect(ui.saveButton, SIGNAL(released()), this, SLOT(save()));
    
    ui.entryWidget->verticalHeader()->setVisible(false);
}

void ConfigDialog::displayConfigFile(const QString& path)
{
    config = KSharedConfig::openConfig(path);

    Q_FOREACH(const QString & kConfigGrpName, config->groupList()) {
        QListWidgetItem* item = new QListWidgetItem(ui.groupList);
        item->setText(kConfigGrpName);
    }
}

void ConfigDialog::displayConfigEntries(QListWidgetItem* configGroupItem)
{
    int row = 0;
    KConfigGroup grp(config, configGroupItem->text());
    QMapIterator<QString, QString> i(grp.entryMap());
    ui.entryWidget->clearContents();
    ui.entryWidget->setRowCount(grp.entryMap().size());
    while (i.hasNext()) {
        i.next();
        QTableWidgetItem* entrieName = new QTableWidgetItem(i.key());
        entrieName->setFlags(Qt::ItemIsSelectable);
        ui.entryWidget->setItem(row, 0, entrieName);
        ui.entryWidget->setItem(row, 1, new QTableWidgetItem(i.value()));
        ui.entryWidget->setItem(row, 2, new QTableWidgetItem(im.getInfo(grp, i.key())));
        row++;
    }
}

void ConfigDialog::save()
{
  QListWidgetItem* configGroupItem = ui.groupList->currentItem();
  
  KConfigGroup grp(config, configGroupItem->text());
  
  for(int i =0;i<ui.entryWidget->rowCount();i++){
   QTableWidgetItem* keyItem = ui.entryWidget->item(i, 0);
   QTableWidgetItem* valueItem = ui.entryWidget->item(i, 1);
   grp.writeEntry(keyItem->text(), valueItem->text());
  }
  grp.sync();
  config->sync();
}


