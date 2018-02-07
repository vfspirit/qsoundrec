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
    void saveChanges();

private:
    void refresh();

    QSettings *settings;
    QList<Recording *> recordings;
};

#endif // RECORDINGMODEL_HPP
