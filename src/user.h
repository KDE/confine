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

#ifndef USER_H
#define USER_H

#include "profile.h"
#include <QString>
#include <QList>

class User
{
public:
  User();
  User(const QString& userName, const QString& userShell, const QString& userHomeDir);
  ~User();
  QString getName() const;
  QString getHomeDir();
  QString getXDG_CONFIG_HOME();
  QString getEnvironmentVariableFile();
  void setXDG_CONFIG_HOME(const QString& configHome);
  void setEnvironmentVariableFile(const QString& varFile);
  void save();
  void addProfile(const Profile& profile);
  void clearProfiles();
  QList<Profile> getProfiles();
  bool canLogin() const;
  
private:
  QString name;
  QString shell;
  QString homeDir;
  QString XDG_CONFIG_HOME;
  QString environmentVariableFile;
  QList<Profile> profiles;
};

#endif // USER_H
