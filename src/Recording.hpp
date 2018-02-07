#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <QtCore>

class RecordingModel;

class Recording : public QObject
{
public:
    enum State {EMPTY, MODIFIED, DELETED, CURRENT};

    explicit Recording(QObject *parent = nullptr);
    Recording(QObject *parent, RecordingModel *model);
    Recording(const Recording &recording);
    Recording operator=(const Recording &recording);
    operator QString() const;
    QString getName() const;
    void setName(const QString &name, bool touch = false);
    QString getPath() const;
    void setPath(const QString &path);
    State getState() const;
    RecordingModel* getModel() const;
    void remove();

private:
    RecordingModel *model;
    QString name;
    QString path;
    State state;
};

#endif // RECORDING_HPP
