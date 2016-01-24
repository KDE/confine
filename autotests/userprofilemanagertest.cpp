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

#include "userprofilemanagertest.h"
#include "userprofilemanager.h"
#include "confineconfiguration.h"

#include <QtTest>

QTEST_MAIN(UserProfileManagerTest)

void UserProfileManagerTest::testInitialization()
{
  ConfineConfiguration* confineConfiguration = new ConfineConfiguration();
  confineConfiguration->setPasswdPath(QDir::currentPath() + QLatin1String("/data/passwd"));
  qApp->setProperty("confineConfiguration", QVariant::fromValue<ConfineConfiguration*>(confineConfiguration));
  //create temporary test passwd file
  QFile tmpFile(QDir::currentPath() + QLatin1String("/data/passwd"));
  if (!tmpFile.open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QTextStream out(&tmpFile);
  out << "kde-test:x:1001:1001::";
  out << QDir::currentPath() + QLatin1String("/data");
  out << ":/bin/bash\n";
  
  out.flush();

  //create temporary test .profile file
  QFile tmpFileProfile(QDir::currentPath() + QLatin1String("/data/.profile"));
  if (!tmpFileProfile.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out2(&tmpFileProfile);
  out2 << "export XDG_CONFIG_DIRS=";
  out2 << QDir::currentPath() + QLatin1String("/data/kf5-profile");
  out2 << ":/etc/xdg/\n";
  
  out2.flush();
  
  //start testing
  UserProfileManager um;
  QCOMPARE(um.getUserNames().at(0), QLatin1String("kde-test"));
  
  QVERIFY(um.getProfileNames().contains(QDir::currentPath() + QLatin1String("/data/kf5-profile")));
  QVERIFY(um.getProfileNames().contains(QLatin1String("/etc/xdg/")));
  
  QCOMPARE(um.getProfilesfromUser(QLatin1String("kde-test")).at(0).getName(), QDir::currentPath() + QLatin1String("/data/kf5-profile"));
  QCOMPARE(um.getProfilesfromUser(QLatin1String("kde-test")).at(1).getName(), QLatin1String("/etc/xdg/"));
  
  tmpFileProfile.remove();
  tmpFile.remove();
}


#include "userprofilemanagertest.moc"
