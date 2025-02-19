/*******************************************************************************
 * File:        arduinotrigger.hpp
 * Created:     03. October 2021
 * Author:      Timo Hueser
 * Contact:     timo.hueser@gmail.com
 * Copyright:   2021 Timo Hueser
 * License:     LGPL v3.0
 ******************************************************************************/

#ifndef ARDUINOTRIGGER_H
#define ARDUINOTRIGGER_H

#include "arduinoserialpeer.hpp"
#include "globals.hpp"
#include "serialinterface.hpp"
#include "triggerinterface.hpp"

#include <QQueue>
#include <QThread>
#include <QTreeWidget>

class ArduinoTriggerWorker : public QObject {
    Q_OBJECT
  public:
    ArduinoTriggerWorker(const QString &deviceName);
    ~ArduinoTriggerWorker();

  private:
    SerialInterface *serialInterface;
    SerialPeer *serialPeer;

  public slots:
    void sendSetupSlot(int cmdDelay, int frameRate, int frameLimit,
                       bool syncRisingEdge = true, bool resetCounter = true);

  private slots:
    void recvSlot();

  signals:
    void statusUpdated(statusType status, const QString &statusMessage);
    void provideTriggerdata(QVariantList args);
};

class ArduinoTrigger : public TriggerInterface {
    Q_OBJECT
  public:
    explicit ArduinoTrigger(const QString &deviceName);
    ~ArduinoTrigger();

    void enable();
    void disable();
    void pause(bool state);
    void changeSimpleSetting(const QString &setting, const QString &value);

  private:
    QThread workerThread;
    ArduinoTriggerWorker *workerClass;
    void createSettings();

  private slots:
    void settingChangedSlot(const QString &name, QList<QString> subMenus,
                            SettingsNode::nodeType type, const QString &val,
                            bool update);

  signals:
    void sendSetupSignal(int cmdDelay, int frameRate, int frameLimit,
                         bool syncRisingEdge = true, bool resetCounter = true);
};

#endif
