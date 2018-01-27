#include <QFileInfo>
#include "Recording.hpp"

Recording::Recording(QObject *parent) :
    QObject(parent)
{
    empty = true;
}

Recording::Recording(const Recording &recording) :
    QObject(recording.parent())
{
    this->empty = recording.isEmpty();
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

void Recording::setName(const QString &name)
{
    this->name = name;
}

QString Recording::getPath() const
{
    return path;
}

void Recording::setPath(const QString &path)
{
    this->path = path;
}

bool Recording::isEmpty() const
{
    return empty;
}

void Recording::setEmpty(bool empty)
{
    this->empty = empty;
}
