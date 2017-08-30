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

#include "usertest.h"
#include "user.h"
#include "profile.h"

#include <QtTest>

QTEST_MAIN(UserTest)


void UserTest::testUser()
{
    User user(QLatin1String("kde-test"), QLatin1String("/bin/bash"), QDir::currentPath() + QLatin1String("/data/"));
    QString envFilePath(QDir::currentPath() + QLatin1String("/data/.profile"));

    Profile profile1(QLatin1String("/etc/kf5-profile"));
    Profile profile2(QLatin1String("/etc/xdg"));
    Profile profile3(QLatin1String("/etc/test-profile"));

    user.addProfile(profile1);

    user.setEnvironmentVariableFile(QLatin1String(".profile"));
    user.save();

    QCOMPARE(readAllEnvFile(envFilePath), QLatin1String("export XDG_CONFIG_DIRS=/etc/kf5-profile\n"));

    user.clearProfiles();
    QFile::remove(QDir::currentPath() + QLatin1String("/data/.profile"));


    user.addProfile(profile3);
    user.addProfile(profile2);
    user.addProfile(profile1);
    user.save();
    QCOMPARE(readAllEnvFile(envFilePath), QLatin1String("export XDG_CONFIG_DIRS=/etc/test-profile:/etc/xdg:/etc/kf5-profile\n"));
    user.setXDG_CONFIG_HOME(QLatin1String("/etc/users"));
    user.save();
    QCOMPARE(readAllEnvFile(envFilePath), QLatin1String("export XDG_CONFIG_DIRS=/etc/test-profile:/etc/xdg:/etc/kf5-profile\nexport XDG_CONFIG_HOME=/etc/users\n"));
    QFile::remove(QDir::currentPath() + QLatin1String("/data/.profile"));

}

QString UserTest::readAllEnvFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QString result(file.readAll());

    return result;

}


#include "usertest.moc"
