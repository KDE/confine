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

#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KLocalizedString>

#include "mainwindow.h"
#include "confineconfiguration.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("confine");

    KAboutData aboutData(
        QStringLiteral("confine"),
        i18n("Confine"),
        QStringLiteral("0.1"),
        i18n("Replacement to the KIOSK tool"),
        KAboutLicense::GPL,
        i18n("(c) 2015"),
        QStringLiteral("https://projects.kde.org/projects/playground/utils/confine"),
        QStringLiteral("submit@bugs.kde.org"));
    
    aboutData.addAuthor(i18n("Gilbert Assaf"), i18n("Project maintainer & developer, overall design"), "gassaf@gmx.de");
    aboutData.addAuthor(i18n("Siddharth Sharma"), i18n("Mentor"), "Siddharth.kde@gmail.com");
    
    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    ConfineConfiguration* confineConfiguration = new ConfineConfiguration();
    app.setProperty("confineConfiguration", QVariant::fromValue<ConfineConfiguration*>(confineConfiguration));
    
    MainWindow* window = new MainWindow();
    window->show();

    return app.exec();
}
