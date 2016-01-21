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

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include "profile.h"

#include <QList>
#include <QHash>

class UserProfileManager
{
public:
UserProfileManager();
~UserProfileManager();
QStringList getUserNames();
QStringList getProfileNames();
QList<Profile> getProfilesfromUser(QString& userName);

private:
  QHash<QString, User> users;
  QHash<QString, Profile> profiles;
  QString XDG_CONFIG_DIRS;
  QString XDG_CONFIG_HOME;
  void getUsersOnSystem();
  void getXDGConfig(User& user);
};

#endif // USERMANAGER_H
