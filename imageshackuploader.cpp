#include "imageshackuploader.h"
#include "ui_imageshackuploader.h"

ImageShackUploader::ImageShackUploader(QString imagename) :
    ui(new Ui::ImageShackUploader)
{
    ui->setupUi(this);

    QPixmap * preview = new QPixmap(imagename);
    if (preview->width() > preview->height())
        ui->previewLabel->setPixmap(preview->scaledToWidth(150));
    else
        ui->previewLabel->setPixmap(preview->scaledToHeight(150));

    STOP = false;
    SHOWHEADERS = true;

    QNetworkAccessManager qnam;
    QNetworkCookieJar *cookieJar = new QNetworkCookieJar;
    qnam.setCookieJar(cookieJar);

    QByteArray keyValue = "23459IQU68b63f0be4cfcf631cc6b40b5519c6cb";
    QByteArray formatValue = "json";

    QFile file(imagename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << endl << "can't open file " << file.fileName() << " because: " << file.errorString();
        return;
    }
    QByteArray imageData = file.readAll();

    QByteArray boundary = QString::number(mRand(1111111, 9999999)).toUtf8() + QString::number(mRand(1111111, 9999999)).toUtf8();
    postData = "-----------------------------" + boundary;
    postData.append("\r\nContent-Disposition: form-data; name=\"key\"\r\n\r\n" + keyValue);
    postData.append("\r\n-----------------------------" + boundary);

    QByteArray contentType = "application/octet-stream";
    QString fileName = file.fileName();
    if (fileName.endsWith(".jpg", Qt::CaseInsensitive) || fileName.endsWith(".jpeg", Qt::CaseInsensitive)) contentType = "image/jpeg";
    else if (fileName.endsWith(".png", Qt::CaseInsensitive)) contentType = "image/png";
    else if (fileName.endsWith(".gif", Qt::CaseInsensitive)) contentType = "image/gif";

    postData.append("\r\nContent-Disposition: form-data; name=\"fileupload\"; filename=\"" +
                    fileName.toUtf8() + "\"\r\nContent-Type: " + contentType + "\r\n\r\n");
    postData.append(imageData);
    postData.append("\r\n-----------------------------" + boundary);
    postData.append("\r\nContent-Disposition: form-data; name=\"format\"\r\n\r\n" + formatValue);
    postData.append("\r\n-----------------------------" + boundary + "--\r\n");

//    qDebug() << endl << "postData" << postData;

    request.setRawHeader("Content-Type", "multipart/form-data; boundary=---------------------------" + boundary);
    request.setRawHeader("Accept-Encoding", "identity");
    request.setRawHeader("Connection", "close");
    request.setUrl(QUrl("https://post.imageshack.us/upload_api.php"));

    if (STOP) return;
    POST(qnam, readAnswerToString, showHeaders, autoRedirect, request.rawHeader("Content-Type"));
    if (STOP) return;
    writeOnFile("test/1.html", answer);

    QHash<QString, QString> jsonElements;
    parseJson(jsonElements, answer);
/*
    qDebug() << endl << "status" << jsonElements.value("status");
    qDebug() << "version" << jsonElements.value("version");
    qDebug() << "timestamp" << jsonElements.value("timestamp");
    qDebug() << "base_url" << jsonElements.value("base_url");
    qDebug() << "id" << jsonElements.value("id");

    qDebug() << endl << "rating.ratings" << jsonElements.value("rating.ratings");
    qDebug() << "rating.avg" << jsonElements.value("rating.avg");

    qDebug() << endl << "files.server" << jsonElements.value("files.server");
    qDebug() << "files.bucket" << jsonElements.value("files.bucket");

    qDebug() << endl << "files.image.size" << jsonElements.value("files.image.size");
    qDebug() << "files.image.content-type" << jsonElements.value("files.image.content-type");
    qDebug() << "files.image.filename" << jsonElements.value("files.image.filename");
    qDebug() << "files.image.original_filename" << jsonElements.value("files.image.original_filename");

    qDebug() << endl << "files.thumb.size" << jsonElements.value("files.thumb.size");
    qDebug() << "files.thumb.content" << jsonElements.value("files.thumb.content");
    qDebug() << "files.thumb.filename" << jsonElements.value("files.thumb.filename");

    qDebug() << endl << "resolution.width" << jsonElements.value("resolution.width");
    qDebug() << "resolution.height" << jsonElements.value("resolution.height");

    qDebug() << endl << "class" << jsonElements.value("class");
    qDebug() << "visibility" << jsonElements.value("visibility");

    qDebug() << endl << "uploader.ip" << jsonElements.value("uploader.ip");
    qDebug() << "uploader.cookie" << jsonElements.value("uploader.cookie");


    QString imageLink = jsonElements.value("links.image_link");
    qDebug() << endl << "links.image_link" << imageLink;
    qDebug() << "links.image_html" << jsonElements.value("links.image_html");
    qDebug() << "links.image_bb" << jsonElements.value("links.image_bb");
    qDebug() << "links.image_bb2" << jsonElements.value("links.image_bb2");
    qDebug() << "links.thumb_link" << jsonElements.value("links.thumb_link");
    qDebug() << "links.thumb_bb" << jsonElements.value("links.thumb_bb");
    qDebug() << "links.thumb_bb2" << jsonElements.value("links.thumb_bb2");
    qDebug() << "links.is_link" << jsonElements.value("links.is_link");
    qDebug() << "links.done" << jsonElements.value("links.done");*/
    ui->linkLineEdit->setText(jsonElements.value("links.is_link"));
    ui->fullForumsLineEdit->setText(jsonElements.value("links.image_bb"));
    ui->fullSitesLineEdit->setText(jsonElements.value("links.image_html"));
    ui->fullDirectLineEdit->setText(jsonElements.value("links.image_link"));
    ui->thumbForumsLineEdit->setText(jsonElements.value("links.thumb_bb"));
    ui->thumbsSitesLineEdit->setText(jsonElements.value("links.thumb_link"));
}

ImageShackUploader::~ImageShackUploader()
{
    delete ui;
}

void ImageShackUploader::on_pushButton_clicked()
{
    close();
}

void ImageShackUploader::closeEvent(QCloseEvent *event)
{
    emit overed();
    event->accept();
}


void ImageShackUploader::GET(QNetworkAccessManager &qnam, int howReadAnswer, int headers, int redirect, QByteArray contentType, bool repeat)
{
    QEventLoop loopGet;
    int badProxyCount=0;

    request.setRawHeader("Content-Type", contentType);

    REPEATGET:

    reply = qnam.get(request);
    reply->ignoreSslErrors();

    QTimer::singleShot(60000, &loopGet, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loopGet, SLOT(quit()));
    loopGet.exec();

    if (reply->error())
    {
//        qDebug() << "REPLY ERROR:" << reply->errorString() << reply->error();

        if (repeat && badProxyCount<3)
        {
            badProxyCount++;
//            qDebug() << "badProxyCountGet: " << badProxyCount;
            delete reply;
            goto REPEATGET;
        }
    }

///////////////////////////////// ЗАПИСЬ ЗАГОЛОВКОВ НАЧАЛО ////////////////////////
    requestHeaders.clear();
    QList<QByteArray> listHeaders = reply->request().rawHeaderList();
    foreach(QByteArray nameHeader, listHeaders)
    {
        QByteArray valueHeader = reply->request().rawHeader(nameHeader);
        requestHeaders.insert(nameHeader, valueHeader);
    }
    replyHeaders.clear();
    listHeaders = reply->rawHeaderList();
    foreach(QByteArray nameHeader, listHeaders)
    {
        QByteArray valueHeader = reply->rawHeader(nameHeader);
        replyHeaders.insert(nameHeader, valueHeader);
    }
///////////////////////////////// ЗАПИСЬ ЗАГОЛОВКОВ КОНЕЦ ////////////////////////

    if (headers == showHeaders) HTTPHEADERS(reply);

    location = QByteArray::fromPercentEncoding(reply->rawHeader("Location"));
    if (!reply->rawHeader("Ajax-Location").isEmpty())
    {
        location = QByteArray::fromPercentEncoding(reply->rawHeader("Ajax-Location"));
    }
    if (redirect == autoRedirect)
    {
        if (!location.isEmpty())
        {
            lastUrl = reply->url().toString();

            redirectCount = 0;
            REDIRECT(qnam, howReadAnswer, showHeaders);
            return;
        }
    }

    answer.clear();
    answerBytes.clear();

    if (howReadAnswer == readAnswerToString)
    {
        answer = reply->readAll();
    }
    else if (howReadAnswer == readAnswerToByteArray)
    {
        answerBytes = reply->readAll();
    }
    else if (howReadAnswer == readAnswerToOther)
    {
        if (reply->rawHeader("Content-Type").contains("image/gif"))
        {
                answerBytes = reply->readAll();
        }
        else
        {
                answer = reply->readAll();
        }
    }

    lastUrl = reply->url().toString();

    delete reply;
}

void ImageShackUploader::POST(QNetworkAccessManager &qnam, int howReadAnswer, int headers, int redirect, QByteArray contentType, bool repeat)
{
    QEventLoop loopPost;
    int badProxyCount=0;

    REPEATPOST:

    request.setRawHeader("Content-Type", contentType);

    reply = qnam.post(request, postData);
    reply->ignoreSslErrors();

    QTimer::singleShot(60000, &loopPost, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loopPost, SLOT(quit()));
    loopPost.exec();

    if (reply->error())
    {
//        qDebug() << "REPLY ERROR:" << reply->errorString() << reply->error();

        if (repeat && badProxyCount<3)
        {
            badProxyCount++;
//            qDebug() << "badProxyCountPost: " << badProxyCount;
            delete reply;
            goto REPEATPOST;
        }
    }

///////////////////////////////// ЗАПИСЬ ЗАГОЛОВКОВ НАЧАЛО ////////////////////////
    requestHeaders.clear();
    QList<QByteArray> listHeaders = reply->request().rawHeaderList();
    foreach(QByteArray nameHeader, listHeaders)
    {
        QByteArray valueHeader = reply->request().rawHeader(nameHeader);
        requestHeaders.insert(nameHeader, valueHeader);
    }
    replyHeaders.clear();
    listHeaders = reply->rawHeaderList();
    foreach(QByteArray nameHeader, listHeaders)
    {
        QByteArray valueHeader = reply->rawHeader(nameHeader);
        replyHeaders.insert(nameHeader, valueHeader);
    }
///////////////////////////////// ЗАПИСЬ ЗАГОЛОВКОВ КОНЕЦ ////////////////////////

    if (headers == showHeaders) HTTPHEADERS(reply);

    location = QByteArray::fromPercentEncoding(reply->rawHeader("Location"));
    if (!reply->rawHeader("Ajax-Location").isEmpty())
    {
        location = QByteArray::fromPercentEncoding(reply->rawHeader("Ajax-Location"));
    }
    if (redirect == autoRedirect)
    {
        if (!location.isEmpty())
        {
            lastUrl = reply->url().toString();

            redirectCount = 0;
            REDIRECT(qnam, howReadAnswer, showHeaders);
            return;
        }
    }

    if (howReadAnswer == readAnswerToString)
    {
        answer = reply->readAll();
    }
    else if (howReadAnswer == readAnswerToByteArray)
    {
        answerBytes = reply->readAll();
    }

    lastUrl = reply->url().toString();

    delete reply;
}

void ImageShackUploader::REDIRECT(QNetworkAccessManager &qnam, int howReadAnswer, int headers)
{
    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

//    qDebug() << "redirect url do: " << redirectUrl;

    if (!redirectUrl.isEmpty() && redirectUrl.isRelative())
    {
        redirectUrl = reply->url().resolved(redirectUrl);
//        qDebug() << "redirect url posle: " << redirectUrl;
    }
    else if (!reply->rawHeader("Ajax-Location").isEmpty())
    {
        request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
        request.setRawHeader("Wicket-Ajax", QByteArray());
        redirectUrl = reply->url().resolved(QUrl::fromPercentEncoding(reply->rawHeader("Ajax-Location")));
//        qDebug() << "redirect url Ajax-Location posle: " << redirectUrl;
    }
    else if (redirectUrl.isEmpty())
    {
//        qDebug() << endl << this->objectName() << endl << endl << "NO REDIRECT!!!" << endl << endl << endl;
        return;
    }

    request.setRawHeader("Content-Type", QByteArray());
    request.setUrl(redirectUrl);

    QEventLoop loopRedirect;
    int badProxyCount=0;

    delete reply;

    REPEATREDIRECT:

    reply = qnam.get(request);
    reply->ignoreSslErrors();

    QTimer::singleShot(60000, &loopRedirect, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loopRedirect, SLOT(quit()));
    loopRedirect.exec();

//    qDebug() << reply->url();
    if (reply->error())
    {
//        qDebug() << "REPLY ERROR:" << reply->errorString() << reply->error();

        if (badProxyCount<3)
        {
            badProxyCount++;
//            qDebug() << "badProxyCountRedirect: " << badProxyCount;
            delete reply;
            goto REPEATREDIRECT;
        }
    }

///////////////////////////////// ЗАПИСЬ ЗАГОЛОВКОВ НАЧАЛО ////////////////////////
    requestHeaders.clear();
    QList<QByteArray> listHeaders = reply->request().rawHeaderList();
    foreach(QByteArray nameHeader, listHeaders)
    {
        QByteArray valueHeader = reply->request().rawHeader(nameHeader);
        requestHeaders.insert(nameHeader, valueHeader);
    }
    replyHeaders.clear();
    listHeaders = reply->rawHeaderList();
    foreach(QByteArray nameHeader, listHeaders)
    {
        QByteArray valueHeader = reply->rawHeader(nameHeader);
        replyHeaders.insert(nameHeader, valueHeader);
    }
///////////////////////////////// ЗАПИСЬ ЗАГОЛОВКОВ КОНЕЦ ////////////////////////

    if (headers == showHeaders) HTTPHEADERS(reply);

    if ((!reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().isEmpty() ||
         !reply->rawHeader("Ajax-Location").isEmpty())
            && redirectCount < 33)
    {
        redirectCount++;
        REDIRECT(qnam, howReadAnswer, showHeaders);
    }
    else
    {
        if (howReadAnswer == readAnswerToString)
        {
                answer = reply->readAll();
        }
        else if (howReadAnswer == readAnswerToByteArray)
        {
            answerBytes = reply->readAll();
        }

        locationRedirect = QByteArray::fromPercentEncoding(reply->rawHeader("Location"));
        if (!reply->rawHeader("Ajax-Location").isEmpty())
        {
            locationRedirect = QByteArray::fromPercentEncoding(reply->rawHeader("Ajax-Location"));
        }
        lastUrl = reply->url().toString();
        delete reply;
    }
}

void ImageShackUploader::HTTPHEADERS(QNetworkReply *reply)
{
    if (!SHOWHEADERS) return;

//    qDebug() << endl << this->objectName() << reply->url();
//    qDebug() << "Request headers:  ";
    QList<QByteArray> reqHeaders = reply->request().rawHeaderList();
    foreach (QByteArray reqName, reqHeaders)
    {
        QByteArray reqValue = reply->request().rawHeader(reqName);
//        qDebug() << reqName << ":" << reqValue;
    }
//    qDebug() << "Reply headers:  ";
    QList<QByteArray> reqHeadersReply = reply->rawHeaderList();
    foreach( QByteArray reqName, reqHeadersReply )
    {
        QByteArray reqValue = reply->rawHeader( reqName );
//        qDebug() << reqName << ":" << reqValue;
    }
}

quint64 ImageShackUploader::mRand(qint64 a, qint64 b, bool makeUSleep)
{
    if (makeUSleep) usleep(1);
    qsrand(QDateTime::currentMSecsSinceEpoch() + reinterpret_cast<quint64>(this));
    return (qrand() + qrand() * RAND_MAX)%(b-a+1)+a;
}

void ImageShackUploader::writeOnFile(const QString &fileName, const QString &data) const
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << data;
    file.close();
}

void ImageShackUploader::parseJson(QHash<QString, QString> &jsonElements, const QString &strIndexIn, const QString &prepend)
{
    QRegExp rx; rx.setMinimal(true); rx.setCaseSensitivity(Qt::CaseInsensitive);
    rx.setPattern("\"([^\"]+)\":(?:([^\\{][^,]*)(?:,|\\})|(\\{[^\\{]*\\})|(\\{.+\\}[^,]+))");
    int pos = 0;
    while ((pos = rx.indexIn(strIndexIn, pos)) != -1)
    {
        //qDebug() << "rx.cap(1)" << rx.cap(1) << "rx.cap(2)" << rx.cap(2) << "rx.cap(3)" << rx.cap(3) << "rx.cap(4)" << rx.cap(4);
        if (!rx.cap(2).isEmpty()) {
            jsonElements.insert(prepend + rx.cap(1), rx.cap(2).replace(QRegExp("(?:\"|\\})"), ""));
        }
        else if (!rx.cap(3).isEmpty())
        {
            parseJson(jsonElements, rx.cap(3), parent + rx.cap(1) + ".");
        }
        else if (!rx.cap(4).isEmpty())
        {
            parent.append(rx.cap(1) + ".");
            parseJson(jsonElements, rx.cap(4), parent);
            parent.clear();
        }
        pos += rx.matchedLength();
    }
}
