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

#include "immutabletest.h"
#include "kconfigimmutable.h"

#include <KConfig>
#include <QtTest>

QTEST_MAIN(ImmutableTest)

void ImmutableTest::setImmutable()
{
    QFile testConfig(QDir::currentPath() + QLatin1String("/data/immutableTestConfig"));
    QVERIFY(testConfig.open(QIODevice::WriteOnly | QIODevice::Text));

    QTextStream out(&testConfig);
    out << "[Group1]\n";
    out << "bar=false\n";
    out << "foo=0\n";
    out << "foobar=test\n";
    out << "\n";
    out << "[Group2]\n";
    out << "bar=true\n";
    out << "foo=1\n";
    out << "foobar=new\n";
    out.flush();

    testConfig.close();

    KConfig config(QDir::currentPath() + QLatin1String("/data/immutableTestConfig"));
    KConfigGroup grp(&config, "Group1");
    QVERIFY(!grp.isImmutable());
    KConfigGroup grpNew = KConfigImmutable::setGroupImmutable(grp);
    QVERIFY(grpNew.isImmutable());
    
    QString entry("bar");
    KConfigGroup grpEntryTest(&config, "Group2");
    QVERIFY(!grpEntryTest.isEntryImmutable(entry));
    KConfigGroup grpEntryTestNew = KConfigImmutable::setEntryImmutable(grpEntryTest, entry, true);
    QVERIFY(grpEntryTestNew.isEntryImmutable(entry));
    
    grpEntryTestNew = KConfigImmutable::setEntryImmutable(grpEntryTestNew, entry, false);
    QVERIFY(!grpEntryTestNew.isEntryImmutable(entry));
    
    
    QVERIFY(testConfig.remove());
}

#include "immutabletest.moc"
