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
#include <QFile>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) : KXmlGuiWindow(parent)
{
    ui.setupUi(this);
    ui.tabWidget->setCurrentIndex(0);

    ui.userList->addItems(um.getUserNames());
    ui.profileList->addItems(um.getProfileNames());

    connect(ui.userList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(fillUserData(QListWidgetItem*)));
    connect(ui.profileList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(fillWithConfigFiles(QListWidgetItem*)));
    connect(ui.saveProfileButton, SIGNAL(released()), this, SLOT(saveProfiles()));
    connect(ui.buttonUp, SIGNAL(released()), this, SLOT(moveUp()));
    connect(ui.buttonDown, SIGNAL(released()), this, SLOT(moveDown()));
    connect(ui.editConfigFile, SIGNAL(released()), this, SLOT(displayConfigFile()));
    connect(ui.copyConfigFileButton, SIGNAL(released()), this, SLOT(copyConfigFile()));
    connect(ui.createProfileButton, SIGNAL(released()), this, SLOT(createProfile()));
}


void MainWindow::fillUserData(QListWidgetItem* userNameItem)
{
    ui.profileListForUser->clear();

    QString userName = userNameItem->data(Qt::DisplayRole).toString();
    QList<Profile> pfList = um.getProfilesfromUser(userName);
    QStringList profileNames;
    Q_FOREACH(const Profile & profile, pfList) {
        profileNames << profile.getName();
    }

    Q_FOREACH(const QString & profileName, profileNames) {
        QListWidgetItem* item = new QListWidgetItem(ui.profileListForUser);
        item->setText(profileName);
        item->setCheckState(Qt::Checked);
    }

    Q_FOREACH(const QString & profileName, um.getProfileNames()) {
        if (!profileNames.contains(profileName)) {
            QListWidgetItem* item = new QListWidgetItem(ui.profileListForUser);
            item->setText(profileName);
            item->setCheckState(Qt::Unchecked);
        }
    }

}

void MainWindow::fillWithConfigFiles(QListWidgetItem* configFileItem)
{
    QString profileName = configFileItem->text();
    Profile pf = um.getProfile(profileName);
    ui.profileFileList->clear();
    ui.profileFileList->addItems(pf.getConfigFiles());
}

void MainWindow::saveProfiles()
{
    if (ui.userList->currentItem() == 0)
        return;

    QString userName = ui.userList->currentItem()->text();
    QList<Profile> profileList;
    for (int row = 0; row < ui.profileListForUser->count(); row++) {
        QListWidgetItem* item = ui.profileListForUser->item(row);
        QString profileName = item->text();
        if (item->checkState() == Qt::Checked) {
            profileList << um.getProfile(profileName);
        }

    }
    um.setProfilesForUser(userName, profileList);
    um.saveProfilesForUser(userName);
}

void MainWindow::displayConfigFile()
{
    if (ui.profileFileList->currentItem() == 0)
        return;

    QString configFileName = ui.profileFileList->currentItem()->text();
    QString profileName = ui.profileList->currentItem()->text();
    Profile pf = um.getProfile(profileName);

    if (!configDialog) {
        configDialog = new ConfigDialog(this);
    }

    configDialog->show();
    configDialog->raise();
    configDialog->activateWindow();
    configDialog->displayConfigFile(pf.getDirectory() + configFileName);
}


void MainWindow::moveDown()
{
    int currentIndex = ui.profileListForUser->currentRow();
    QListWidgetItem* currentItem = ui.profileListForUser->item(currentIndex);
    QListWidgetItem* nextItem = ui.profileListForUser->item(currentIndex + 1);
    if (nextItem == 0)
        return;

    if (currentItem->checkState() == Qt::Checked && nextItem->checkState() == Qt::Checked) {
        currentItem = ui.profileListForUser->takeItem(currentIndex);
        ui.profileListForUser->insertItem(currentIndex + 1, currentItem);
        ui.profileListForUser->setCurrentRow(currentIndex + 1);
    }
}

void MainWindow::moveUp()
{
    int currentIndex = ui.profileListForUser->currentRow();
    QListWidgetItem* currentItem = ui.profileListForUser->item(currentIndex);
    QListWidgetItem* prevItem = ui.profileListForUser->item(currentIndex - 1);

    if (prevItem == 0)
        return;

    if (currentItem->checkState() == Qt::Checked && prevItem->checkState() == Qt::Checked) {
        currentItem = ui.profileListForUser->takeItem(currentIndex);
        ui.profileListForUser->insertItem(currentIndex - 1, currentItem);
        ui.profileListForUser->setCurrentRow(currentIndex - 1);
    }
}

void MainWindow::copyConfigFile()
{
    if (ui.profileFileList->currentItem() == 0 || ui.profileList->currentItem() == 0)
        return;

    QString configFileName = ui.profileFileList->currentItem()->text();
    QString sourceProfile = ui.profileList->currentItem()->text();
    QStringList profileNames = um.getProfileNames();
    profileNames.removeAll(sourceProfile);
    if (!copyConfigFileDialog) {
        copyConfigFileDialog = new CopyConfigFile(this);
    }
    copyConfigFileDialog->fillWithData(configFileName, profileNames);
    if (copyConfigFileDialog->exec() == QDialog::Accepted) {
        QFile::copy(sourceProfile + configFileName, copyConfigFileDialog->getSelectedProfile() + QDir::separator() + configFileName);
    }
}

void MainWindow::createProfile()
{
    if (!createProfileDialog) {
        createProfileDialog = new CreateProfile(this);
    }
    if (createProfileDialog->exec() == QDialog::Accepted) {
        QString newProfilePath = createProfileDialog->getProfilePath();
        Profile newProfile(newProfilePath);
        um.addProfile(newProfile);
        ui.profileList->clear();
        ui.profileList->addItems(um.getProfileNames());
        QDir profilePath;
        profilePath.mkpath(newProfilePath);
    }
}
