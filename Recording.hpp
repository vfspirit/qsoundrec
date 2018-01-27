#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <QObject>
#include <QString>

class Recording : public QObject
{
public:
    explicit Recording(QObject *parent = nullptr);
    Recording(const Recording &recording);
    Recording operator=(const Recording &recording);
    operator QString() const;
    QString getName() const;
    void setName(const QString &name);
    QString getPath() const;
    void setPath(const QString &path);
    bool isEmpty() const;
    void setEmpty(bool empty);

private:
    QString name;
    QString path;
    bool empty;
};
Q_DECLARE_METATYPE(Recording)

#endif // RECORDING_HPP
