#ifndef SETTINGSWINDOW_HPP
#define SETTINGSWINDOW_HPP

#include <QDialog>
#include <QSettings>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <QMap>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent, QAudioRecorder *audioRecorder);
    ~SettingsWindow();
    QSettings* getSettings();
    QString getSavePath() const;
    QString getCodec() const;
    QString getContainer() const;

private:
    void saveSettings();

    Ui::SettingsWindow *ui;
    QSettings settings;
    QAudioRecorder *audioRecorder;
    QAudioEncoderSettings audioRecorderSettings;
    QMap<QString, QMultimedia::EncodingQuality> qualityList;

    QMultimedia::EncodingQuality encodingQuality;
    QString codec;
    QString container;
    QString savePath;

public slots:
    void browseAction();

private slots:
    void accept();
    void reject();
};

#endif // SETTINGSWINDOW_HPP
