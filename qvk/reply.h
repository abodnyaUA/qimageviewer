#ifndef REPLY_H
#define REPLY_H

#include <QNetworkReply>
#include <QObject>
#include "json.h"


class Reply : public QObject
{
    Q_OBJECT
public:
    explicit Reply(QObject *parent = 0);
    explicit Reply(QNetworkReply *reply, QObject *parent = 0);
    void setWrappedReply(QNetworkReply *reply);
    /*virtual void abort() {}
    virtual void setReadBufferSize(qint64 size) {}
    virtual void close() {}
    virtual qint64 readData(char *data, qint64 maxlen) {}*/

signals:
    void resultReady(QVariant);
    
public slots:
    void onRequestFinished();
    //virtual void ignoreSslErrors() {}

private:
    QNetworkReply *wrappedReply;
};

#endif // REPLY_H
