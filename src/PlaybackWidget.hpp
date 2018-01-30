#ifndef PLAYBACKWIDGET_HPP
#define PLAYBACKWIDGET_HPP

#include <QtWidgets>

namespace Ui {
class PlaybackWidget;
}

class PlaybackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaybackWidget(QWidget *parent = 0);
    ~PlaybackWidget();
    void setLength(int seconds);
    void setPosition(int seconds);

private:
    Ui::PlaybackWidget *ui;
    int length;

signals:
    void positionChanged(int seconds);

private slots:
    void sliderMoved(int position);
};

#endif // PLAYBACKWIDGET_HPP
