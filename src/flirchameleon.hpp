/*******************************************************************************
 * File:        flirchameleon.hpp
 * Created:     05. June 2020
 * Author:      Timo Hueser
 * Contact:     timo.hueser@gmail.com
 * Copyright:   2021 Timo Hueser
 * License:     LGPL v3.0
 ******************************************************************************/

#ifndef FLIRCHAMELEON_H
#define FLIRCHAMELEON_H

#include "camerainterface.hpp"
#include "globals.hpp"

#include <QThread>
#include <QTreeWidget>

#include "SpinGenApi/SpinnakerGenApi.h"
#include "Spinnaker.h"

class FlirWorker : public AcquisitionWorker {
    Q_OBJECT
  public:
    FlirWorker(Spinnaker::CameraPtr pCam, const QString &cameraName,
               const QString &serialNumber,
               const AcquisitionSpecs &acquisitionSpecs);
    ~FlirWorker();

  public slots:
    void acquireImages();

  private:
    const Spinnaker::CameraPtr m_pCam;
    QImage m_img;
};

class FLIRChameleon : public CameraInterface {
    Q_OBJECT
  public:
    explicit FLIRChameleon(const QString &cameraName,
                           const QString &serialNumber);
    ~FLIRChameleon();
    QThread workerThread;
    bool savePreset(const QString &preset);
    bool loadPreset(const QString &preset);
    bool saveUserSetToFile(const QString &userSet, const QString &path);
    bool loadUserSetFromFile(const QString &userSet, const QString &path);
    QString getDefaultUserSet();
    bool setDefaultUserSet(const QString &userSet);
    void getSimpleSettingsValues();
    void changeSimpleSetting(const QString &setting, const QString &val);
    const QString &getDeviceID() { return m_serialNumber; }
    int getBufferUsage();
    int getBufferSize();
    bool setupCamera(const CameraSettings &cameraSettings);

  public slots:
    void settingChangedSlot(const QString &name, QList<QString> subMenus,
                            SettingsNode::nodeType type, const QString &val,
                            bool update);
    void startAcquisitionSlot(AcquisitionSpecs acquisitionSpecs);
    void stopAcquisitionSlot();
    void pauseSlot() {}
    void continueSlot() {}

  signals:
    void startAcquisition();

  private:
    void createSettings();
    void reloadUserSet();
    const QString m_serialNumber;
    const Spinnaker::SystemPtr m_camSystem;
    Spinnaker::CameraPtr m_pCam;

    int createSettingsTreeFromCam(Spinnaker::GenApi::CNodePtr node,
                                  SettingsNode *settingsNode);
    void updateSettings(Spinnaker::GenApi::INodeMap &nodeMap,
                        SettingsNode *settingsNode);

    bool openFileToRead();
    bool openFileToWrite();
    bool closeFile();
    bool executeReadCommand();
    bool executeWriteCommand();
    bool executeDeleteCommand();

  private slots:
    void streamImageSlot(QImage img);
};

#endif
