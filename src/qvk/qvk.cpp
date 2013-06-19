#include "qvk.h"

QVk::QVk(int application_id)
{
    app_id = application_id;
}

/// <AUTH SECTION>
void QVk::startAuth(QString icon) // auth step 1
{
    QUrl authUrl("http://api.vkontakte.ru/oauth/authorize");
#if QT_VERSION >= 0x050000
    QUrlQuery *args = new QUrlQuery();
    args->addQueryItem("client_id", QString::number(app_id));
    args->addQueryItem("display", "popup");
    args->addQueryItem("scope", "photos,wall,offline");
    args->addQueryItem("redirect_uri", "http://oauth.vk.com/blank.html");
    args->addQueryItem("response_type", "token");
    authUrl.setQuery(*args);
#else
    authUrl.addQueryItem("client_id", QString::number(app_id));
    authUrl.addQueryItem("display", "popup");
    authUrl.addQueryItem("scope", "photos,wall,offline");
    authUrl.addQueryItem("redirect_uri", "http://oauth.vk.com/blank.html");
    authUrl.addQueryItem("response_type", "token");
#endif

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
#if QT_VERSION >= 0x050000
        QUrlQuery *args = new QUrlQuery(new_url);
        access_token = args->queryItemValue("access_token");
        currentUser_id = args->queryItemValue("user_id").toInt();
#else
        access_token = new_url.queryItemValue("access_token");
        currentUser_id = new_url.queryItemValue("user_id").toInt();
#endif
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
#if QT_VERSION >= 0x050000
    QUrlQuery *requestArgs = new QUrlQuery();

    for (auto it = args.begin(); it!=args.end(); ++it)
    {
        requestArgs->addQueryItem(it.key(), it.value().toString());
    }
    requestArgs->addQueryItem("access_token", access_token);

    requestUrl.setQuery(*requestArgs);
#else
    for (auto it = args.begin(); it!=args.end(); ++it)
    {
        requestUrl.addQueryItem(it.key(), it.value().toString());
    }
    requestUrl.addQueryItem("access_token", access_token);
#endif

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
    qDebug() << "commin aid = "<<album_id;
    auto reply = request("photos.get", args);
    connect(reply, SIGNAL(resultReady(QVariant)), this, SLOT(onGotUrlList(QVariant)));
}

void QVk::onGotUrlList(QVariant reply)
{
    qDebug () << reply;
    photoNo = 1;
    downloadIndexes.clear();
    QVariantList replyContent = reply.toList();
    //replyContent.removeAt(0);
    downloadAmount = replyContent.size();
    urlList.clear();
    for (auto i : replyContent)
    {
        auto map = i.toMap();
        urlList.append(map["src_xxbig"].toString());
    }
    emit downloadReady(downloadAmount);
    downloadPhotosList(urlList);
}

int QVk::getDownloadAmount()
{
    return downloadAmount;
}

void QVk::downloadPhotosList(QList<QUrl> urlList)
{
    qDebug() << "in downloadPhotosList";
    downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(saveToFile(QNetworkReply*)));
    downloadCount = 0;
    urlDownloadList = urlList;
    downloader->get(QNetworkRequest(urlDownloadList[0]));
}

QString NumberToString(int num)
{
    if (num < 10) return "00"+QString::number(num);
    if (num < 100)return "0"+QString::number(num);
    return QString::number(num);
}

QList<QString> QVk::getDownloadIndexes()
{
    return downloadIndexes;
}
void QVk::abortDownload()
{
    disconnect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(saveToFile(QNetworkReply*)));
    delete downloader;
}

void QVk::saveToFile(QNetworkReply *reply)
{
    qDebug() << "in saveToFile";
    while (QFile(downloadDirectory+NumberToString(photoNo)+".jpg").exists()) photoNo++;
    downloadIndexes.append(NumberToString(photoNo));
    QFile file(downloadDirectory+NumberToString(photoNo)+".jpg");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.flush();
        file.close();
    }
    downloadCount++;
    photoNo++;
    emit downloadFinished();
    if (downloadCount < downloadAmount)
    {
        downloader->get(QNetworkRequest(urlDownloadList[downloadCount]));
    }
    else
    {
        disconnect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(saveToFile(QNetworkReply*)));
        delete downloader;
    }
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
