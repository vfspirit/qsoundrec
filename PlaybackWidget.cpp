#include "PlaybackWidget.hpp"
#include "ui_PlaybackWidget.h"

PlaybackWidget::PlaybackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaybackWidget)
{
    ui->setupUi(this);
    connect(ui->playbackSlider, &QSlider::sliderMoved, this, &PlaybackWidget::sliderMoved);
}

PlaybackWidget::~PlaybackWidget()
{
    delete ui;
}

void PlaybackWidget::setLength(int seconds)
{
    ui->playbackSlider->setMaximum(seconds);
    this->length = seconds;
    ui->playbackLabel->setText(QString::asprintf("0:00 / %d:%02d", length / 60, length % 60));
}

void PlaybackWidget::setPosition(int seconds)
{
    ui->playbackSlider->setValue(seconds);
    ui->playbackLabel->setText(QString::asprintf("%d:%02d / %d:%02d", seconds / 60, seconds % 60, length / 60, length % 60));
}

void PlaybackWidget::sliderMoved(int position)
{
    setPosition(position);
    emit positionChanged(position);;
}
