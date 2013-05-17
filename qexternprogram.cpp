#include "qexternprogram.h"

QExternProgram::QExternProgram(QString name,QString icon,QString command, image *imagewidget)
{
    this->name = name;
    this->icon = icon;
    this->command = command;
    this->imagewidget = imagewidget;
}

void QExternProgram::exec()
{
    QProcess *vec = new QProcess;
    QString start = command + " " + imagewidget->currentImageName();
    vec->startDetached(start);
}
