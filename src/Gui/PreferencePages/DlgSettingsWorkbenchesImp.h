// SPDX-License-Identifier: LGPL-2.1-or-later

/****************************************************************************
*   Copyright (c) 2020 Chris Hennes (chennes@pioneerlibrarysystem.org)     *
*   Copyright (c) 2023 FreeCAD Project Association                         *
*                                                                          *
*   This file is part of FreeCAD.                                          *
*                                                                          *
*   FreeCAD is free software: you can redistribute it and/or modify it     *
*   under the terms of the GNU Lesser General Public License as            *
*   published by the Free Software Foundation, either version 2.1 of the   *
*   License, or (at your option) any later version.                        *
*                                                                          *
*   FreeCAD is distributed in the hope that it will be useful, but         *
*   WITHOUT ANY WARRANTY; without even the implied warranty of             *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       *
*   Lesser General Public License for more details.                        *
*                                                                          *
*   You should have received a copy of the GNU Lesser General Public       *
*   License along with FreeCAD. If not, see                                *
*   <https://www.gnu.org/licenses/>.                                       *
*                                                                          *
***************************************************************************/


#ifndef GUI_DIALOG_DLGSETTINGSWORKBENCHES_IMP_H
#define GUI_DIALOG_DLGSETTINGSWORKBENCHES_IMP_H

#include <Gui/PropertyPage.h>
#include <Gui/ListWidgetDragBugFix.h>
#include <memory>

namespace Gui::Dialog {
class Ui_DlgSettingsWorkbenches;

/**
 * The DlgSettingsWorkbenchesImp class implements a pseudo-preference page explain why
 * the remaining preference pages aren't loaded yet, and to help the user do so on demand.
 * \author Jürgen Riegel
 */
class DlgSettingsWorkbenchesImp : public PreferencePage
{
    Q_OBJECT

public:
    explicit DlgSettingsWorkbenchesImp( QWidget* parent = nullptr );
    ~DlgSettingsWorkbenchesImp() override;

    void saveSettings() override;
    void loadSettings() override;

protected Q_SLOTS:
    void wbItemMoved();
    void onWbSelectorChanged(int index);
    void onWbByTabToggled(bool val);

protected:
    void buildWorkbenchList();
    void changeEvent(QEvent *e) override;

private:

    void setStartWorkbenchComboItems();

    void saveWorkbenchSelector();
    void loadWorkbenchSelector();


    std::string _startupModule;

    std::unique_ptr<Ui_DlgSettingsWorkbenches> ui;
};

class WorkbenchList : public ListWidgetDragBugFix
{
    Q_OBJECT

public:
    explicit WorkbenchList(QWidget* parent = nullptr);

    void saveSettings();
    void loadSettings();

    static QStringList getEnabledWorkbenches();
    static QStringList getDisabledWorkbenches();
    void setStartupWorkbench(QString workbenchName);

protected Q_SLOTS:
    void onWbToggled(const QString& wbName, bool enabled);

Q_SIGNALS:
    void wbToggled(const QString& wbName, bool enabled);

protected:
    void buildWorkbenchList();
    // std::string _startupModule;
    std::vector<std::string> _backgroundAutoloadedModules;

private:
    void addWorkbench(const QString& it, bool enabled);
};


}// namespace Gui::Dialog

#endif // GUI_DIALOG_DLGSETTINGSWORKBENCHES_IMP_H
