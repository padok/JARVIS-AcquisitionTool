/*******************************************************************************
 * File:        triggersettingswinow.hpp
 * Created:     23. October 2020
 * Author:      Timo Hueser
 * Contact:     timo.hueser@gmail.com
 * Copyright:   2021 Timo Hueser
 * License:     LGPL v3.0
 ******************************************************************************/

#ifndef TRIGGERSETTINGSWINDOW_H
#define TRIGGERSETTINGSWINDOW_H

#include "globals.hpp"
#include "loadcamerapresetsinterface.hpp"
#include "presetswindow.hpp"
#include "savecamerapresetsinterface.hpp"
#include "settingsnode.hpp"
#include "settingsobject.hpp"
#include "toggleswitch.hpp"

#include <QDockWidget>
#include <QSpinBox>
#include <QSplitter>
#include <QStackedWidget>
#include <QTextEdit>

class TriggerSettingsWindow : public QDockWidget {
    Q_OBJECT
  public:
    explicit TriggerSettingsWindow(QWidget *parent = nullptr,
                                   SettingsObject *activeSettings = nullptr);

  public slots:
    void setSettingsObjectSlot(SettingsObject *);

  signals:
    void loadPreset(SettingsObject *);

  private:
    QSettings *settings;
    QString settingsName;
    QGridLayout *layout;
    QList<QString> presets;
    PresetsWindow *loadPresetsWindow;
    PresetsWindow *savePresetsWindow;
    SaveCameraPresetsInterface *saveCameraPresetsWindow;
    LoadCameraPresetsInterface *loadCameraPresetsWindow;

    QSplitter *mainSplitter;
    QWidget *mainWidget;
    QToolBar *toolBar;
    QToolButton *advancedSimpleButton;
    QAction *advancedSimpleAction;
    QToolButton *expandButton;
    QAction *expandAction;
    QToolButton *savePresetButton;
    QAction *savePresetAction;
    QToolButton *loadPresetButton;
    QAction *loadPresetAction;

    QTextEdit *toolTipBox;

    QToolBar *searchBar;
    QLineEdit *searchEdit;

    QStackedWidget *advancedSimpleStackWidget;
    QSpinBox *frameRateEdit;
    QDoubleSpinBox *frameLimitEdit;
    QDoubleSpinBox *cmdDelayEdit;
    ToggleSwitch *syncRisingEdgeToggle;

    SettingsObject *m_activeSettings = nullptr;

    void saveSettingsLayer(SettingsNode *node);
    void loadSettingsLayer(SettingsNode *node);

    int searchRecursive(QTreeWidgetItem *parent,
                        QList<QTreeWidgetItem *> results);

  private slots:
    void searchEditedSlot(const QString &text);
    void advancedSimpleToggledSlot(bool toggle);
    void expandClickedSlot();
    void treeItemActivatedSlot(QTreeWidgetItem *item, int column);
    void savePresetsClickedSlot();
    void loadPresetsClickedSlot();
    void savePresetSlot(const QString &preset);
    void loadPresetSlot(const QString &preset);
    void frameRateEditChangedSlot(int val);
    void frameLimitEditChangedSlot(double val);
    void cmdDelayEditChangedSlot(double val);
    void syncRisingEdgeToggleChangedSlot(bool val);
};

#endif
