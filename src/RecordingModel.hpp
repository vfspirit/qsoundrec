#ifndef RECORDINGMODEL_HPP
#define RECORDINGMODEL_HPP

#include <QtCore>
#include <QtWidgets>
#include "Recording.hpp"

class RecordingModel : public QStandardItemModel
{
    Q_OBJECT
public:
    RecordingModel(QObject *parent, QSettings *settings);
    void append(Recording *recording);

private:
    void refresh();

    QSettings *settings;
    QList<Recording *> recordings;

private slots:
    void saveChanges();
    void recordingRemoved(Recording *recording);
};

#endif // RECORDINGMODEL_HPP
