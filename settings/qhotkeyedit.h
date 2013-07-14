#ifndef HOTKEYEDIT_H
#define HOTKEYEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QDebug>
#include <QRegExpValidator>

class QHotkeyEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QHotkeyEdit(QWidget *parent = 0);
    explicit QHotkeyEdit(QString text = "", QWidget *parent = 0);
    
signals:
    void needDecline();
    void needAccept();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    
};

#endif // HOTKEYEDIT_H
