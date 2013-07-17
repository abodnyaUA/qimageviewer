#ifndef OS_APPLICATION_H
#define OS_APPLICATION_H
#include <QMessageBox>
#include <QApplication>
#include <QtGui>
#include <QFileOpenEvent>
#include "qimageviewer.h"
//#define osApplication \
//    (static_cast<OS_application*>(QCoreApplication::instance()))

class OS_application : public QApplication
{
    Q_OBJECT
public:
    OS_application( int &argc, char **argv );
    void setWindow(QImageViewer *window);
protected:
    bool event(QEvent *ev);
public:
    QString file();
private:
    QString osfile;
    QImageViewer *window;
};
#endif // OS_APPLICATION_H
