#ifndef QEXTERNPROGRAM_H
#define QEXTERNPROGRAM_H

#include <QObject>
#include <QDebug>
#include "image.h"

class QExternProgram : public QObject
{
    Q_OBJECT
public:
    explicit QExternProgram(QString name,QIcon icon,QString command,image * imagewidget);
    QString name,command;
    QIcon icon;
    image *imagewidget;
public slots:
    void exec();
};

#endif // QEXTERNPROGRAM_H
