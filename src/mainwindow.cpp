/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  Gilbert Assaf <gassaf@gmx.de>
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

#include "mainwindow.h"
#include "userprofilemanager.h"

#include <KLocalizedString>


MainWindow::MainWindow(QWidget* parent) : KXmlGuiWindow(parent)
{
    ui.setupUi(this);
    ui.tabWidget->setCurrentIndex(0);

    ui.userList->addItems(um.getUserNames());
    ui.profileList->addItems(um.getProfileNames());

    Q_FOREACH(const QString & profileName, um.getProfileNames()) {
        QListWidgetItem* item = new QListWidgetItem(ui.profileListForUser);
        item->setData(Qt::DisplayRole, profileName);
        item->setData(Qt::CheckStateRole, Qt::Unchecked);
    }

    connect(ui.userList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(fillUserData(QListWidgetItem*)));

}


void MainWindow::fillUserData(QListWidgetItem* userNameItem)
{
    QString userName = userNameItem->data(Qt::DisplayRole).toString();
    QList<Profile> pfList = um.getProfilesfromUser(userName);
    QStringList profileNames;
    Q_FOREACH(const Profile & profile, pfList) {
        profileNames << profile.getName();
    }
    for (int row = 0; row < ui.profileListForUser->count(); row++) {
        QListWidgetItem* item = ui.profileListForUser->item(row);
        QString profileName = item->data(Qt::DisplayRole).toString();
        if (profileNames.contains(profileName)) {
            item->setData(Qt::CheckStateRole, Qt::Checked);
        } else {
            item->setData(Qt::CheckStateRole, Qt::Unchecked);
        }

    }
}

