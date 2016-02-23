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

#include "kconfigimmutable.h"

#include <QDir>
#include <KConfig>
#include <QTextStream>

KConfigGroup KConfigImmutable::setGroupImmutable(KConfigGroup& grp)
{
    QFile fileOut(QDir::currentPath() + QLatin1String("/temporary_config"));
    createSubsetConfigFile(fileOut, grp);
    if (!fileOut.open(QIODevice::ReadOnly | QIODevice::Text))
        return grp;
    QString searchString = QLatin1Char('[') + grp.name() + QLatin1String("]\n");
    QString replaceString = QLatin1Char('[') + grp.name() + QLatin1String("][$i]\n");


    return setImmutableStatus(fileOut, searchString, replaceString, grp);
}

KConfigGroup KConfigImmutable::setEntryImmutable(KConfigGroup& grp, QString& entry)
{
    QFile fileOut(QDir::currentPath() + QLatin1String("/temporary_config"));
    createSubsetConfigFile(fileOut, grp);

    if (!fileOut.open(QIODevice::ReadOnly | QIODevice::Text))
        return grp;
    QString entryValue = grp.readEntry(entry);

    QString searchString = entry + QLatin1Char('=') + entryValue + QLatin1Char('\n');
    QString replaceString = entry + QLatin1String("[$i]=") + entryValue + QLatin1Char('\n');

    return setImmutableStatus(fileOut, searchString, replaceString, grp);
}

void KConfigImmutable::createSubsetConfigFile(QFile& fileOut, KConfigGroup& grp)
{
    KConfig config2(fileOut.fileName());
    KConfigGroup grp2(&config2, grp.name());
    grp.copyTo(&grp2);
    config2.sync();
}

KConfigGroup KConfigImmutable::setImmutableStatus(QFile& fileOut, QString& searchString, QString& replaceString, KConfigGroup& grp)
{
    QString fileContent = fileOut.readAll();
    fileContent.replace(searchString, replaceString);
    fileOut.close();

    KConfigGroup newGroup = createNewGroupFromSubsetConfig(fileOut, grp, fileContent);
    fileOut.remove();
    return newGroup;
}


KConfigGroup KConfigImmutable::createNewGroupFromSubsetConfig(QFile& fileOut, KConfigGroup& grp, QString& fileContent)
{

    if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
        return grp;
    QTextStream out(&fileOut);
    out << fileContent;
    out.flush();
    KConfig config3(fileOut.fileName());
    KConfigGroup grp3(&config3, grp.name());
    grp3.copyTo(&grp);
    KConfig* config = grp.config();
    config->sync();
    KConfigGroup newGroup(config, grp.name());
    fileOut.close();
    return newGroup;
}
