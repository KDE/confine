/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  <copyright holder> <email>
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

#include "usermanager.h"
#include "user.h"

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <QFile>
#include <QList>
#include <QDir>
#include <QDebug>

UserManager::UserManager()
{
    XDG_CONFIG_DIRS = "XDG_CONFIG_DIRS";
    XDG_CONFIG_HOME = "XDG_CONFIG_HOME";
    getUsersOnSystem();
}

UserManager::~UserManager()
{

}

void UserManager::getUsersOnSystem()
{
    users.clear();
    //TODO:check if /etc/passwd really exists
    struct passwd* pwd_entry;
    FILE* fpwd = fopen(QFile::encodeName("/etc/passwd"), "r");

    while ((pwd_entry = fgetpwent(fpwd)) != NULL) {
        QString name = QString::fromLocal8Bit(pwd_entry->pw_name);
        QString homeDir = QString::fromLocal8Bit(pwd_entry->pw_dir);
        QString shell =  QString::fromLocal8Bit(pwd_entry->pw_shell);
        User userEntry(name, shell, homeDir);
        getXDGConfig(userEntry);
        users.append(userEntry);
    }
    fclose(fpwd);
}

void UserManager::getXDGConfig(User& user)
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
    user.setXDG_CONFIG_DIRS(configDirs);
    user.setXDG_CONFIG_HOME(configHome);
}

void UserManager::setXDGConfig(User& user, QString& configDirs, QString& configHome)
{
    if (user.getHomeDir().isEmpty() || !QFile::exists(user.getHomeDir())) {
        return;
    }

    QString filePath(user.getHomeDir() + QDir::separator() + user.getEnvironmentVariableFile());

    QString prefix;

    if (user.getEnvironmentVariableFile() == ".profile") {
        prefix = "export ";
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    QStringList fileContent;
    bool hasConfigDirs = false;
    bool hasConfigHome = false;
    while (!file.atEnd()) {
        QString line = file.readLine();
        if (!hasConfigDirs && line.contains(XDG_CONFIG_DIRS) && line.indexOf('=') > -1) {
            hasConfigDirs = true;
            line = prefix + QLatin1String("XDG_CONFIG_DIRS=") + configDirs + QLatin1Char('\n');
        } else if (!hasConfigHome && line.contains(XDG_CONFIG_HOME) && line.indexOf('=') > -1) {
            hasConfigHome = true;
            line = prefix + QLatin1String("XDG_CONFIG_HOME=") + configHome + QLatin1Char('\n');
        }
        fileContent << line;
    }

    if (!hasConfigDirs) {
        fileContent << prefix + QLatin1String("XDG_CONFIG_DIRS=") + configDirs + QLatin1Char('\n');
    }

    if (!hasConfigHome) {
        fileContent << prefix + QLatin1String("XDG_CONFIG_HOME=") + configHome + QLatin1Char('\n');
    }

    file.seek(0);
    QTextStream out(&file);
    Q_FOREACH(const QString & line, fileContent) {
        out << line;
    }
    file.flush();
    file.close();

}
