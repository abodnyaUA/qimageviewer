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
    QString filename = imagewidget->currentImageName();
#ifdef Q_OS_WIN32
    for (int i=1;i<filename.size();i++)
        if (filename[i] == '/')
        {
            filename.remove(i,1);
            filename.insert(i,"\\");
        }
#endif
    QString start = '"' + command + '"' + " " + '"' + filename + '"';
    qDebug() << start;
    vec->startDetached(start);
}
