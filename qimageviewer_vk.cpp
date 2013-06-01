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
        connect(vkuploadimagesform,SIGNAL(abort()),this,SLOT(vkUploadImageListAbort()));
        vkUploadImagesCount = 0;
        vkUploadImagesAmount = vkuploadimagesform->getlinkslist().size(); //OR "-1" ?
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

void QImageViewer::vkUploadImageListAbort()
{
    vkuploadimagesform->close();
    disconnect(vkuploadimagesform,SIGNAL(abort()),this,SLOT(vkUploadImageListAbort()));
    disconnect(vkApi,SIGNAL(uploadFinished()),this,SLOT(vkUploadImageListUpdate()));
    delete vkuploadimagesform;
}

void QImageViewer::vkUploadImageListUpdateAlbums()
{
    vkuploadimagesform->updatealbums(vkApi->getAlbumsList());
}

void QImageViewer::vkDownloadAlbum()
{

}
