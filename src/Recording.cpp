#include "Recording.hpp"

Recording::Recording(QObject *parent) :
    QObject(parent)
{
    state = EMPTY;
}

Recording::Recording(QObject *parent, RecordingModel *model) :
    QObject(parent)
{
    this->model = model;
    state = EMPTY;
}

Recording::Recording(const Recording &recording) :
    QObject(recording.parent())
{
    this->name = recording.getName();
    this->path = recording.getPath();
    this->state = recording.getState();

}

Recording Recording::operator=(const Recording &recording)
{
    return Recording(recording);
}

Recording::operator QString() const
{
    if (!name.isEmpty()) {
        return name;
    } else {
        QFileInfo fileInfo(path);
        return fileInfo.fileName();
    }
}

QString Recording::getName() const
{
    return name;
}

void Recording::setName(const QString &name, bool touch)
{
    this->name = name;

    if (touch) {
        state = MODIFIED;
    }
}

QString Recording::getPath() const
{
    return path;
}

void Recording::setPath(const QString &path)
{
    this->path = path;
}

Recording::State Recording::getState() const
{
    return state;
}

RecordingModel* Recording::getModel() const
{
    return model;
}

void Recording::remove()
{
    state = DELETED;
}
