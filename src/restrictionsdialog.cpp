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

#include "restrictionsdialog.h"

#include <QMessageBox>
#include <KLocalizedString>
#include <QDebug>

RestrictionsDialog::RestrictionsDialog(const Profile& pf, QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.cancelButton, SIGNAL(released()), this, SLOT(close()));
    connect(ui.saveButton, SIGNAL(released()), this, SLOT(save()));
    profile = pf;
    readKDERestrictionsFromProfile();
}

void RestrictionsDialog::fillWithRestrictions()
{
    QMap<QString, QString> profileRestrictions = profile.getKDEActionRestrictions();
    ui.restrictionSets->clear();
    ui.modulRestrictionSets->clear();

    kdeRestrictions.append(RestrictionElement("action/logout", i18n("Disable Logout option"), i18n("This prevents the user from logging out. To make this option effective it is important to ensure that key-combinations to terminate the X-server, such as Alt-Ctrl-Backspace, are disabled in the configuration of the X-server.")));
    kdeRestrictions.append(RestrictionElement("skip_drm", i18n("Enforce DRM restrictions"), i18n("Some KDE applications can enforce DRM restrictions on content. You may want to enable this if your organisation relies on such restrictions or if local law requires it.")));
    kdeRestrictions.append(RestrictionElement("action/run_command", i18n("Disable \"Run Command\" option (Alt-F2)"), i18n("The \"Run Command\" option can normally be used to execute arbitrary commands; however, when access to a command shell is restricted only applications and services defined by a .desktop file can be started this way. Disabling \"Run Command\" here hides the option completely.")));
    kdeRestrictions.append(RestrictionElement("action/kwin_rmb", i18n("Disable Window Manager context menu (Alt-F3)"), i18n("The Window Manager context menu is normally shown when Alt-F3 is pressed or when the menu button on the window frame is pressed.")));
    kdeRestrictions.append(RestrictionElement("action/bookmarks", i18n("Disable Bookmarks"), i18n("Disable Bookmarks in all applications.")));
    kdeRestrictions.append(RestrictionElement("action/shell_access", i18n("Disable access to a command shell"), i18n("In an environment where the desktop is dedicated to a defined set of tasks it may be desirable to disable access to a command shell to prevent users from engaging in tasks that were not intended or authorized; hence, it is strongly recommended to disable access to a command shell if the desktop is to act as a public terminal.")));
    kdeRestrictions.append(RestrictionElement("lock_screen", i18n("Disable Lock Screen option"), i18n("When the desktop is to act as a public terminal or is to be shared by different users it may be desirable to prevent locking of the screen to ensure that the system remains accessible if a user leaves the terminal.")));
    kdeRestrictions.append(RestrictionElement("movable_toolbars", i18n("Disable toolbar moving"), i18n("Normally toolbars in applications can be moved around; when this option is chosen all toolbars are fixed in their original positions.")));
    kdeRestrictions.append(RestrictionElement("run_desktop_files", i18n("Disable execution of arbitrary .desktop files."), i18n("This option defines whether users may execute .desktop files that are not part of the system-wide desktop icons, KDE menu, registered services or autostart services. When access to a command shell is restricted it is recommended to to disable the execution of arbitrary .desktop files as well since such .desktop files can be used to circumvent the command shell restriction.")));
    kdeRestrictions.append(RestrictionElement("lineedit_text_completion", i18n("Disable input line history"), i18n("If a single account is used by multiple people it may be desirable to disable the input line history out of privacy considerations.")));
    kdeRestrictions.append(RestrictionElement("action/kdesktop_rmb", i18n("Disable context menus"), i18n("When checked the user will no longer get any context menu. Normally the user can get a context menu by clicking with the right mouse button.")));
    kdeRestrictions.append(RestrictionElement("action/menuedit", i18n("Disable menu editing"), i18n("This disables the menu-option to edit the KDE Application Menu. When disabled, users will no longer be able to make changes to their personal application menu.")));
    kdeRestrictions.append(RestrictionElement("action/openwith", i18n("Disable Open With action"), i18n("This option can be used to disable the <i>Open With</i> menu option.")));
    kdeRestrictions.append(RestrictionElement("action/editfiletype", i18n("Disable \"Edit file type\" in properties dialog"), i18n("The \"Edit file type\" can be used to change how files are recognized and with which applications such files are opened.")));
    kdeRestrictions.append(RestrictionElement("action/help_contents", i18n("Disable Help -> <Application> Handbook"), i18n("This option can be used to remove the <i>Help -> <Application> Handbook</i> menu-option from all applications.")));
    kdeRestrictions.append(RestrictionElement("action/help_whats_this", i18n("Disable Help -> What's this?"), i18n("This option can be used to remove the <i>Help -> What's this?</i> menu-option from all applications.")));
    kdeRestrictions.append(RestrictionElement("action/help_report_bug", i18n("Disable Help -> Report Bug..."), i18n("This option can be used to remove the <i>Help -> Report Bug...</i> menu-option from all applications. ")));
    kdeRestrictions.append(RestrictionElement("action/help_about_app", i18n("Disable Help -> About <Application> "), i18n("This option can be used to remove the <i>Help -> About <Application></i> menu-option from all applications. ")));
    kdeRestrictions.append(RestrictionElement("action/help_about_kde", i18n("Disable Help -> About KDE"), i18n("This option can be used to remove the <i>Help -> About KDE</i> menu-option from all applications.")));
    kdeRestrictions.append(RestrictionElement("action/options_show_toolbar", i18n("Disable Settings -> Configure Toolbars..."), i18n("This option can be used to remove the <i>Settings -> Configure Toolbars...</i> menu-option from all applications.")));
    kdeRestrictions.append(RestrictionElement("action/allow_downloading", i18n("Prevent Downloading"), i18n("Some KDE applications can prevent the user from downloading or uploading files.  The normal web browsing experience will not be affected.  Note that you still have to set up the unix file permissions correctly - treat this as merely cosmetic for security.")));
    kdeRestrictions.append(RestrictionElement("action/editable_desktop_icons", i18n("Lock down all Desktop icons"), i18n("When checked the user will not be able to remove or edit any existing icon or file on the Desktop or add any new icon or file.")));
    kdeRestrictions.append(RestrictionElement("action/start_new_session", i18n("Disable starting of a second X session."), i18n("KDM has the possibility to login a second user in parallel to the current session. Note that this can also be configured as part of the KDM settings in which case the setting here should be left enabled.")));
    //TODO: get detailed desciptions
    kdeRestrictions.append(RestrictionElement("plasma-desktop/add_activities", i18n("plasma-desktop/add_activities"), i18n("")));
    kdeRestrictions.append(RestrictionElement("warn_unwritable_config", i18n("warn_unwritable_config"), i18n("")));
    kdeRestrictions.append(RestrictionElement("plasma/allow_configure_when_locked", i18n("pplasma/allow_configure_when_locked"), i18n("")));
    kdeRestrictions.append(RestrictionElement("plasma/plasmashell/unlockedDesktop", i18n("plasma/plasmashell/unlockedDesktop"), i18n("Whether or not widgets can be unlocked by the user. Unlocking widgets would allow the user to move around, add, and remove widgets, panels, launchers, etc.")));
    kdeRestrictions.append(RestrictionElement("plasma-desktop/scripting_console", i18n("plasma - desktop / scripting_console"), i18n("")));
    kdeRestrictions.append(RestrictionElement("action/switch_user", i18n("action/switch_user"), i18n("")));
    kdeRestrictions.append(RestrictionElement("action/switch_application_language", i18n("action/switch_application_language"), i18n("")));
    kdeRestrictions.append(RestrictionElement("action/plasma/containment_actions", i18n("action/plasma/containment_actions"), i18n("")));
    kdeRestrictions.append(RestrictionElement("action/show_lancelot", i18n("action/show_lancelot"), i18n("")));
    kdeRestrictions.append(RestrictionElement("action/lock widgets", i18n("action/lock widgets"), i18n("")));

    ui.restrictionSets->setRowCount(kdeRestrictions.count());
    ui.restrictionSets->setColumnCount(3);
    ui.restrictionSets->verticalHeader()->setVisible(false);

    QStringList m_TableHeader;
    m_TableHeader << i18n("Enabled") << i18n("Caption") << i18n("Description");
    ui.restrictionSets->setHorizontalHeaderLabels(m_TableHeader);


    for (int i = 0; i < kdeRestrictions.count(); i++) {
        QTableWidgetItem* capItem = new QTableWidgetItem(kdeRestrictions.at(i).getCaption());
        QTableWidgetItem* descItem = new QTableWidgetItem(kdeRestrictions.at(i).getDescription());
        QTableWidgetItem* enableItem = new QTableWidgetItem(QString());

        enableItem->setCheckState(Qt::Unchecked);

        if (profileRestrictions.contains(kdeRestrictions.at(i).getKey())) {
            if (profileRestrictions.value(kdeRestrictions.at(i).getKey()) == "true") {
                enableItem->setCheckState(Qt::Checked);
            }
        }
        ui.restrictionSets->setItem(i, 0, enableItem);
        ui.restrictionSets->setItem(i, 1, capItem);
        ui.restrictionSets->setItem(i, 2, descItem);

    }
    ui.restrictionSets->resizeColumnsToContents();

    QStringList controlRestrictions;
    controlRestrictions << "filebehavior" <<
                        "khtml_behavior" <<
                        "kde-kwinoptions.desktop";
    ui.modulRestrictionSets->insertItems(0, controlRestrictions);

}


void RestrictionsDialog::readKDERestrictionsFromProfile()
{
    fillWithRestrictions();

    QMap<QString, QString> controlRestrictions = profile.getKDEControlModuleRestrictions();
    for (int i = 0; i < ui.modulRestrictionSets->count(); i++) {
        QListWidgetItem* item = ui.modulRestrictionSets->item(i);
        item->setCheckState(Qt::Unchecked);

        if (controlRestrictions.contains(item->text())) {
            if (controlRestrictions.value(item->text()) == "true") {
                item->setCheckState(Qt::Checked);
            }
        }
    }
}

void RestrictionsDialog::save()
{

    for (int i = 0; i < ui.restrictionSets->rowCount(); i++) {
        QTableWidgetItem* item = ui.restrictionSets->item(i, 0);
        QString actionKey = kdeRestrictions.at(i).getKey();
        qDebug() << actionKey;
        if (item->checkState() == Qt::Unchecked) {
            if (!profile.setKDEActionRestriction(actionKey, "false")) {
                return;
            }
        } else if (item->checkState() == Qt::Checked) {
            if (!profile.setKDEActionRestriction(actionKey, "true")) {
                return;
            }
        }

    }

    for (int i = 0; i < ui.modulRestrictionSets->count(); i++) {
        QListWidgetItem* item = ui.modulRestrictionSets->item(i);
        if (item->checkState() == Qt::Unchecked) {
            if (!profile.setKDEControlModuleRestrictions(item->text(), "false")) {
                return;
            }
        } else if (item->checkState() == Qt::Checked) {
            if (!profile.setKDEControlModuleRestrictions(item->text(), "true")) {
                return;
            }
        }

    }

}

