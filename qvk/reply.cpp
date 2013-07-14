#include "reply.h"
#include <QDebug>

Reply::Reply(QObject *parent) :
    QObject(parent)
{
}

Reply::Reply(QNetworkReply *reply, QObject *parent) :
    QObject(parent)
{
    wrappedReply = reply;
    connect(wrappedReply, SIGNAL(finished()), this, SLOT(onRequestFinished()));
}

void Reply::setWrappedReply(QNetworkReply *reply)
{
    wrappedReply = reply;
    connect(wrappedReply, SIGNAL(finished()), this, SLOT(onRequestFinished()));
}

void Reply::onRequestFinished()
{
    QVariantMap parseResult = JSON::parse(wrappedReply->readAll()).toMap();
    auto response = parseResult.value("response");
    emit resultReady(response);
}
