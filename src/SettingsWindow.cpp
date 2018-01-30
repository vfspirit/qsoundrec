#include "SettingsWindow.hpp"
#include "ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, QAudioRecorder *audioRecorder) :
    QDialog(parent),
    ui(new Ui::SettingsWindow),
    settings("qsoundrec")
{
    this->audioRecorder = audioRecorder;
    ui->setupUi(this);

    connect(ui->browseButton, &QPushButton::clicked, this, &SettingsWindow::browseAction);

    qualityList[QString(tr("Very Low"))] = QMultimedia::VeryLowQuality;
    qualityList[QString(tr("Low"))] = QMultimedia::LowQuality;
    qualityList[QString(tr("Normal"))] = QMultimedia::NormalQuality;
    qualityList[QString(tr("High"))] = QMultimedia::HighQuality;
    qualityList[QString(tr("Very High"))] = QMultimedia::VeryHighQuality;

    ui->encodingBox->addItems(audioRecorder->supportedAudioCodecs());
    ui->containerBox->addItems(audioRecorder->supportedContainers());
    ui->qualityBox->addItems(qualityList.keys());

    encodingQuality = (QMultimedia::EncodingQuality)settings.value("quality", QMultimedia::HighQuality).toInt();
    codec = settings.value("codec", "audio/x-vorbis").toString();
    container = settings.value("container", "audio/ogg").toString();
    audioRecorder->setAudioSettings(audioRecorderSettings);
    audioRecorder->setContainerFormat(container);
    savePath = settings.value("save_path", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).toString();

    ui->encodingBox->setCurrentText(codec);
    ui->containerBox->setCurrentText(container);
    ui->qualityBox->setCurrentText(qualityList.key(encodingQuality));
    ui->savePathLabel->setText(savePath);
    saveSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

QSettings* SettingsWindow::getSettings()
{
    return &settings;
}

QString SettingsWindow::getSavePath() const
{
    return savePath;
}

QString SettingsWindow::getCodec() const
{
    return codec;
}

QString SettingsWindow::getContainer() const
{
    return container;
}

void SettingsWindow::saveSettings()
{
    settings.setValue("quality", encodingQuality);
    settings.setValue("codec", codec);
    settings.setValue("conrainer", container);
    settings.setValue("save_path", savePath);
}

void SettingsWindow::browseAction()
{
    savePath = QFileDialog::getExistingDirectory(this, tr("Select default save directory"), savePath);
    ui->savePathLabel->setText(savePath);
}

void SettingsWindow::accept()
{
    encodingQuality = qualityList[ui->qualityBox->currentText()];
    codec = ui->encodingBox->currentText();
    saveSettings();
    setResult(Accepted);
    hide();
}

void SettingsWindow::reject()
{
    setResult(Rejected);
    hide();
}
