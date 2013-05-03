#include "qimageviewer.h"
#include <QApplication>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QTranslator>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
    prTranslator.load(":/lng/qimageviewer_" + lng);
    a.installTranslator(&prTranslator);

    qDebug() << QLocale::system().name();

    QImageViewer w(argv[1]);
    w.show();
    
    return a.exec();
}
