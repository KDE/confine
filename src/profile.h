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

#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QStringList>
#include <QMap>

class Profile
{
public:
  Profile();
  explicit Profile(const QString& profileDir);
  Profile(QString& profileDir, QString& profileName);
  ~Profile();
  QMap<QString, QString> getKDEActionRestrictions() const;
  void setKDEActionRestriction(const QString& key, const QString& value);
  QMap<QString, QString> getKDEControlModuleRestrictions() const;
  void setKDEControlModuleRestrictions(const QString& key, const QString& value);
  QStringList getConfigFiles() const;
  QString getDirectory() const;
  QString getName() const;
  
  
private:
  void refreshConfigurationFilesFromProfile();
  QString name;
  QString directory;
  QStringList configurationFiles;
};

#endif // PROFILE_H
