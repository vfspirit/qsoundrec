#include <QFile>
#include "RecordingModel.hpp"

RecordingModel::RecordingModel(QObject *parent, QSettings *settings) :
    QStandardItemModel(parent)
{
    this->settings = settings;

    int size = settings->beginReadArray("recordings");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        Recording recording;
        recording.setName(settings->value("name").toString());
        recording.setPath(settings->value("path").toString());
        recordings << recording;
    }
    settings->endArray();

    setColumnCount(1);

    refresh();
}

void RecordingModel::append(Recording *recording)
{
    recordings << *recording;
    saveChanges();
}

void RecordingModel::deleteByPath(const QString &path)
{
    for (int i = 0; i < recordings.size(); ++i) {
        if (recordings[i].getPath() == path) {
            recordings.removeAt(i);
            break;
        }
    }

    QFile file(path);
    file.remove();

    saveChanges();
}

void RecordingModel::setNameByPath(const QString &path, const QString &name)
{
    for (int i = 0; i < recordings.size(); ++i) {
        if (recordings[i].getPath() == path) {
            recordings[i].setName(name);
            break;
        }
    }

    saveChanges();
}

void RecordingModel::saveChanges()
{
    settings->beginWriteArray("recordings", recordings.size());
    for (int i = 0; i < recordings.size(); ++i) {
        settings->setArrayIndex(i);
        settings->setValue("name", recordings[i].getName());
        settings->setValue("path", recordings[i].getPath());
    }
    settings->endArray();
    refresh();
}

void RecordingModel::refresh()
{
    setRowCount(recordings.size());
    for (int i = 0; i < recordings.size(); ++i) {
        setData(index(i, 0), QString(recordings[i]), Qt::DisplayRole);
        setData(index(i, 0), QVariant::fromValue(recordings[i]), Qt::UserRole);
    }
}
