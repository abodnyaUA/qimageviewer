#include "qimageviewer.h"
#include "ui_qimageviewer.h"

void QImageViewer::vkStart()
{
    vkToken = vkApi->getAccessToken();
    vkUserId = vkApi->getUserID();
    ui->vkLogInAction->setVisible(false);
    ui->vkLogOutAction->setVisible(true);
    ui->vkDownloadAlbumAction->setEnabled(true);
    if (imagewidget->isReady())
    {
        ui->vkUploadImageAction->setEnabled(true);
        ui->vkUploadImagesAction->setEnabled(true);
    }
}

void QImageViewer::vkLogIn()
{
    vkApi = new QVk;
    connect(vkApi, SIGNAL(authSuccess()), this, SLOT(vkStart()));
    vkApi->startAuth(iconpacks[currenticonpack] + icon["Vkontakte"]);
}

void QImageViewer::vkLogOut()
{
    vkToken = "";
    vkUserId = 0;
    ui->vkLogInAction->setVisible(true);
    ui->vkLogOutAction->setVisible(false);
    ui->vkUploadImageAction->setEnabled(false);
    ui->vkUploadImagesAction->setEnabled(false);
    ui->vkDownloadAlbumAction->setEnabled(false);
    delete vkApi;
}

void QImageViewer::vkUploadImage()
{
    vkuploadimageform = new vkUploadImageForm(vkApi->getAlbumsList(),iconpacks[currenticonpack] + icon["Add"]);
    vkuploadimageform->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Vkontakte"])));
    vkuploadimageform->show();
    isVkUploadImageFormActive = true;
    connect(vkuploadimageform,SIGNAL(overed(bool)),this,SLOT(vkUploadImageOvered(bool)));
    connect(vkuploadimageform,SIGNAL(addNewAlbum(QString)),vkApi,SLOT(createNewAlbum(QString)));
    connect(vkApi,SIGNAL(albumListWasUpdated()),this,SLOT(vkUploadImageUpdateAlbums()));
}

void QImageViewer::vkUploadImageOvered(bool arg)
{
    if (arg) vkApi->uploadPhotoToAlbum(imagewidget->currentImageName(),vkuploadimageform->getalbum());

    vkuploadimageform->close();
    isVkUploadImageFormActive = false;
    disconnect(vkuploadimageform,SIGNAL(overed(bool)),this,SLOT(vkUploadImageOvered(bool)));
    delete vkuploadimageform;

    if (arg) QMessageBox::information(this,tr("Uploading has been completed"),
                                      tr("Image ")+imagewidget->currentImageName()+tr(" has been uploading to vk.com"),
                                      QMessageBox::Ok | QMessageBox::Default);
}

void QImageViewer::vkUploadImageUpdateAlbums()
{
    vkuploadimageform->updatealbums(vkApi->getAlbumsList());
}


void QImageViewer::vkUploadImageList()
{
    vkuploadimagesform = new vkUploadImagesForm(vkApi->getAlbumsList(),iconpacks[currenticonpack] + icon["Add"]);
    vkuploadimagesform->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Vkontakte"])));
    vkuploadimagesform->loadlist(imagewidget->getImageList(),lastdirectory,imagewidget->currentImage());
    vkuploadimagesform->show();
    isVkUploadImagesFormActive = true;
    connect(vkuploadimagesform,SIGNAL(overed(bool)),this,SLOT(vkUploadImageListOvered(bool)));
    connect(vkuploadimagesform,SIGNAL(addNewAlbum(QString)),vkApi,SLOT(createNewAlbum(QString)));
    connect(vkApi,SIGNAL(albumListWasUpdated()),this,SLOT(vkUploadImageListUpdateAlbums()));
}

void QImageViewer::vkUploadImageListOvered(bool arg)
{
    if (arg)
    {
        vkApi->uploadPhotoToAlbum(vkuploadimagesform->getlinkslist()[0],vkuploadimagesform->getalbum());
        connect(vkApi,SIGNAL(uploadFinished()),this,SLOT(vkUploadImageListUpdate()));
        connect(vkuploadimagesform,SIGNAL(abort(bool)),this,SLOT(vkUploadImageListAbort(bool)));
        vkUploadImagesCount = 0;
        vkUploadImagesAmount = vkuploadimagesform->getlinkslist().size();
        disconnect(vkuploadimagesform,SIGNAL(overed(bool)),this,SLOT(vkUploadImageListOvered(bool)));
    }
    else
    {
        vkuploadimagesform->close();
        isVkUploadImagesFormActive = false;
        disconnect(vkuploadimagesform,SIGNAL(overed(bool)),this,SLOT(vkUploadImageListOvered(bool)));
        delete vkuploadimagesform;
    }
}

void QImageViewer::vkUploadImageListUpdate()
{
    if (vkUploadImagesCount == vkUploadImagesAmount-1)
    {
        vkuploadimagesform->close();
        isVkUploadImagesFormActive = false;
        disconnect(vkApi,SIGNAL(uploadFinished()),this,SLOT(vkUploadImageListUpdate()));
        delete vkuploadimagesform;
        QMessageBox::information(this,tr("Uploading has been completed"),tr("All images have been uploading to vk.com"),
                                 QMessageBox::Ok | QMessageBox::Default);
    }
    else
    {
        vkUploadImagesCount++;
        vkuploadimagesform->setCurrentUploadedItem(vkUploadImagesCount);
        vkApi->uploadPhotoToAlbum(vkuploadimagesform->getlinkslist()[vkUploadImagesCount],vkuploadimagesform->getalbum());
    }
}

void QImageViewer::vkUploadImageListAbort(bool exit)
{
    if (exit)
    {
        vkuploadimagesform->close();
        isVkUploadImagesFormActive = false;
    }
    disconnect(vkuploadimagesform,SIGNAL(abort(bool)),this,SLOT(vkUploadImageListAbort(bool)));
    disconnect(vkApi,SIGNAL(uploadFinished()),this,SLOT(vkUploadImageListUpdate()));
    if (exit) delete vkuploadimagesform;
    if (!exit)
    {
        connect(vkuploadimagesform,SIGNAL(overed(bool)),this,SLOT(vkUploadImageListOvered(bool)));
        connect(vkuploadimagesform,SIGNAL(addNewAlbum(QString)),vkApi,SLOT(createNewAlbum(QString)));
        connect(vkApi,SIGNAL(albumListWasUpdated()),this,SLOT(vkUploadImageListUpdateAlbums()));
    }
}

void QImageViewer::vkUploadImageListUpdateAlbums()
{
    vkuploadimagesform->updatealbums(vkApi->getAlbumsList());
}

void QImageViewer::vkDownloadAlbum()
{
    vkdownloadalbumform = new vkDownloadAlbumForm(vkApi->getAlbumsList(),vkApi->getUserID(),lastdirectory);
    vkdownloadalbumform->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Vkontakte"])));
    vkdownloadalbumform->show();
    connect(vkdownloadalbumform,SIGNAL(overed(bool)),this,SLOT(vkDownloadAlbumOvered(bool)));
    isVkDownloadAlbumFormActive = true;
}

void QImageViewer::vkDownloadAlbumOvered(bool res)
{
    if (res)
    {
        QMap<QString, QVariant> args = vkdownloadalbumform->getDownloadData();
        vkApi->downloadUserAlbum(args["user"].toInt(),args["album"].toInt(),args["folder"].toString());
        connect(vkApi,SIGNAL(downloadFinished()),this,SLOT(vkDownloadAlbumUpdate()));
        connect(vkdownloadalbumform,SIGNAL(abort(bool)),this,SLOT(vkDownloadAlbumAbort(bool)));
        connect(vkApi,SIGNAL(downloadReady(int )),this,SLOT(vkDownloadAlbumReady(int)));
        //connect(vkApi,&QVk::downloadReady,[=](int amount) {vkDownloadImagesAmount = amount;});
        vkDownloadImagesCount = 0;
        disconnect(vkdownloadalbumform,SIGNAL(overed(bool)),this,SLOT(vkDownloadAlbumOvered(bool)));
    }
    else
    {
        vkdownloadalbumform->close();
        isVkDownloadAlbumFormActive = false;
        disconnect(vkdownloadalbumform,SIGNAL(overed(bool)),this,SLOT(vkDownloadAlbumOvered(bool)));
        delete vkdownloadalbumform;
    }
}
void QImageViewer::vkDownloadAlbumReady(int amount)
{
    vkDownloadImagesAmount = amount;
    disconnect(vkApi,SIGNAL(downloadReady(int )),this,SLOT(vkDownloadAlbumReady(int)));
}

void QImageViewer::vkDownloadAlbumUpdate()
{
    if (vkDownloadImagesCount == vkDownloadImagesAmount-1)
    {
        QString folder = vkdownloadalbumform->getDownloadData()["folder"].toString();
        vkdownloadalbumform->close();
        isVkDownloadAlbumFormActive = false;
        disconnect(vkApi,SIGNAL(downloadFinished()),this,SLOT(vkDownloadAlbumUpdate()));
        delete vkdownloadalbumform;
        QMessageBox::information(this,tr("Downloading has been completed"),
                                 tr("All images have been downloading to ")+folder,
                                 QMessageBox::Ok | QMessageBox::Default);
        defaultpath = folder + vkApi->getDownloadIndexes()[0]+".jpg";
        fileOpen();
    }
    else
    {
        vkDownloadImagesCount++;
        vkdownloadalbumform->setCurrentDownloadedProcent(
                    ((int)((double)((vkDownloadImagesCount)*100.0)/(double)(vkDownloadImagesAmount))) );
    }
}

void QImageViewer::vkDownloadAlbumAbort(bool)
{
    if (exit)
    {
        vkdownloadalbumform->close();
        isVkDownloadAlbumFormActive = false;
    }
    disconnect(vkdownloadalbumform,SIGNAL(abort(bool)),this,SLOT(vkDownloadAlbumAbort(bool)));
    disconnect(vkApi,SIGNAL(downloadFinished()),this,SLOT(vkDownloadAlbumUpdate()));
    if (exit)
    {
        vkApi->abortDownload();
        int r = QMessageBox::question(this,tr("Some images have been downloaded"),
                                      tr("Do you want to delete them?"),
                                      QMessageBox::Yes, QMessageBox::No);
        if (r == QMessageBox::Yes)
        {
            QList<QString> lst = vkApi->getDownloadIndexes();
            qDebug() << "lst download = " <<lst.size();
            QString folder = vkdownloadalbumform->getDownloadData()["folder"].toString();
            for (int i=0;i<lst.size();i++)
                QFile(folder+lst[i]+".jpg").remove();
        }
        delete vkdownloadalbumform;
    }
    if (!exit)
    {
        connect(vkdownloadalbumform,SIGNAL(overed(bool)),this,SLOT(vkDownloadAlbumOvered(bool)));
    }
}
