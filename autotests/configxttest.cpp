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

#include "configxttest.h"
#include "kentryinfo.h"
#include "kgroupinfo.h"
#include "kconfigfileinfo.h"
#include "confineconfiguration.h"
#include "configxtreader.h"
#include "configurationinfomanager.h"

#include <QtTest>
#include <KConfig>
#include <KConfigGroup>
#include <ksharedconfig.h>
#include <kconfigbase.h>
#include <kconfiggroup.h>

QTEST_MAIN(ConfigXTTest)

void ConfigXTTest::testConfig()
{
    ConfineConfiguration* confineConfiguration = new ConfineConfiguration();
    confineConfiguration->setConfigXTDir(QDir::currentPath() + QLatin1String("/data"));
    qApp->setProperty("confineConfiguration", QVariant::fromValue<ConfineConfiguration*>(confineConfiguration));
    
    ConfigXTReader configReader;
    QHash<QString, KConfigFileInfo> infos = configReader.getConfigurationFilesInfos();

    QVERIFY(infos.contains(QLatin1String("kjotsrc")));
    QCOMPARE(infos.size(), 1);
    KConfigFileInfo ki = infos.value(QLatin1String("kjotsrc"));
    KGroupInfo gi = ki.getKGroupInfo(QLatin1String("kjots"));
    QCOMPARE(gi.getName(), QLatin1String("kjots"));

    KEntryInfo kei = gi.getKEntryInfo(QLatin1String("OpenBooks"));

    QCOMPARE(kei.getLabel(), QLatin1String("All books that are opened."));

}

void ConfigXTTest::testInfoManager()
{
    KSharedConfigPtr config = KSharedConfig::openConfig(QDir::currentPath() + QLatin1String("/data/kjotsrc"));
    KConfigGroup grp(config, "kjots");
    ConfigurationInfoManager confInfo;
    QCOMPARE(confInfo.getInfo(grp, QLatin1String("CurrentBook")), QLatin1String("The book currently opened."));
    QCOMPARE(confInfo.getInfo(grp, QLatin1String("Height")), QLatin1String("Height of the main window."));
    QCOMPARE(confInfo.getInfo(grp, QLatin1String("SplitterSizes")), QLatin1String("How the main window is divided."));
    QCOMPARE(confInfo.getInfo(grp, QLatin1String("DoesNotExist")), QString());
    
}


#include "configxttest.moc"
