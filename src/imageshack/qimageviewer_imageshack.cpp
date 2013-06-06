#include "qimageviewer.h"
#include "ui_qimageviewer.h"

/** upload single image to ImageShack.us**/
void QImageViewer::imageshackShare()
{
    ImageShackUploader * newimage = new ImageShackUploader(imagewidget->currentImageName());
    newimage->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Imageshack"])));
    imageshack.append(newimage);
}

/** upload imagelist to ImageShack.us **/
void QImageViewer::imageshackShareList()
{
    if (!isImageShackListUploaderOpened)
    {
        imageshackuploader = new ImageShackListUpload;
        imageshackuploader->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Imageshack"])));
        imageshackuploader->loadlist(imagewidget->getImageList(),lastdirectory,imagewidget->currentImage());
        connect(imageshackuploader,SIGNAL(overed(bool)),this,SLOT(imageshackShareListOvered(bool)));
        connect(imageshackuploader,SIGNAL(aborted(bool)),this,SLOT(imageshackShareListAborted(bool)));

        imageshackuploader->show();
        isImageShackListUploaderOpened = true;
    }
}

void QImageViewer::imageshackShareListOvered(bool arg)
{
    if (arg)
    {
        ImageShackListView * newlist = new ImageShackListView(imageshackuploader->getlinkslist());
        newlist->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Imageshack"])));
        imageshacklist.append(newlist);
        newlist->show();
    }
    imageshackuploader->close();
    disconnect(imageshackuploader,SIGNAL(overed(bool)),this,SLOT(imageshackShareListOvered(bool)));
    delete imageshackuploader;
    isImageShackListUploaderOpened = false;
}

void QImageViewer::imageshackShareListAborted(bool arg)
{
    if (arg)
    {
        ImageShackListView * newlist = new ImageShackListView(imageshackuploader->getlinkslist());
        newlist->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Imageshack"])));
        imageshacklist.append(newlist);
        newlist->show();
    }
}
