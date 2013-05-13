#ifndef QHOTKEYWIDGET_H
#define QHOTKEYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "qhotkeyedit.h"
#include <QLayout>
#include <QTreeWidget>
#include <QCloseEvent>

class qhotkeywidget : public QWidget
{
    Q_OBJECT
public:
    explicit qhotkeywidget(QWidget *parent = 0);
    QTreeWidgetItem *changingItem;
    QHotkeyEdit * hotkeyEdit;
    bool isChanging;
    void loadItem(QTreeWidgetItem *item);
    
private:
    QString defaulthotkey;
    QPushButton * buttOK;
    QPushButton * buttCancel;
    QPushButton * buttDefault;
    QHBoxLayout * buttLayout;
    QVBoxLayout * changehotkeyLayout;
    
signals:
    void butOKClicked();
    void butCancelClicked();

private slots:
    void buttDefaultClicked();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // QHOTKEYWIDGET_H
