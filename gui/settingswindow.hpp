/*******************************************************************************
 * File:        settingswindow.hpp
 * Created:     23. October 2020
 * Author:      Timo Hueser
 * Contact:     timo.hueser@gmail.com
 * Copyright:   2021 Timo Hueser
 * License:     LGPL v3.0
 ******************************************************************************/

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "globals.hpp"
#include "toggleswitch.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSlider>
#include <QSpinBox>

class SettingsWindow : public QWidget {
    Q_OBJECT

  public:
    explicit SettingsWindow(QWidget *parent = nullptr);

  signals:

  public slots:

  private:
    void loadSettings();
    void updateSettings();

    QSettings *settings;
    QPushButton *closeButton;

    // recorderSettings
    QComboBox *recorderTypeBox;
    QComboBox *imageOrVideoSelectorBox;
    QSpinBox *jpegQualityEdit;

    // streamingSettings
    QComboBox *streamingSubsamplingRatioBox;
    ToggleSwitch *streamingEnabledSwitch;
    QComboBox *recordingSubsamplingRatioBox;

  private slots:
    void recorderTypeChangedSlot(const QString &type);
    void imageOrVideoSelectorChangedSlot(const QString &type);
    void jpegQualityChangedSlot(int val);
    void streamingSubsamplingRatioChangedSlot(const QString &ratio);
    void streamingEnableToggledSlot(bool toggle);
    void recordingSubsamplingRatioChangedSlot(const QString &ratio);
};

#endif
