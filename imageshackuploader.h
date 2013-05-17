#ifndef IMAGEHACKUPLOADER_H
#define IMAGEHACKUPLOADER_H

#include <QWidget>
#include <QCloseEvent>
#include <QtNetwork>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QHash>
#include "zlib.h"

namespace Ui {
class ImageShackUploader;
}

class ImageShackUploader : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageShackUploader(QString imagename);
    ~ImageShackUploader();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::ImageShackUploader *ui;

    enum answerType {
        readAnswerToString,
        readAnswerToByteArray,
        readAnswerToOther,
        noReadAnswer
    };
    enum headersShowType {
        showHeaders,
        noShowHeaders
    };
    enum redirectType {
        autoRedirect,
        noMakeRedirect
    };
    void GET(QNetworkAccessManager &qnam, int howReadAnswer = readAnswerToString, int headers = showHeaders, int redirect = autoRedirect, QByteArray contentType = QByteArray(), bool repeat = true);
    void POST(QNetworkAccessManager &qnam, int howReadAnswer = readAnswerToString, int headers = showHeaders, int redirect = autoRedirect, QByteArray contentType = "application/x-www-form-urlencoded", bool repeat = true);
    void REDIRECT(QNetworkAccessManager &qnam, int howReadAnswer = readAnswerToString, int headers = showHeaders);
    void HTTPHEADERS(QNetworkReply *reply);

    quint64 mRand(qint64, qint64, bool makeUSleep = false);
    void writeOnFile(const QString &fileName, const QString &data) const;
    void parseJson(QHash<QString, QString> &jsonElements, const QString &strIndexIn, const QString &prepend = QString());

    QNetworkReply *reply;
    QNetworkRequest request;
    QString answer;
    QByteArray answerBytes;
    QString lastUrl;
    QString location;
    QString locationRedirect;
    int redirectCount;
    QHash<QByteArray, QByteArray> requestHeaders;
    QHash<QByteArray, QByteArray> replyHeaders;
    QByteArray postData;

    bool SHOWHEADERS;
    bool STOP;

    QString parent;

signals:
    void overed();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // IMAGEHACKUPLOADER_H
