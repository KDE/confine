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

RestrictionsDialog::RestrictionsDialog(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.cancelButton, SIGNAL(released()), this, SLOT(close()));
    connect(ui.saveButton, SIGNAL(released()), this, SLOT(save()));
}

void RestrictionsDialog::fillWithRestrictions()
{
    ui.restrictionSets->clear();
    ui.modulRestrictionSets->clear();

    QStringList actionRestrictions;
    actionRestrictions << "movable_toolbars" <<
                       "plasma-desktop/add_activities" <<
                       "lineedit_text_completion" <<
                       "run_desktop_files" <<
                       "warn_unwritable_config" <<
                       "shell_access" <<
                       "plasma/allow_configure_when_locked" <<
                       "plasma-desktop/add_activities" <<
                       "skip_drm" <<
                       "logout" <<
                       "run_command" <<
                       "action/kdesktop_rmb" <<
                       "plasma/plasmashell/unlockedDesktop" <<
                       "plasma-desktop/scripting_console" <<
                       "action/menuedit" <<
                       "action/openwith" <<
                       "action/editfiletype" <<
                       "action/logout" <<
                       "action/lock_screen" <<
                       "action/start_new_session" <<
                       "action/switch_user" <<
                       "action/help_contents" <<
                       "action/help_whats_this" <<
                       "action/help_report_bug" <<
                       "action/switch_application_language" <<
                       "action/help_about_app" <<
                       "action/help_about_kde" <<
                       "action/options_show_toolbar" <<
                       "action/allow_downloading" <<
                       "action/bookmarks" <<
                       "action/switch_application_language" <<
                       "action/plasma/containment_actions" <<
                       "action/shell_access" <<
                       "action/editable_desktop_icons" <<
                       "action/bookmarks" <<
                       "action/kwin_rmb" <<
                       "action/show_lancelot" <<
                       "action/run_command" <<
                       "action/lock widgets";
    ui.restrictionSets->insertItems(0, actionRestrictions);

    QStringList controlRestrictions;
    controlRestrictions << "filebehavior" <<
                        "khtml_behavior" <<
                        "kde-kwinoptions.desktop";
    ui.modulRestrictionSets->insertItems(0, controlRestrictions);

}


void RestrictionsDialog::readKDERestrictionsFromProfile(const Profile& pf)
{
    profile = pf;

    fillWithRestrictions();

    QMap<QString, QString> profileRestrictions = pf.getKDEActionRestrictions();
    for (int i = 0; i < ui.restrictionSets->count(); i++) {
        QListWidgetItem* item = ui.restrictionSets->item(i);
        item->setCheckState(Qt::Unchecked);

        if (profileRestrictions.contains(item->text())) {
            if (profileRestrictions.value(item->text()) == "true") {
                item->setCheckState(Qt::Checked);
            }
        }
    }

    QMap<QString, QString> controlRestrictions = pf.getKDEControlModuleRestrictions();
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

    for (int i = 0; i < ui.restrictionSets->count(); i++) {
        QListWidgetItem* item = ui.restrictionSets->item(i);
        if (item->checkState() == Qt::Unchecked) {
            if (profile.setKDEActionRestriction(item->text(), "false")) {
                return;
            }
        } else if (item->checkState() == Qt::Checked) {
            if (profile.setKDEActionRestriction(item->text(), "true")) {
                return;
            }
        }

    }

    for (int i = 0; i < ui.modulRestrictionSets->count(); i++) {
        QListWidgetItem* item = ui.modulRestrictionSets->item(i);
        if (item->checkState() == Qt::Unchecked) {
            if (profile.setKDEControlModuleRestrictions(item->text(), "false")) {
                return;
            }
        } else if (item->checkState() == Qt::Checked) {
            if (profile.setKDEControlModuleRestrictions(item->text(), "true")) {
                return;
            }
        }

    }

}

