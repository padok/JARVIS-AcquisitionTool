/*------------------------------------------------------------
 *  testcamera.cpp
 *  Created: 05. June 2020
 *  Author:   Timo Hüser
 *------------------------------------------------------------*/

#include "testcamera.hpp"

#include <QApplication>
#include <QTreeWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>

TestAcquisitionWorker::TestAcquisitionWorker(const QString& cameraName, const AcquisitionSpecs& acquisitionSpecs, QImage **testimgs)
			: AcquisitionWorker{cameraName, acquisitionSpecs}, m_testimgs{testimgs}  { }

void TestAcquisitionWorker::acquireImages() {
	forever {
		QImage img = m_recordingInterface->recordFrame((*m_testimgs[m_frameCount%5]).bits());
		emit streamImage(img);
		m_frameCount ++;
		delayl(500);
		if (QThread::currentThread()->isInterruptionRequested()) return;
	}
}


TestCamera::TestCamera(QString cameraName, const QString& basepath, const QString&)
			: CameraInterface(testCamera, cameraName) {
	createSettings();
	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &TestCamera::testStatusReadySlot);
	timer->setSingleShot(true);
	timer->start(2000);
	QTimer *timer2 = new QTimer(this);
	connect(timer2, &QTimer::timeout, this, &TestCamera::testStatusWarningSlot);
	timer2->setSingleShot(false);
	timer2->start(5000);

	for (int i = 0; i < 5; i++) {
		testimgs[i] = new QImage(basepath + "/frame" + QString::number(i*8+31) + ".png");
		*testimgs[i] = testimgs[i]->convertToFormat(QImage::Format_RGB888);
		//testimgs[i] = new QImage("../Imgs/Cam0_200/Acquisition-" + QString::number(i*4) + ".jpg");
	}
}

TestCamera::~TestCamera() {
	delete (m_cameraSettings);
}


void TestCamera::settingChangedSlot(const QString& name, QList<QString>,
			SettingsNode::nodeType, const QString& val, bool) {
	std::cout << name.toStdString() << " " << val.toStdString() << std::endl;
}


void TestCamera::startAcquisitionSlot(AcquisitionSpecs acquisitionSpecs) {
	if (!m_isStreaming) {
		acquisitionSpecs.frameSize.width = 1280;
		acquisitionSpecs.frameSize.height = 1024;
		m_acquisitionWorker = new TestAcquisitionWorker(m_cameraName, acquisitionSpecs,testimgs);
		m_acquisitionWorker->moveToThread(&workerThread);
		connect(this, &TestCamera::startAcquisition, m_acquisitionWorker, &AcquisitionWorker::acquireImages);
		connect(m_acquisitionWorker, &AcquisitionWorker::streamImage, this, &TestCamera::streamImageSlot);
		connect(&workerThread, &QThread::finished, m_acquisitionWorker, &AcquisitionWorker::deleteLater);
		workerThread.start();
		emit startAcquisition();
		emit statusUpdated(Streaming, "");
		m_isStreaming = true;
	}
}


void TestCamera::stopAcquisitionSlot() {
	if (m_isStreaming) {
		workerThread.requestInterruption();
		workerThread.quit();
		workerThread.wait();
		m_isStreaming = false;
		emit statusUpdated(Ready, "Stopped Streaming");
	}
}


void TestCamera::createSettings() {
	m_cameraSettingsRootNode = new rootNode("Test Camera");

	categoryNode *analogControlNode = new categoryNode(m_cameraSettingsRootNode, "AnalogControl");
	analogControlNode->setDescription("Analog Control, does something probably");
	stringNode *gainSelectorNode = new stringNode(analogControlNode, "GainSelector");
	gainSelectorNode->setDescription("Select the Gain, as a String of course");
	gainSelectorNode->setValue("test");
	intNode *gainNode = new intNode(analogControlNode, "Gain");
	gainNode->setDescription("Gain, this acutally has the right type");
	floatNode *whiteBalanceNode = new floatNode(analogControlNode, "WhiteBalance");
	whiteBalanceNode->setDescription("Test Text");
	whiteBalanceNode->setDefaultValue(9.8);
	whiteBalanceNode->setRange(0,20);
	enumNode *enumTestNode = new enumNode(analogControlNode, "enumTest");
	enumItemNode *testItem1Node = new enumItemNode(enumTestNode, "Test 1");
	enumItemNode *testItem2Node = new enumItemNode(enumTestNode, "Test 2");
	boolNode *boolTestNode = new boolNode(analogControlNode, "boolTest");
	actionNode *actionTestNode = new actionNode(analogControlNode, "actionTest");
	enumTestNode->addItem(testItem1Node);
	enumTestNode->addItem(testItem2Node);

	analogControlNode->addChild(gainSelectorNode);
	analogControlNode->addChild(gainNode);
	analogControlNode->addChild(whiteBalanceNode);
	analogControlNode->addChild(enumTestNode);
	analogControlNode->addChild(boolTestNode);
	analogControlNode->addChild(actionTestNode);

	categoryNode *deviceControlNode = new categoryNode(m_cameraSettingsRootNode, "DeviceControl");
	m_cameraSettingsRootNode->addChild(analogControlNode);
	m_cameraSettingsRootNode->addChild(deviceControlNode);

	m_cameraSettings = new settingsObject(this, "Camera Settings", m_cameraSettingsRootNode);
}


void TestCamera::testStatusReadySlot() {
	emit statusUpdated(Ready,"");
}


void TestCamera::testStatusWarningSlot() {
	emit statusUpdated(Warning,"This is a test Warning");
}


void TestCamera::testStatusErrorSlot() {
	emit statusUpdated(Error,"This is a test Error");
}


void TestCamera::streamImageSlot(QImage img) {
	emit streamImage(img);
}
