#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QAudioRecorder>
#include <QMediaPlayer>
#include "PlaybackWidget.hpp"
#include "SettingsWindow.hpp"
#include "RecordingModel.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PlaybackWidget playbackWidget;
    QAudioRecorder audioRecorder;
    QMediaPlayer mediaPlayer;
    SettingsWindow *settingsWindow;
    RecordingModel *model;
    Recording *currentRecording;

    enum PlaybackState {
        READY, PLAYBACK, PLAYBACK_PAUSED, RECORDING, RECORDING_PAUSED
    };

    PlaybackState currentState;

    void updateState(PlaybackState state);

public slots:
    void actionRecord();
    void actionPlay();
    void actionPause();
    void actionStop();
    void actionExit();
    void actionEdit();
    void actionDelete();
    void actionSettings();
    void actionAbout();
    void aboutToQuit();

private slots:
    void durationChanged(qint64 duration);
    void playbackPositionChanged(qint64 position);
    void playbackStatusChanged(QMediaPlayer::MediaStatus status);
    void playbackSliderMoved(int position);
};

#endif // MAINWINDOW_HPP
