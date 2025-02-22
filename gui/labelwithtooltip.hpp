/*******************************************************************************
 * File:        labelwithtooltip.hpp
 * Created:     01. August 2021
 * Author:      Timo Hueser
 * Contact:     timo.hueser@gmail.com
 * Copyright:   2021 Timo Hueser
 * License:     LGPL v3.0
 ******************************************************************************/

#ifndef LABELWITHTOOLTIP_H
#define LABELWITHTOOLTIP_H

#include "globals.hpp"

#include <QGridLayout>
#include <QLabel>

class LabelWithToolTip : public QWidget {
    Q_OBJECT
  public:
    explicit LabelWithToolTip(QString name, QString toolTip = "Tip goes here",
                              QWidget *parent = nullptr);
};

#endif
