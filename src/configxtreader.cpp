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

#include "configxtreader.h"
#include "kgroupinfo.h"
#include "kentryinfo.h"

#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QDebug>

ConfigXTReader::ConfigXTReader()
{
    QDir profileDir("/usr/share/config.kcfg");
    QStringList nameFilter("*.kcfg");
    QStringList allConfigFiles = profileDir.entryList(nameFilter);
    for (int i = 0; i < allConfigFiles.size(); i++) {
        QFile file("/usr/share/config.kcfg/" + allConfigFiles.at(i));
        qDebug() << allConfigFiles.at(i);
        parseKcfgFile(file);
    }

}

ConfigXTReader::~ConfigXTReader()
{

}

void ConfigXTReader::parseKcfgFile(QFile& file)
{
    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    QDomNodeList rates = doc.elementsByTagName("kcfg");

    QDomNodeList liste = rates.item(0).childNodes();
    QString configFileName;

    for (int i = 0; i < liste.size(); i++) {
        QDomNode n = liste.item(i);
        if (n.isElement()) {
            QDomElement currElement = n.toElement();
            if (currElement.tagName() == QLatin1String("kcfgfile")) {
                QDomNode configFileNameAttribut = currElement.attributes().namedItem("name");
                configFileName = configFileNameAttribut.toAttr().value();
            }

            if (currElement.tagName() == QLatin1String("group")) {
                parseGroupElement(currElement, configFileName);
            }

        }
    }
}

void ConfigXTReader::parseGroupElement(QDomElement& grpElement, QString& configFileName)
{
    QDomNodeList entries = grpElement.childNodes();
    KGroupInfo kGrpInfo(configFileName);
    KConfigFileInfo kConfigInfo(configFileName);
    if (configurationFilesInfos.contains(configFileName)) {
        kConfigInfo = configurationFilesInfos.value(configFileName);
    }
    for (int j = 0; j < entries.size(); j++) {
        QDomNode entry = entries.item(j);
        QDomNode entryName = entry.attributes().namedItem("name");
        QDomNode entryType = entry.attributes().namedItem("type");
        QDomElement label = entry.firstChildElement("label");
        QString name(entryName.toAttr().value());
        QString type(entryType.toAttr().value());
        QString infoLabel(label.text());
        KEntryInfo kInfo(name, type, infoLabel);
        kGrpInfo.addKEntryInfo(kInfo);
    }

}
