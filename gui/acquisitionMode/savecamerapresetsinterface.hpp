/*******************************************************************************
 * File:        savecamerapresetsinterface.hpp
 * Created:     23. October 2020
 * Author:      Timo Hueser
 * Contact:     timo.hueser@gmail.com
 * Copyright:   2021 Timo Hueser
 * License:     LGPL v3.0
 ******************************************************************************/

#ifndef SAVECAMERAPRESETSINTERFACE_H
#define SAVECAMERAPRESETSINTERFACE_H

#include "globals.hpp"

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QToolBar>
#include <QToolButton>

class SaveCameraPresetsInterface : public QDialog {
    Q_OBJECT

  public:
    explicit SaveCameraPresetsInterface(QWidget *parent = nullptr){};
    virtual QString getActivePreset() = 0;

  signals:

  private:
  private slots:
};

#endif
