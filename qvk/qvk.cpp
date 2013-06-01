#include "qvk.h"

QVk::QVk(int application_id)
{
    app_id = application_id;
}

/// <AUTH SECTION>
void QVk::startAuth(QString icon) // auth step 1
{
    QUrl authUrl("http://api.vkontakte.ru/oauth/authorize");

    QUrlQuery *args = new QUrlQuery();
    args->addQueryItem("client_id", QString::number(app_id));
    args->addQueryItem("display", "popup");
    args->addQueryItem("scope", "photos,wall,offline");
    args->addQueryItem("redirect_uri", "http://oauth.vk.com/blank.html");
    args->addQueryItem("response_type", "token");
    authUrl.setQuery(*args);

    authWindow = new QWebView();
    authWindow->resize(470, 350);
    authWindow->show();
    authWindow->load(authUrl);
    authWindow->setWindowIcon(QIcon(QPixmap(icon)));
    connect(authWindow, SIGNAL(urlChanged(QUrl)), SLOT(onAuthRedirected(QUrl)));
}

void QVk::onAuthRedirected(QUrl new_url) // auth step 2
{
    if (new_url.path() == "/blank.html")
    {
        authWindow->hide();
        disconnect(this);
        new_url = new_url.toString().replace("#","?");
        QUrlQuery *args = new QUrlQuery(new_url);
        access_token = args->queryItemValue("access_token");
        currentUser_id = args->queryItemValue("user_id").toInt();
        initAlbumList();

        emit authSuccess();
        authStatus = true;
    }
    else
        emit authFail();
}

void QVk::initAlbumList()
{
    auto reply = request("photos.getAlbums");
    connect(reply, SIGNAL(resultReady(QVariant)), this, SLOT(onGotAlbumList(QVariant)));
}

void QVk::onGotAlbumList(QVariant reply)
{
    auto replyList = reply.toList();
    for (auto i : replyList)
    {
        auto map = i.toMap();
        QString title = map["title"].toString();
        int aid = map["aid"].toInt();
        albumList[title] = aid;
    }
    emit albumListWasUpdated();
}
/// </AUTH SECTION>


/// <REQUEST METHOD>
Reply* QVk::request(const QString &method, const QVariantMap &args) // standart request method, Reply is wrapper over QNetworkReply
{
    QUrl requestUrl = QString("https://api.vk.com/method/")+method;
    QUrlQuery *requestArgs = new QUrlQuery();

    for (auto it = args.begin(); it!=args.end(); ++it)
    {
        requestArgs->addQueryItem(it.key(), it.value().toString());
    }
    requestArgs->addQueryItem("access_token", access_token);

    requestUrl.setQuery(*requestArgs);

    manager = new QNetworkAccessManager();
    return new Reply(manager->get(QNetworkRequest(requestUrl)));
}
/// </REQUEST METHOD>

/// <UPLOAD SECTION>
void QVk::uploadPhotoToAlbum(QString filePath, int album_id)
{
    files.append(filePath);
    QVariantMap args;
    args["aid"] = album_id;
    auto reply = request("photos.getUploadServer", args);
    connect(reply, SIGNAL(resultReady(QVariant)), this, SLOT(onGotUploadServer(QVariant)));
}

void QVk::uploadPhotosToAlbum(QList<QString> fileList, int album_id)
{
    files = fileList;
    QVariantMap args;
    args["aid"] = album_id;
    auto reply = request("photos.getUploadServer", args);
    connect(reply, SIGNAL(resultReady(QVariant)), this, SLOT(onGotUploadServer(QVariant)));
}

void QVk::onGotUploadServer(QVariant reply)
{
    QVariantMap responseContent = reply.toMap();
    uploadServer = responseContent["upload_url"].toUrl();

    for (auto i : files)
    {
        uploadRequest(i, uploadServer);
    }

    files.clear();
}

void QVk::uploadRequest(QString filePath, QUrl uploadServer)
{
    QNetworkAccessManager *uploader = new QNetworkAccessManager(this);
    connect(uploader, SIGNAL(finished(QNetworkReply*)), this, SLOT(saveToAlbum(QNetworkReply*)));

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QFileInfo info(file);

        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("image/%1").arg(info.suffix())));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant(QString("form-data; name=\"photo\"; filename=\"%1\"").arg(info.fileName())));
        imagePart.setBody(file.readAll());
        file.close();

        multiPart->append(imagePart);

        uploader->post(QNetworkRequest(uploadServer), multiPart);
    }
}

void QVk::saveToAlbum(QNetworkReply *response)
{
    QVariantMap responseMap = JSON::parse(response->readAll()).toMap();
    request("photos.save", responseMap);
    emit uploadFinished();
}

void QVk::createNewAlbum(QString title)
{
    QVariantMap args;
    args["title"] = title;
    auto reply = request("photos.createAlbum", args);
    connect(reply, SIGNAL(resultReady(QVariant)), this, SLOT(onNewAlbumCreated(QVariant)));
}

void QVk::onNewAlbumCreated(QVariant response)
{
    initAlbumList();
}

/// </UPLOAD SECTION>

/// <DOWNLOAD SECTION>
void QVk::downloadAllUserPhotos(int owner_id, QString directory)
{
    downloadDirectory = directory;
    QVariantMap args;
    args["owner_id"] = owner_id;
    auto reply = request("photos.getAll", args);
    connect(reply, SIGNAL(resultReady(QVariant)), this, SLOT(onGotUrlList(QVariant)));
}

void QVk::downloadUserAlbum(int owner_id, int album_id, QString directory)
{
    downloadDirectory = directory;
    QVariantMap args;
    args["owner_id"] = owner_id;
    args["aid"] = album_id;
    auto reply = request("photos.get", args);
    connect(reply, SIGNAL(resultReady(QVariant)), this, SLOT(onGotUrlList(QVariant)));
}

void QVk::onGotUrlList(QVariant reply)
{
    qDebug () << reply;
    QVariantList replyContent = reply.toList();
    replyContent.removeAt(0);

    for (auto i : replyContent)
    {
        auto map = i.toMap();
        urlList.append(map["src_xxbig"].toString());
    }
    downloadPhotosList(urlList);
}

void QVk::downloadPhotosList(QList<QUrl> urlList)
{
    qDebug() << "in downloadPhotosList";
    QNetworkAccessManager *downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(saveToFile(QNetworkReply*)));
    for (QUrl i : urlList)
    {
        downloader->get(QNetworkRequest(i));
    }
}

void QVk::saveToFile(QNetworkReply *reply)
{
    qDebug() << "in saveToFile";
    QFile file(downloadDirectory+QString::number(photoNo)+".jpg");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.flush();
        file.close();
    }
    photoNo++;
    emit downloadFinished();
}
/// </DOWNLOAD SECTION>

/// <USEFUL PUBLIC METHODS>
QMap<QString, int> QVk::getAlbumsList()
{
    return albumList;
}

int QVk::getUserID()
{
    return currentUser_id;
}

void QVk::setUserID(int id)
{
    currentUser_id = id;
}

QString QVk::getAccessToken()
{
    return access_token;
}

void QVk::setAccessToken(QString token)
{
    access_token = token;
    authStatus = true;
    initAlbumList();
    emit authSuccess();
}

void QVk::setAppID(int id)
{
    app_id = id;
}

bool QVk::getAuthStatus()
{
    return authStatus;
}

/// </USEFUL PUBLIC METHODS>
