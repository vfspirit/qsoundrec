#include <QtCore>
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    playbackWidget(this)
{
    ui->setupUi(this);
    connect(ui->actionRecord, &QAction::triggered, this, &MainWindow::actionRecord);
    connect(ui->actionMenuRecord, &QAction::triggered, this, &MainWindow::actionRecord);
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::actionStop);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::actionExit);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::actionSettings);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::actionDelete);
    connect(ui->actionMenuDelete, &QAction::triggered, this, &MainWindow::actionDelete);
    connect(ui->actionRename, &QAction::triggered, this, &MainWindow::actionEdit);
    connect(ui->actionMenuRename, &QAction::triggered, this, &MainWindow::actionEdit);
    connect(ui->actionPlay, &QAction::triggered, this, &MainWindow::actionPlay);
    connect(ui->actionPause, &QAction::triggered, this, &MainWindow::actionPause);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::actionAbout);
    connect(&audioRecorder, &QAudioRecorder::durationChanged, this, &MainWindow::durationChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::playbackPositionChanged);
    connect(&mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::playbackStatusChanged);
    connect(&playbackWidget, &PlaybackWidget::positionChanged, this, &MainWindow::playbackSliderMoved);
    currentRecording = nullptr;

    settingsWindow = new SettingsWindow(this, &audioRecorder);
    model = new RecordingModel(this, settingsWindow->getSettings());
    ui->listView->setModel(model);
    playbackWidget.setHidden(true);
    ui->playbackWidgetContainer->addWidget(&playbackWidget);

    updateState(READY);
}

MainWindow::~MainWindow()
{
    delete model;
    delete settingsWindow;
    delete ui;
}

void MainWindow::updateState(PlaybackState state)
{
    currentState = state;
    ui->actionRecord->setEnabled(state == READY || state == RECORDING_PAUSED);
    ui->actionPause->setEnabled(state == PLAYBACK || state == RECORDING);
    ui->actionStop->setEnabled(state == PLAYBACK || state == PLAYBACK_PAUSED || state == RECORDING || state == RECORDING_PAUSED);
    ui->actionPlay->setEnabled(model->rowCount() > 0 && (state == READY || state == PLAYBACK_PAUSED));
    ui->actionMenuRecord->setEnabled(ui->actionRecord->isEnabled());
    ui->actionRename->setEnabled(model->rowCount() > 0 && state == READY);
    ui->actionDelete->setEnabled(model->rowCount() > 0 && state == READY);

    switch (state) {
    case READY:
        ui->statusBar->showMessage(tr("Ready."));
        playbackWidget.setHidden(true);
        break;
    case PLAYBACK:
        playbackWidget.setHidden(false);
        break;
    case PLAYBACK_PAUSED:
        ui->statusBar->showMessage(tr("Playback: Paused"));
        break;
    case RECORDING:
        ui->statusBar->showMessage(tr("Recording..."));
        break;
    case RECORDING_PAUSED:
        ui->statusBar->showMessage(tr("Recording: Paused"));
        break;
    }
}

void MainWindow::actionRecord()
{
    if (currentState == READY) {
        currentRecording = new Recording();

        QDir dir;
        if (!dir.exists(settingsWindow->getSavePath())) {
            dir.mkpath(settingsWindow->getSavePath());
        }

        currentRecording->setPath(settingsWindow->getSavePath() + "/recording_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));
        audioRecorder.setOutputLocation(QUrl::fromLocalFile(currentRecording->getPath()));
    }

    audioRecorder.record();
    updateState(RECORDING);
}

void MainWindow::actionPlay()
{
    if (currentState == READY) {
        Recording recording = qvariant_cast<Recording>(ui->listView->currentIndex().data(Qt::UserRole));
        mediaPlayer.setMedia(QUrl::fromLocalFile(recording.getPath()));
    }

    mediaPlayer.play();
    updateState(PLAYBACK);
}

void MainWindow::actionPause()
{
    if (currentState == PLAYBACK) {
        mediaPlayer.pause();
        updateState(PLAYBACK_PAUSED);
    } else if (currentState == RECORDING) {
        audioRecorder.pause();
        updateState(RECORDING_PAUSED);
    }
}

void MainWindow::actionStop()
{
    if (currentState == RECORDING || currentState == RECORDING_PAUSED) {
        // stop recording
        audioRecorder.stop();

        // apped file extension
        QMimeDatabase mimeDatabase;
        QMimeType mimeType = mimeDatabase.mimeTypeForFile(currentRecording->getPath());
        QFile file(currentRecording->getPath());
        QString newPath = currentRecording->getPath() + "." + mimeType.suffixes().first();
        file.rename(currentRecording->getPath(), newPath);
        currentRecording->setPath(newPath);

        model->append(currentRecording);
        delete currentRecording;
    } else if (currentState == PLAYBACK || currentState == PLAYBACK_PAUSED) {
        mediaPlayer.stop();
    }

    updateState(READY);
}

void MainWindow::actionExit()
{
    QCoreApplication::exit();
}

void MainWindow::actionEdit()
{
    Recording recording = qvariant_cast<Recording>(ui->listView->currentIndex().data(Qt::UserRole));
    bool ok;
    QString name = QInputDialog::getText(this, tr("Rename"), tr("New name for recording:"), QLineEdit::Normal, recording, &ok);
    if (ok) {
        model->setNameByPath(recording.getPath(), name);
    }
}

void MainWindow::actionDelete()
{
    QString selectedName = ui->listView->currentIndex().data().toString();
    QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Confirm deletion"), QString::asprintf(tr("Are you sure you want to delete the following: %s?").toStdString().c_str(), selectedName.toStdString().c_str()), QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::Yes) {
        Recording recording = qvariant_cast<Recording>(ui->listView->currentIndex().data(Qt::UserRole));
        model->deleteByPath(recording.getPath());
        updateState(currentState);
    }
}

void MainWindow::actionSettings()
{
    settingsWindow->show();
}

void MainWindow::actionAbout()
{
    QMessageBox::about(this, tr("Sound Recorder - About"), tr("Copyright 2018, vfspirit.\nLicensed under the terms of the 3-Clause BSD License."));
}

void MainWindow::aboutToQuit()
{
    if (currentState != READY) {
        actionStop();
    }
}

void MainWindow::durationChanged(qint64 duration)
{
    long secs = duration / 1000;
    statusBar()->showMessage(QString::asprintf(tr("Recording: %02ld:%02ld").toStdString().c_str(), secs / 60, secs % 60));
}

void MainWindow::playbackPositionChanged(qint64 position)
{
    long secs = position / 1000;
    statusBar()->showMessage(QString::asprintf(tr("Playback: %02ld:%02ld").toStdString().c_str(), secs / 60, secs % 60));
    playbackWidget.setLength(mediaPlayer.duration() / 1000);
    playbackWidget.setPosition(secs);
}

void MainWindow::playbackStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        updateState(READY);
    }
}

void MainWindow::playbackSliderMoved(int position)
{
    mediaPlayer.setPosition(position * 1000);
}
