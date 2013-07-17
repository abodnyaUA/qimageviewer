#include "qimageviewer.h"
#include <QApplication>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QTranslator>
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#ifdef Q_OS_MAC
#include "os_application.h"
#include <QFileOpenEvent>
#endif
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
#ifdef Q_OS_MAC
    OS_application a( argc, argv );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
#else
    QApplication a(argc, argv);
#endif
    //loadStyleSheet();
    QApplication::setApplicationName("QImageViewer");
    QApplication::setApplicationVersion("0.23");

    QSettings *qsettings = new QSettings("QImageViewer","QImageViewer");
    QString lng = qsettings->value("Programm/Language","sys").toString();
    if (lng == "rus") lng = "ru_RU";
    else if (lng == "ukr") lng = "uk_UA";
    else if (lng == "eng") lng = "en_US";
    else if (lng == "cze") lng = "cs_CS";
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

    QStringList args = QApplication::arguments();
#ifdef Q_OS_MAC
    if (args.size()>1) args.removeFirst();
    else args << "" << a.file();
#endif
    QImageViewer * w;

    // check incomming file. Image or not. If it isn't image, so ignore it.
    if (args.size() != 1)
    {
        QString url = args[1];
        if (!url.endsWith("jpg",Qt::CaseInsensitive) && !url.endsWith("png",Qt::CaseInsensitive) &&
            !url.endsWith("bmp",Qt::CaseInsensitive) && !url.endsWith("jpeg",Qt::CaseInsensitive) &&
            !url.endsWith("ppm",Qt::CaseInsensitive) && !url.endsWith("xbm",Qt::CaseInsensitive) &&
            !url.endsWith("xbm",Qt::CaseInsensitive) && !url.endsWith("tiff",Qt::CaseInsensitive) &&
            !url.endsWith("gif",Qt::CaseInsensitive) &&
            !url.endsWith("pbm",Qt::CaseInsensitive) && !url.endsWith("pgm",Qt::CaseInsensitive)) args.removeAt(1);
    }

    //Run application without any images
    if (args.size() == 1) w = new QImageViewer;
    //Run application with extern image
    else w = new QImageViewer(args[1]);

#ifdef Q_OS_MAC
    a.setWindow(w);
#endif
    //Show application
    w->show();
    
    return a.exec();
}
