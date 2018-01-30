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
    void deleteByPath(const QString &path);
    void setNameByPath(const QString &path, const QString &name);

private:
    void saveChanges();
    void refresh();

    QSettings *settings;
    QList<Recording> recordings;
};

#endif // RECORDINGMODEL_HPP
