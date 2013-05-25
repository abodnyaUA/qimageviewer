#include "qimageviewer.h"
#include <QApplication>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QTranslator>
#include <QSettings>
#include <QDebug>
void loadStyleSheet()
{
    /* Let's use QFile and point to a resource... */
    QFile data(":/style.qss");
    QString style;
    /* ...to open the file */
    if(data.open(QFile::ReadOnly))
    {
        /* QTextStream... */
        QTextStream styleIn(&data);
        /* ...read file to a string. */
        style = styleIn.readAll();
        data.close();
        /* We'll use qApp macro to get the QApplication pointer
         * and set the style sheet application wide. */
        qApp->setStyleSheet(style);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //loadStyleSheet();
    QApplication::setApplicationName("QImageViewer");
    QApplication::setApplicationVersion("0.1.8");

    QSettings *qsettings = new QSettings("QImageViewer","QImageViewer");
    QString lng = qsettings->value("Programm/Language","sys").toString();
    if (lng == "rus") lng = "ru_RU";
    else if (lng == "ukr") lng = "uk_UA";
    else if (lng == "eng") lng = "en_US";
    else if (lng == "pol") lng = "pl_PL";
    else lng = QLocale::system().name();

    /// system locale for messageboxes ///
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + lng,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    /// Programm language ///
    QTranslator prTranslator;
    qDebug() << lng;
    prTranslator.load(":/lng/qimageviewer_" + lng);
    a.installTranslator(&prTranslator);

    qDebug() << "argc="<<argc;
    QImageViewer * w;
    if (argc == 1) w = new QImageViewer;
    else w = new QImageViewer(QString::fromLocal8Bit(argv[1]));
    w->show();
    
    return a.exec();
}
