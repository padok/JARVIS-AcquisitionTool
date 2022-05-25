/*******************************************************************************
 * File:        serialinterface.hpp
 * Created:     23 June 2020
 * Author:      Timo Hueser
 * Contact:     timo.hueser@gmail.com
 * Copyright:   2021 Timo Hueser
 * License:     LGPL v3.0
 ******************************************************************************/

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include "globals.hpp"
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

class SerialInterface {

  public:
    static QList<QString> getAvailableDevices();
    SerialInterface(const QString &deviceName);
    ~SerialInterface();
    int write(int val);
    int send_instruction(int mode, int readwrite, int val1, int val2);
    int get_answer(int answer[]);
    int get_answer();
    bool isConnected() { return serial_conn; }

  private:
    bool serial_conn = false;
    QString serialPortName;
    QSerialPort *serialPort;
};

#endif
