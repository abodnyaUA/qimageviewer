#ifndef UPLOADTOIMAGESHACK_H
#define UPLOADTOIMAGESHACK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
#include <QUrl>
#include <QBuffer>
#include <QDebug>
#include <QMessageBox>
#include "qvk/json.h"

class UploadToImageShack : public QObject
{
    Q_OBJECT
public:
    void operator() (QString name);
    void abort();
private:
    void upload(QByteArray &picture,QString imagename);
private slots:
    void uploaded(QNetworkReply *response);
    void problems();
signals:
    void linkReady(QMap<QString,QString>);
};

#endif // UPLOADTOIMAGESHACK_H
