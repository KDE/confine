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

#include "userprofilemanager.h"
#include "user.h"
#include "profile.h"
#include "confineconfiguration.h"

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <QFile>
#include <QList>
#include <QDir>
#include <QApplication>


UserProfileManager::UserProfileManager()
{
    XDG_CONFIG_DIRS = "XDG_CONFIG_DIRS";
    XDG_CONFIG_HOME = "XDG_CONFIG_HOME";
    getUsersOnSystem();
}

UserProfileManager::~UserProfileManager()
{

}

void UserProfileManager::getUsersOnSystem()
{
    users.clear();
    ConfineConfiguration* cf = qApp->property("confineConfiguration").value<ConfineConfiguration*>();

    if (!QFile::exists(cf->getPasswdPath())) {
        return;
    }

    struct passwd* pwd_entry;
    FILE* fpwd = fopen(QFile::encodeName(cf->getPasswdPath()), "r");

    while ((pwd_entry = fgetpwent(fpwd)) != NULL) {
        QString name = QString::fromLocal8Bit(pwd_entry->pw_name);
        QString homeDir = QString::fromLocal8Bit(pwd_entry->pw_dir);
        QString shell =  QString::fromLocal8Bit(pwd_entry->pw_shell);
        User userEntry(name, shell, homeDir);
        getXDGConfig(userEntry);
        users.insert(userEntry.getName(), userEntry);
    }
    fclose(fpwd);
}

void UserProfileManager::getXDGConfig(User& user)
{
    if (user.getHomeDir().isEmpty() || !QFile::exists(user.getHomeDir())) {
        return;
    }

    QString filePath(user.getHomeDir() + QDir::separator() + user.getEnvironmentVariableFile());

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QString configDirs;
    QString configHome;
    while (!file.atEnd()) {
        QString line = file.readLine();

        if (line.isEmpty())
            continue;

        if (line.contains(XDG_CONFIG_DIRS) && line.indexOf('=') > -1) {
            configDirs = line.mid(line.indexOf('=') + 1).trimmed();
        } else if (line.contains(XDG_CONFIG_HOME) && line.indexOf('=') > -1) {
            configHome = line.mid(line.indexOf('=') + 1).trimmed();
        }
    }

    QStringList configDirsList = configDirs.split(QLatin1Char(':'));
    Q_FOREACH(const QString & singleDir, configDirsList) {
        if (singleDir.isEmpty()) {
            continue;
        }

        QString dir(singleDir);
        if (!dir.endsWith(QLatin1Char('/'))) {
            dir += QLatin1Char('/');
        }

        if (!profiles.contains(dir)) {
            Profile pf(dir);
            profiles.insert(dir, pf);
        }
        user.addProfile(profiles.value(dir));
    }

    user.setXDG_CONFIG_HOME(configHome);
}

QStringList UserProfileManager::getUserNames()
{
    QStringList userNames;
    Q_FOREACH(const User & user, users.values()) {
        userNames << user.getName();
    }
    return userNames;
}

QStringList UserProfileManager::getProfileNames()
{
    QStringList profileNames;
    Q_FOREACH(const Profile & profile, profiles.values()) {
        profileNames << profile.getName();
    }

    return profileNames;
}

QList< Profile > UserProfileManager::getProfilesfromUser(const QString& userName)
{
    User user = users.value(userName);
    return user.getProfiles();
}

void UserProfileManager::setProfilesForUser(const QString& userName, QList< Profile > profileList)
{
    User user = users.value(userName);
    user.clearProfiles();
    Q_FOREACH(const Profile & profile, profileList) {
        user.addProfile(profile);
        if (!profiles.contains(profile.getName())) {
            profiles.insert(profile.getName(), profile);
        }
    }
    users.insert(user.getName(), user);
}

Profile UserProfileManager::getProfile(const QString& profileName)
{
    return profiles.value(profileName);
}
