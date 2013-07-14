#include "qexternprogram.h"

QExternProgram::QExternProgram(QString name,QIcon icon,QString command, image *imagewidget)
{
    this->name = name;
    this->icon = icon;
    this->command = command;
    this->imagewidget = imagewidget;
}

void QExternProgram::exec()
{
    QProcess *vec = new QProcess;
#ifdef Q_OS_LINUX
    QString prog = command;
    QString start;
    if (command.endsWith(".exe") || prog.endsWith(".bat") || prog.endsWith(".msn"))
    {
        start = QString("wine ") + '"' + command +'"' + " " +
            '"' + imagewidget->currentImageName() + '"';
    }
    else start = command + " " + '"' + imagewidget->currentImageName() + '"';
#endif
#ifdef Q_OS_WIN32
    QString filename = imagewidget->currentImageName();
    for (int i=1;i<filename.size();i++)
        if (filename[i] == '/')
        {
            filename.remove(i,1);
            filename.insert(i,"\\");
        }

    QString start = '"' + command + '"' + " " + '"' + filename + '"';
#endif
    qDebug() << start;
    vec->startDetached(start);
}
