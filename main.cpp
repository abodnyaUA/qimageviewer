#include "qimageviewer.h"
#include <QApplication>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QTranslator>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /// system locale for messageboxes ///
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                          QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    /// Programm language ///
    QTranslator prTranslator;
    prTranslator.load(":/lng/qimageviewer_" + QLocale::system().name());
    a.installTranslator(&prTranslator);

    qDebug() << QLocale::system().name();

    QImageViewer w(argv[1]);
    w.show();
    
    return a.exec();
}
