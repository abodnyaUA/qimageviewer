#include "uploadtoimageshack.h"
void UploadToImageShack::operator()(QString imagename)
{
    QPixmap image(imagename);
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    upload(imageData,imagename);
}

void UploadToImageShack::upload(QByteArray &picture,QString imagename)
{
    qDebug() << "start upload";
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl("https://post.imageshack.us/upload_api.php"));
    request.setRawHeader("Host", "imageshack.us");
    request.setRawHeader("Content-type", "multipart/form-data, boundary=AyV04a");
    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Accept","*/*");

    QByteArray requestData;

    requestData.append("--AyV04a\r\n");
    requestData.append("content-disposition: ");
    requestData.append("form-data; name=\"key\"\r\n");
    requestData.append("\r\n");
    requestData.append("23459IQU68b63f0be4cfcf631cc6b40b5519c6cb");
    requestData.append("\r\n");

    requestData.append("--AyV04a\r\n");
    requestData.append("content-type: ");
    requestData.append("image/png\r\n");
    requestData.append("content-disposition: ");
    requestData.append(QString("file; name=\"fileupload\"; filename=\"%1\"\r\n").arg(imagename));
    requestData.append("Content-Transfer-Encoding: binary\r\n");
    requestData.append("\r\n");
    requestData.append(picture);
    requestData.append("\r\n");

    requestData.append("--AyV04a\r\n");
    requestData.append("content-disposition: ");
    requestData.append("form-data; name=\"format\"\r\n");
    requestData.append("\r\n");
    requestData.append("json\r\n");
    requestData.append("--AyV04a--");

    manager->post(request, requestData);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploaded(QNetworkReply*)));
    connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(problems()));
}

void UploadToImageShack::uploaded(QNetworkReply *response)
{
    qDebug() << "finish upload";
    QVariantMap responseMap = JSON::parse(response->readAll()).toMap();
    QVariantMap linkMap = responseMap["links"].toMap();
    QMap<QString,QString> links;
    links["Link"] = linkMap["is_link"].toString();
    links["ImageBB"] = linkMap["image_bb"].toString();
    links["ImageHtml"] = linkMap["image_html"].toString();
    links["ImageLink"] = linkMap["image_link"].toString();
    links["ThumbBB"] = linkMap["thumb_bb"].toString();
    links["ThumbLink"] = linkMap["thumb_link"].toString();
    emit linkReady(links);
}
void UploadToImageShack::problems()
{
    qDebug() << "problems with upload";
    QMap<QString,QString> links;
    emit linkReady(links);
}
void UploadToImageShack::abort()
{
 // How abort current uploading ?
}
