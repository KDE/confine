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

#include "settingsdialog.h"
#include "confineconfiguration.h"

#include <KConfig>
#include <KConfigGroup>
#include <ksharedconfig.h>
#include <kconfigbase.h>

SettingsDialog::SettingsDialog(UserProfileManager& um, QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.cancelButton, SIGNAL(released()), this, SLOT(close()));
    connect(ui.applyButton, SIGNAL(released()), this, SLOT(save()));
    connect(ui.okButton, SIGNAL(released()), this, SLOT(saveAndClose()));

    QStringList stdProfiles = um.getStandardProfiles();
    Q_FOREACH(const QString & profileName, stdProfiles) {
        QListWidgetItem* item = new QListWidgetItem(ui.profileList);
        item->setText(profileName);
        item->setCheckState(Qt::Checked);
    }
    QStringList profileNames = um.getProfileNames();
    QSet<QString> profileSet = profileNames.toSet();
    profileSet.subtract(stdProfiles.toSet());

    Q_FOREACH(const QString & profileName, profileSet) {
        QListWidgetItem* item = new QListWidgetItem(ui.profileList);
        item->setText(profileName);
        item->setCheckState(Qt::Unchecked);
    }

}

void SettingsDialog::save()
{
    QStringList profileList;
    for (int row = 0; row < ui.profileList->count(); row++) {
        QListWidgetItem* item = ui.profileList->item(row);
        QString profileName = item->text();
        if (item->checkState() == Qt::Checked) {
            profileList << profileName;
        }

    }
    QString xdgContent = profileList.join(QLatin1Char(':'));

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup grp(config, "Global");

    grp.writeEntry("XDG_CONFIG_DIRS_DEFAULT", xdgContent);
    grp.sync();
    config->sync();
}

void SettingsDialog::saveAndClose()
{
    save();
    close();
}

