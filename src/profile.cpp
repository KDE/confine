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

#include "profile.h"
#include <KConfig>
#include <KConfigGroup>
#include <ksharedconfig.h>
#include <kconfigbase.h>

Profile::Profile()
{

}

Profile::Profile(QString& profileDir) : name(profileDir), directory(profileDir)
{

}

Profile::Profile(QString& profileDir, QString& profileName) : name(profileName), directory(profileDir)
{

}

Profile::~Profile()
{

}

QString Profile::getDirectory() const
{
    return directory;
}

QString Profile::getName() const
{
    return name;
}

QStringList Profile::getKDEActionRestrictions()
{
    KSharedConfigPtr config = KSharedConfig::openConfig(directory + "kdeglobals");
    KConfigGroup grp(config, "KDE Action Restrictions");
    return grp.keyList();
}

void Profile::setKDEActionRestriction(QString& key, QString& value)
{
    KConfig config(directory + "kdeglobals");
    KConfigGroup grp(&config, "KDE Action Restrictions");
    if (!grp.isImmutable()) {
        grp.writeEntry(key, value, KConfigBase::Persistent);
    }
}
