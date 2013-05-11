#ifndef HOTKEYEDIT_H
#define HOTKEYEDIT_H

#include <QLineEdit>

class QHotkeyEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QHotkeyEdit(QWidget *parent = 0);
    
signals:
    
public slots:

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    
};

#endif // HOTKEYEDIT_H
