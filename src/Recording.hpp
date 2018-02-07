#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <QtCore>

class Recording : public QObject
{
    Q_OBJECT
public:
    explicit Recording(QObject *parent = nullptr);
    Recording(const Recording &recording);
    Recording operator=(const Recording &recording);
    operator QString() const;
    QString getName() const;
    void setName(const QString &name, bool touch = false);
    QString getPath() const;
    void setPath(const QString &path);
    void remove();

private:
    QString name;
    QString path;

signals:
    void modified();
    void removed(Recording *recording);
};

#endif // RECORDING_HPP
