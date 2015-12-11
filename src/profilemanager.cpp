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

#include "profilemanager.h"
#include <ksharedconfig.h> 
#include <KConfigGroup>
#include <QDir>
#include <QDebug> 
#include <KConfig>

ProfileManager::ProfileManager()
{ 

}

void ProfileManager::createProfile(const QString& name)
{
    if (name.isEmpty())
        return;
    KConfig config("/etc/kde4rc");
    //qDebug() << config.isConfigWritable();
    QString newProfileEntry = QString("Directories-%1").arg(name);

    KConfigGroup grp(&config, newProfileEntry);
    QString profilePath("/usr/share/kubuntu-default-settings/kde4-profile/");
    grp.writeEntry("prefixes", profilePath + name + '/');
    grp.sync();
    config.sync();

}

QString ProfileManager::getProfilePrefix()
{

    KConfigGroup grp(KSharedConfig::openConfig("/etc/kde4rc"), "Directories");

    QString prefix = grp.readEntry("prefixes");
    if (!prefix.isEmpty() && !prefix.endsWith('/'))
        prefix.append('/');
    return prefix;

}

QString ProfileManager::newProfile()
{

    QString profilePrefix = getProfilePrefix();

    KSharedConfigPtr config = KSharedConfig::openConfig();
    for (int p = 1; p; p++) {
        QString profile = QString("profile%1").arg(p);
        QString group = QString("Directories-%1").arg(profile);
        if (!config->hasGroup(group)) {
            if (profilePrefix.isEmpty())
                return profile;

            QString profileDir = profilePrefix + profile;
            if (!QDir(profileDir).exists() && !QFile::exists(profileDir))
                return profile;

            // Keep on looking...
        }
    }
    return QString();

}


QStringList ProfileManager::allProfiles()
{
    KSharedConfigPtr config = KSharedConfig::openConfig("/etc/kde4rc");
    QStringList groups = config->groupList();
    QStringList profiles;
    QStringList directories;
    foreach (QString group, groups) {
        //qDebug()<< "f: " <<group;
        if (!group.startsWith(QLatin1String("Directories-")))
            continue;
        profiles.append(group.mid(12));
        KConfigGroup grp =  config->group(group);
        QString installDir = grp.readEntry("prefixes");
        if (!installDir.endsWith('/'))
            installDir.append('/');
        directories.append(installDir);
    }

    QString profilePrefix = getProfilePrefix();
    if (!profilePrefix.isEmpty()) {
        QDir dir(profilePrefix, QString(), QDir::Unsorted, QDir::Dirs);
        QStringList profileDirs = dir.entryList();
        foreach (QString profileDir, profileDirs) {
            if (profileDir.startsWith(QLatin1Char('.')))
                continue;
            QString dir = profilePrefix + profileDir + '/';
            if (directories.contains(dir)) {

                continue;
            }
            if (profiles.contains(profileDir)) {
                continue;
            }

            if (!QFile::exists(dir + ".kdeprofile")) {
                continue;
            }
            profiles.append(profileDir);
            directories.append(dir);
        }
    }

    if (!profiles.contains("default"))
        profiles.append("default");

    return profiles;

}

void ProfileManager::addUser(const QString& profileName, const QString& userName)
{
  KConfig config(getUserMappingList());
  KConfigGroup grp(&config, "Users");
  grp.writeEntry(userName, profileName);
  grp.sync();
  config.sync(); 
}

void ProfileManager::addGroup(const QString& profileName, const QString& groupName)
{
  KConfig config(getUserMappingList());
  KConfigGroup grp(&config, "Groups");
  grp.writeEntry(groupName, profileName);
  grp.sync();
  config.sync(); 
}

QString ProfileManager::getUserMappingList()
{
    KConfigGroup grp(KSharedConfig::openConfig("/etc/kde4rc"), "Directories");

    QString prefix = grp.readEntry("userProfileMapFile");
    return prefix;
}

