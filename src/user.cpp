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

#include "user.h"
#include "profile.h"

#include <QString>
#include <QFile>
#include <QList>
#include <QDir>
#include <QTextStream>

User::User(QString& userName, QString& userShell, QString& userHomeDir) : name(userName),
    shell(userShell),
    homeDir(userHomeDir),
    environmentVariableFile(QLatin1String(".profile"))

{


}

User::~User()
{

}

QString User::getName()
{
    return name;
}

QString User::getHomeDir()
{
    return homeDir;
}

QString User::getXDG_CONFIG_HOME()
{
    return XDG_CONFIG_HOME;
}

QString User::getEnvironmentVariableFile()
{
    return environmentVariableFile;
}

void User::setXDG_CONFIG_HOME(QString& configHome)
{
    XDG_CONFIG_HOME = configHome;
}

void User::setEnvironmentVariableFile(QString& varFile)
{
    environmentVariableFile = varFile;
}

void User::addProfile(const Profile& profile)
{
  profiles.append(profile);
}

void User::save()
{
    if (homeDir.isEmpty() || !QFile::exists(homeDir)) {
        return;
    }

    QString filePath(homeDir + QDir::separator() + environmentVariableFile);

    QString prefix;

    if (environmentVariableFile == ".profile") {
        prefix = "export ";
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }
    
    QString configDirs;
    QStringList helperList;
    Q_FOREACH(const Profile & pf, profiles) {
      helperList << pf.getDirectory();
    }
    
    configDirs = helperList.join(QLatin1Char(':'));
    
    QStringList fileContent;
    bool hasConfigDirs = false;
    bool hasConfigHome = false;
    while (!file.atEnd()) {
        QString line = file.readLine();
        if (!hasConfigDirs && line.contains(QLatin1String("XDG_CONFIG_DIRS")) && line.indexOf('=') > -1) {
            hasConfigDirs = true;
            line = prefix + QLatin1String("XDG_CONFIG_DIRS=") + configDirs + QLatin1Char('\n');
        } else if (!hasConfigHome && line.contains(XDG_CONFIG_HOME) && line.indexOf('=') > -1) {
            hasConfigHome = true;
            line = prefix + QLatin1String("XDG_CONFIG_HOME=") + XDG_CONFIG_HOME + QLatin1Char('\n');
        }
        fileContent << line;
    }

    if (!hasConfigDirs) {
        fileContent << prefix + QLatin1String("XDG_CONFIG_DIRS=") + configDirs + QLatin1Char('\n');
    }

    if (!hasConfigHome) {
        fileContent << prefix + QLatin1String("XDG_CONFIG_HOME=") + XDG_CONFIG_HOME + QLatin1Char('\n');
    }

    file.seek(0);
    QTextStream out(&file);
    Q_FOREACH(const QString & line, fileContent) {
        out << line;
    }
    file.flush();
    file.close();

}
