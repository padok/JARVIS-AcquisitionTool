/*
###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (C) 2017 by Juergen Skrotzky (JorgenVikingGod@gmail.com)          #
#               >> https://github.com/Jorgen-VikingGod                        #
#                                                                             #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################
*/

#include "DarkStyle.hpp"
#include <iostream>

DarkStyle::DarkStyle() : DarkStyle(styleBase()) {}

DarkStyle::DarkStyle(QStyle *style) : QProxyStyle(style) {}

QStyle *DarkStyle::styleBase(QStyle *style) const {
    static QStyle *base =
        !style ? QStyleFactory::create(QStringLiteral("Fusion")) : style;
    return base;
}

QStyle *DarkStyle::baseStyle() const { return styleBase(); }

void DarkStyle::polish(QPalette &palette) {
    // modify palette to dark
    palette.setColor(QPalette::Window, QColor(46, 50, 60));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::WindowText,
                     QColor(127, 127, 127));
    palette.setColor(QPalette::Base, QColor(34, 36, 40));
    palette.setColor(QPalette::AlternateBase, QColor(32, 100, 164));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, QColor(37, 40, 47));
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    palette.setColor(QPalette::Dark, QColor(22, 24, 26));
    palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    palette.setColor(QPalette::Button, QColor(46, 50, 60));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,
                     QColor(127, 127, 127));
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(100, 164, 32));
    palette.setColor(QPalette::Disabled, QPalette::Highlight,
                     QColor(80, 80, 80));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText,
                     QColor(127, 127, 127));
}

void DarkStyle::polish(QApplication *app) {
    if (!app)
        return;
    // increase font size for better reading,
    // setPointSize was reduced from +2 because when applied this way in Qt5,
    // the font is larger than intended for some reason
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(12);
    app->setFont(defaultFont);

    // loadstylesheet
    QFile qfDarkstyle(QStringLiteral(":/darkstyle/darkstyle.qss"));
    if (qfDarkstyle.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // set stylesheet
        QString qsStylesheet = QString::fromLatin1(qfDarkstyle.readAll());
        app->setStyleSheet(qsStylesheet);
        qfDarkstyle.close();
    }
}
