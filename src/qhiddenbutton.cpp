#include "qhiddenbutton.h"
#include <QDebug>

QHiddenButton::QHiddenButton(QWidget *parent) :
    QPushButton(parent)
{
    opacityEffect = new QGraphicsOpacityEffect();
    opacityEffect->setOpacity(0);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMaximumWidth(50);
    this->setGraphicsEffect(opacityEffect);
}

void QHiddenButton::enterEvent(QEvent *event)
{
    opacityEffect->setOpacity(0.7);
}

void QHiddenButton::leaveEvent(QEvent *event)
{
    opacityEffect->setOpacity(0);
}
