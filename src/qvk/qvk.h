#ifndef QVK_H
#define QVK_H

#include <QObject>
#include <QUrl>
#include <QList>
#include <QFile>
#include <QVariantList>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QHttpPart>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#include <QtWebKitWidgets/QWebView>
#else
#include <QUrl>
#include <QQueue>
#include <QtWebKit/QWebView>
#endif
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include "reply.h"
#include "json.h"

class QVk : public QObject
{
    Q_OBJECT
public:
    QVk(int application_id=3642078);
    void startAuth(QString icon);
    void uploadPhotoToAlbum(QString filePath, int album_id);
    void uploadPhotosToAlbum(QList<QString> fileList, int album_id);

    void downloadAllUserPhotos(int owner_id, QString directory = QString());
    void downloadUserAlbum(int owner_id, int album_id, QString directory = QString());
    int getDownloadAmount();
    QList<QString> getDownloadIndexes();
    void abortDownload();

    QMap<QString, int> getAlbumsList();
    int getUserID();
    void setUserID(int id);
    QString getAccessToken();
    void setAccessToken(QString token);
    void setAppID(int id);
    bool getAuthStatus();

public slots:
    void createNewAlbum(QString title);

signals:
    void authSuccess();
    void authFail();
    void uploadFinished();
    void downloadFinished();
    void downloadReady(int);
    void albumListWasUpdated();

private slots:
    void onAuthRedirected(QUrl new_url);
    void onGotUploadServer(QVariant reply);
    void saveToAlbum(QNetworkReply *response);
    void onGotUrlList(QVariant reply);
    void downloadPhotosList(QList<QUrl> urlList);
    void saveToFile(QNetworkReply *reply);
    void onGotAlbumList(QVariant reply);
    void onNewAlbumCreated(QVariant response);
    
private:
    //methods
    Reply* request(const QString &method, const QVariantMap &args = QVariantMap());
    void uploadRequest(QString filePath, QUrl uploadServer);
    void initAlbumList();

    //vars
    bool authStatus = false;
    int app_id;
    QString access_token;
    QWebView *authWindow;
    QNetworkAccessManager *manager;
    int photoNo = 1;
    int current_album_id;
    int currentUser_id;
    QHttpMultiPart *multiPart;

    QList<QString> downloadIndexes;
    int downloadAmount;
    int downloadCount;
    QNetworkAccessManager *downloader;
    QString downloadDirectory;
    QList<QUrl> urlDownloadList;

    QUrl uploadServer;
    QList<QUrl> urlList;
    QList<QString> files;
    QMap<QString, int> albumList;
};

#endif // QVK_H
