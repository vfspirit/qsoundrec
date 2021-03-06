#include "Recording.hpp"

Recording::Recording(QObject *parent) :
    QObject(parent)
{

}

Recording::Recording(const Recording &recording) :
    QObject(recording.parent())
{
    this->name = recording.getName();
    this->path = recording.getPath();

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
        emit modified();
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

void Recording::remove()
{
    emit removed(this);
}
