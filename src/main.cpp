#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KLocalizedString>

#include "mainwindow.h"

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

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    MainWindow* window = new MainWindow();
    window->show();

    return app.exec();
}
