#include "qimageviewer.h"
#include <QApplication>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QTranslator>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("QImageViewer");
    QApplication::setApplicationVersion("0.1.7");

    QFile file("settings.txt");
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    QString lng;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        lng = QLocale::system().name();
    else
    {
        lng = out.readLine();
        lng = lng.right(lng.size()-9);
        if (lng == "rus") lng = "ru_RU";
        else if (lng == "ukr") lng = "uk_UA";
        else if (lng == "eng") lng = "en_US";
        else if (lng == "pol") lng = "pl_PL";
        else lng = QLocale::system().name();
    }
    file.close();

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
