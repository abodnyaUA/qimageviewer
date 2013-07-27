#ifndef QHIDDENBUTTON_H
#define QHIDDENBUTTON_H

#include <QPushButton>
#include <QGraphicsOpacityEffect>

class QHiddenButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QHiddenButton(QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    QGraphicsOpacityEffect *opacityEffect;
};

#endif // QHIDDENBUTTON_H
