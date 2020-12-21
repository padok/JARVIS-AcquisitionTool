/*------------------------------------------------------------
 *  connectionwidget.cpp
 *  Created:  27. October 2020
 *  Author:   Timo Hüser
 *------------------------------------------------------------*/

#include "connectionwidget.hpp"
#include "camerainterface.hpp"
#include "testcamera.hpp"
#ifdef FLIR_CHAMELEON
#include "flirchameleon.hpp"
#endif
#include "triggerinterface.hpp"
#include "testtrigger.hpp"

#include <QLineEdit>

ConnectionWidget::ConnectionWidget(QWidget *parent) : QWidget(parent, Qt::Window) {
	settings = new QSettings();
	this->setFocusPolicy(Qt::StrongFocus);
	camLoadPresetsWindow = new PresetsWindow(&camPresets, "load", "connectionMode/camPresets");
	camSavePresetsWindow = new PresetsWindow(&camPresets, "save", "connectionMode/camPresets");
	connect(camLoadPresetsWindow, SIGNAL(loadPreset(QString)), this, SLOT(camLoadPresetSlot(QString)));
	connect(camSavePresetsWindow, SIGNAL(savePreset(QString)), this, SLOT(camSavePresetSlot(QString)));
	triggerLoadPresetsWindow = new PresetsWindow(&triggerPresets, "load", "connectionMode/triggerPresets");
	triggerSavePresetsWindow = new PresetsWindow(&triggerPresets, "save", "connectionMode/triggerPresets");
	connect(triggerLoadPresetsWindow, SIGNAL(loadPreset(QString)), this, SLOT(triggerLoadPresetSlot(QString)));
	connect(triggerSavePresetsWindow, SIGNAL(savePreset(QString)), this, SLOT(triggerSavePresetSlot(QString)));

	QGridLayout *layout = new QGridLayout(this);

	//Camera Widget
	camWidget = new QWidget(this);
	QGridLayout *camlayout = new QGridLayout(camWidget);
	camPanelWidget = new QWidget(camWidget);
	camPanelWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	campanellayout = new QGridLayout(camPanelWidget);
	campanellayout->setContentsMargins(0, 11, 0, 11);
	camToolBar = new QToolBar(camWidget);
	camToolBar->setFixedHeight(45);
	camToolBar->setIconSize(QSize(35,35));
	QLabel *camLabel = new QLabel("Cameras", camToolBar);
	camLabel->setFont(fonts["big"]);
	QWidget *spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	camSavePresetButton = new QToolButton(camToolBar);
	camSavePresetAction = new QAction(camToolBar);
	createToolBarButton(camSavePresetButton, camSavePresetAction, QIcon("icons/upload.png"), true,
											false, QSize(40,40));
	connect(camSavePresetAction, &QAction::triggered, this, &ConnectionWidget::camSavePresetsClickedSlot);
	camLoadPresetButton = new QToolButton(camToolBar);
	camLoadPresetAction = new QAction(camToolBar);
	createToolBarButton(camLoadPresetButton, camLoadPresetAction, QIcon("icons/download.png"), true,
											false, QSize(40,40));
	connect(camLoadPresetAction, &QAction::triggered, this, &ConnectionWidget::camLoadPresetsClickedSlot);
	camToolBar->addWidget(camLabel);
	camToolBar->addWidget(spacer);
	camToolBar->addWidget(camSavePresetButton);
	camToolBar->addWidget(camLoadPresetButton);
	for (int i = 0; i < NUM_CAMS; i++) {
		camPanels[i] = new CamConnectionPanel(this);
		campanellayout->addWidget(camPanels[i], i/(NUM_CAMS/3), i%(NUM_CAMS/3));
		connect(camPanels[i], SIGNAL(camListChanged()), this, SLOT(camListChangedSlot()));
		connect(camPanels[i], SIGNAL(camAdded(CameraInterface *)), this, SLOT(camAddedSlot(CameraInterface *)));
		connect(camPanels[i], SIGNAL(statusUpdated(CameraInterface *, statusType, QString)), this, SLOT(statusUpdatedSlot(CameraInterface *, statusType, QString)));
	}
	camlayout->addWidget(camToolBar,0,0);
	camlayout->addWidget(camPanelWidget,1,0);
	camlayout->setSpacing(0);
	camlayout->setContentsMargins(0, 0, 11, 0);

	//Trigger Widget
	triggerWidget = new QWidget(this);
	triggerWidget->setMaximumSize(350,1000);
	QGridLayout *triggerlayout = new QGridLayout(triggerWidget);
	triggerPanelWidget = new QWidget(triggerWidget);
	triggerpanellayout = new QGridLayout(triggerPanelWidget);
	triggerpanellayout->setContentsMargins(0, 11, 0, 11);
	triggerToolBar = new QToolBar(triggerWidget);
	triggerToolBar->setFixedHeight(45);
	triggerToolBar->setIconSize(QSize(35,35));
	QLabel *triggerLabel = new QLabel("Trigger", triggerToolBar);
	triggerLabel->setFont(fonts["big"]);
	QWidget *triggerSpacer = new QWidget();
	triggerSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	triggerSavePresetButton = new QToolButton(triggerToolBar);
	triggerSavePresetAction = new QAction(triggerToolBar);
	createToolBarButton(triggerSavePresetButton, triggerSavePresetAction, QIcon("icons/upload.png"), true,
											false, QSize(40,40));
	connect(triggerSavePresetAction, &QAction::triggered, this, &ConnectionWidget::triggerSavePresetsClickedSlot);
	triggerLoadPresetButton = new QToolButton(triggerToolBar);
	triggerLoadPresetAction = new QAction(triggerToolBar);
	createToolBarButton(triggerLoadPresetButton, triggerLoadPresetAction, QIcon("icons/download.png"), true,
											false, QSize(40,40));
	connect(triggerLoadPresetAction, &QAction::triggered, this, &ConnectionWidget::triggerLoadPresetsClickedSlot);
	triggerToolBar->addWidget(triggerLabel);
	triggerToolBar->addWidget(triggerSpacer);
	triggerToolBar->addWidget(triggerSavePresetButton);
	triggerToolBar->addWidget(triggerLoadPresetButton);
	triggerPanel = new TriggerConnectionPanel(this);
	connect(triggerPanel, SIGNAL(triggerInstanceChanged()), this, SLOT(triggerInstanceChangedSlot()));
	triggerpanellayout->addWidget(triggerPanel,0, 0);

	triggerlayout->addWidget(triggerToolBar,0,0);
	triggerlayout->addWidget(triggerPanelWidget,1,0);
	triggerlayout->setSpacing(0);
	triggerlayout->setContentsMargins(11, 0, 0, 0);

	layout->addWidget(camWidget,0,0);
	layout->addWidget(triggerWidget,0,1);
	layout->setSpacing(0);
}


void ConnectionWidget::camListChangedSlot() {
	emit camListChanged();
}

void ConnectionWidget::camAddedSlot(CameraInterface * cam) {
	emit camAdded(cam);
}

void ConnectionWidget::triggerInstanceChangedSlot() {
	emit triggerInstanceChanged();
}

void ConnectionWidget::statusUpdatedSlot(CameraInterface* cam, statusType status, QString statusMsg) {
	emit statusUpdated(cam,status,statusMsg);
}


void ConnectionWidget::camLoadPresetsClickedSlot() {
	camLoadPresetsWindow->updateListSlot();
	camLoadPresetsWindow->show();
}


void ConnectionWidget::camSavePresetsClickedSlot() {
	camSavePresetsWindow->updateListSlot();
	camSavePresetsWindow->show();
}


void ConnectionWidget::camLoadPresetSlot(QString preset) {
	settings->beginGroup(preset);
	for (int i = 0; i < NUM_CAMS; i++) {
		delete camPanels[i];
		camPanels[i] = new CamConnectionPanel(this);
		campanellayout->addWidget(camPanels[i], i/(NUM_CAMS/3), i%(NUM_CAMS/3));
		connect(camPanels[i], SIGNAL(camListChanged()), this, SLOT(camListChangedSlot()));
		connect(camPanels[i], SIGNAL(camAdded(CameraInterface *)), this, SLOT(camAddedSlot(CameraInterface *)));
		connect(camPanels[i], SIGNAL(statusUpdated(CameraInterface *, statusType, QString)), this, SLOT(statusUpdatedSlot(CameraInterface *, statusType, QString)));
		settings->beginGroup("Panel_" + QString::number(i));
		if (settings->value("isConfigured").toBool()) {
			camPanels[i]->example1Info->setText(settings->value("example1Info").toString());
			camPanels[i]->example2Info->setText(settings->value("example2Info").toString());
			camPanels[i]->camTypeCombo->setCurrentText(settings->value("cameraType").toString());
			QString cameraName = settings->value("cameraName").toString();
			if (camPanels[i]->camTypeCombo->currentText() == "Test Camera") {
				camPanels[i]->camera = new TestCamera(cameraName, camPanels[i]->example1Info->text(), camPanels[i]->example2Info->text());
				connect(camPanels[i]->camera, SIGNAL(statusUpdated(statusType, QString)), camPanels[i], SLOT(statusUpdatedSlot(statusType, QString)));
				camPanels[i]->infoToolBarLabel->setText(cameraName);
				CameraInterface::cameraList.append(camPanels[i]->camera);
			}
			#ifdef FLIR_CHAMELEON
			else if (camPanels[i]->camTypeCombo->currentText() == "FLIR Chameleon") {
				camPanels[i]->camIDInfo->setText(settings->value("camIDInfo").toString());
				camPanels[i]->camera = new FLIRChameleon(cameraName, camPanels[i]->camIDInfo->text());
				connect(camPanels[i]->camera, SIGNAL(statusUpdated(statusType, QString)), camPanels[i], SLOT(statusUpdatedSlot(statusType, QString)));
				camPanels[i]->infoToolBarLabel->setText(cameraName);
				CameraInterface::cameraList.append(camPanels[i]->camera);
			}
			#endif
			statusLog statusLog;
			statusLog.type = Connecting;
			statusLog.time = new QTime(0,0);
			statusLog.time->restart();
			statusLog.message = "";
			camPanels[i]->statusLogWindow->statusLogList.push_back(statusLog);
			camPanels[i]->stackWidget->setCurrentIndex(2);
			emit camAdded(camPanels[i]->camera);
		}
		settings->endGroup();
	}
	emit camListChanged();
	settings->endGroup();
}


void ConnectionWidget::camSavePresetSlot(QString preset) {
	std::cout << "Saving Preset" << std::endl;
	settings->beginGroup(preset);
	for (int i = 0; i < NUM_CAMS; i++) {
		settings->beginGroup("Panel_" + QString::number(i));
		settings->setValue("isConfigured", camPanels[i]->stackWidget->currentIndex() != 0);
		settings->setValue("camIDInfo", camPanels[i]->camIDInfo->text());
		settings->setValue("example1Info", camPanels[i]->example1Info->text());
		settings->setValue("example2Info", camPanels[i]->example2Info->text());
		settings->setValue("cameraName", camPanels[i]->infoToolBarLabel->text());
		settings->setValue("cameraType", camPanels[i]->camTypeCombo->currentText());
		settings->endGroup();
	}
	settings->endGroup();
}

void ConnectionWidget::triggerLoadPresetsClickedSlot() {
	triggerLoadPresetsWindow->updateListSlot();
	triggerLoadPresetsWindow->show();
}


void ConnectionWidget::triggerSavePresetsClickedSlot() {
	triggerSavePresetsWindow->updateListSlot();
	triggerSavePresetsWindow->show();
}


void ConnectionWidget::triggerLoadPresetSlot(QString preset) {
	settings->beginGroup(preset);
	delete triggerPanel;
	triggerPanel = new TriggerConnectionPanel(this);
	triggerpanellayout->addWidget(triggerPanel, 0, 0);
	connect(triggerPanel, SIGNAL(triggerInstanceChanged()), this, SLOT(triggerInstanceChangedSlot()));
	if (settings->value("isConfigured").toBool()) {
		triggerPanel->example1Info->setText(settings->value("example1Info").toString());
		triggerPanel->example2Info->setText(settings->value("example2Info").toString());
		triggerPanel->triggerTypeCombo->setCurrentText(settings->value("triggerType").toString());
		if (triggerPanel->triggerTypeCombo->currentText() == "Test Trigger") {
			TriggerInterface::triggerInstance = new TestTrigger(triggerPanel->example1Info->text().toInt(), triggerPanel->example2Info->text());
			connect(TriggerInterface::triggerInstance, SIGNAL(statusUpdated(statusType, QString)), triggerPanel, SLOT(statusUpdatedSlot(statusType, QString)));
		}
		statusLog statusLog;
		statusLog.type = Connecting;
		statusLog.time = new QTime(0,0);
		statusLog.time->restart();
		statusLog.message = "";
		triggerPanel->statusLogWindow->statusLogList.push_back(statusLog);
		triggerPanel->stackWidget->setCurrentIndex(2);
		emit triggerInstanceChanged();
	}
	settings->endGroup();
}


void ConnectionWidget::triggerSavePresetSlot(QString preset) {
	settings->beginGroup(preset);
	settings->setValue("isConfigured", triggerPanel->stackWidget->currentIndex() != 0);
	settings->setValue("example1Info", triggerPanel->example1Info->text());
	settings->setValue("example2Info", triggerPanel->example2Info->text());
	settings->setValue("triggerType", triggerPanel->triggerTypeCombo->currentText());
	settings->endGroup();
}
