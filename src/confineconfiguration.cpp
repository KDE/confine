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

#include "confineconfiguration.h"

#include <KConfig>
#include <KConfigGroup>
#include <ksharedconfig.h>
#include <kconfigbase.h>

ConfineConfiguration::ConfineConfiguration() : passwdPath("/etc/passwd"), configxtDir("/usr/share/config.kcfg/")
{

}

QString ConfineConfiguration::getPasswdPath()
{
    return passwdPath;
}

QString ConfineConfiguration::getConfigXTDir()
{
    return configxtDir;
}

void ConfineConfiguration::setConfigXTDir(const QString& configDir)
{
    configxtDir = configDir;
    if (!configxtDir.endsWith(QLatin1Char('/'))) {
        configxtDir += QLatin1Char('/');
    }

}

void ConfineConfiguration::setPasswdPath(const QString& passwd)
{
    passwdPath = passwd;
}

QString ConfineConfiguration::getXDGConfigDirsDefault()
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup grp(config, "Global");

    return grp.readEntry("XDG_CONFIG_DIRS_DEFAULT");
}
