#include "qimageviewer.h"
#include "ui_qimageviewer.h"


void QImageViewer::changeMode()
{
    if (!isReadyPreviews) return;

    if (imagewidget->isVisible() && imagewidget->size() != 0)
    {
        imagewidget->hide();
        ui->prevButton->hide();
        ui->nextButton->hide();
        if (isneedBut.fullscreen)  butFullscreen->hide();
        if (isneedBut.properties)  butProperties->hide();
        if (isneedBut.slideshow)   butSlideshow->hide();
        if (isneedBut.zoomIn)      butZoomIn->hide();
        if (isneedBut.zoomOriginal)butZoomOriginal->hide();
        if (isneedBut.zoomOut)     butZoomOut->hide();
        if (isneedBut.zoomWindow)  butZoomWindow->hide();
        ui->nextimageAction->setVisible(false);
        ui->previmageAction->setVisible(false);
        ui->zoomInAction->setVisible(false);
        ui->zoomOriginalAction->setVisible(false);
        ui->zoomOutAction->setVisible(false);
        ui->zoomWindowAction->setVisible(false);
        ui->fullscreenAction->setVisible(false);
        ui->slideshowAction->setVisible(false);
        ui->actionRedo->setVisible(false);
        ui->actionUndo->setVisible(false);
        ui->saveAction->setVisible(false);
        ui->saveAsAction->setVisible(false);
        ui->cropAction->setVisible(false);
        for (int i=0;i<editorsActions.size();i++)
            editorsActions[i]->setEnabled(false);
        previewwiget->show();
        if (previewwiget->size() != imagewidget->size()) previewwiget->loadImages(imagewidget->getImageList());
        mode = ModePreview;
        previewwiget->setFocus();
        butMODE->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Mode_Previews"])));
        setWindowTitle("QImageViewer - " + lastdirectory);
        if (previewwiget->size() == 0) setWindowTitle("QImageViewer");
    }
    else
    {
        previewwiget->hide();
        ui->prevButton->show();
        ui->nextButton->show();
        if (isneedBut.fullscreen)  butFullscreen->show();
        if (isneedBut.properties)  butProperties->show();
        if (isneedBut.slideshow)   butSlideshow->show();
        if (isneedBut.zoomIn)      butZoomIn->show();
        if (isneedBut.zoomOriginal)butZoomOriginal->show();
        if (isneedBut.zoomOut)     butZoomOut->show();
        if (isneedBut.zoomWindow)  butZoomWindow->show();
        ui->nextimageAction->setVisible(true);
        ui->previmageAction->setVisible(true);
        ui->zoomInAction->setVisible(true);
        ui->zoomOriginalAction->setVisible(true);
        ui->zoomOutAction->setVisible(true);
        ui->zoomWindowAction->setVisible(true);
        ui->fullscreenAction->setVisible(true);
        ui->slideshowAction->setVisible(true);
        ui->actionRedo->setVisible(true);
        ui->actionUndo->setVisible(true);
        ui->saveAction->setVisible(true);
        ui->saveAsAction->setVisible(true);
        ui->cropAction->setVisible(true);
        for (int i=0;i<editorsActions.size();i++)
            editorsActions[i]->setEnabled(true);
        if (previewwiget->size() != imagewidget->size() && !previewwiget->getList().isEmpty())
        {
            defaultpath = previewwiget->getList()[0];
            fileOpen();
        }
        imagewidget->show();
        imagewidget->setFocus();
        mode = ModeImage;
        butMODE->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Mode_Image"])));
        currentIndexWasChanged(imagewidget->currentImage());
        if (previewwiget->size() == 0) setWindowTitle("QImageViewer");
    }
}

void QImageViewer::openImageFromPreview(int number)
{
    imagewidget->setImage(number);
    changeMode();
}

void QImageViewer::modeRotateLeft()
{
    if (mode == ModeImage)   imagewidget->rotateLeft();
    if (mode == ModePreview)
    {
        if (previewwiget->rotateLeft(imagewidget->currentImageName(),imagewidget->isSaved()))
            imagewidget->loadimage(imagewidget->currentImageName());
    }
}

void QImageViewer::modeRotateRight()
{
    if (mode == ModeImage)   imagewidget->rotateRight();
    if (mode == ModePreview)
    {
        if (previewwiget->rotateRight(imagewidget->currentImageName(),imagewidget->isSaved()))
            imagewidget->loadimage(imagewidget->currentImageName());
    }
}

void QImageViewer::modeFlipHorizontal()
{
    if (mode == ModeImage)   imagewidget->flipHorizontal();
    if (mode == ModePreview)
    {
        if (previewwiget->flipHorizontal(imagewidget->currentImageName(),imagewidget->isSaved()))
            imagewidget->loadimage(imagewidget->currentImageName());
    }
}

void QImageViewer::modeFlipVertical()
{
    if (mode == ModeImage)   imagewidget->flipVertical();
    if (mode == ModePreview)
    {
        if (previewwiget->flipVertical(imagewidget->currentImageName(),imagewidget->isSaved()))
            imagewidget->loadimage(imagewidget->currentImageName());
    }
}

void QImageViewer::modeResize()
{
    if (mode == ModeImage)   resizeImage();
    if (mode == ModePreview)
    {
        resizeImageList();
        imagewidget->loadimage(imagewidget->currentImageName());
    }
}

void QImageViewer::modeUploadToVK()
{
    if (mode == ModeImage)   vkUploadImage();
    if (mode == ModePreview) vkUploadImageList();
}

void QImageViewer::modeUploadToImageshack()
{
    if (mode == ModeImage)   imageshackShare();
    if (mode == ModePreview) imageshackShareList();
}

void QImageViewer::modeWallpaper()
{
    if (mode == ModeImage)   imagewidget->setAsWallpaper();
    if (mode == ModePreview)
    {
        QStringList lst = previewwiget->selectedImages();
        if (!lst.isEmpty()) imagewidget->setAsWallpaperExtern(lst[0]);
    }
}

void QImageViewer::modeDelete()
{
    if (mode == ModeImage)
    {
        QString name = imagewidget->currentImageName();
        if (imagewidget->deleteCurrentItem()) previewwiget->removeImage(name);
    }
    if (mode == ModePreview)
    {
        int r = QMessageBox::warning(this, tr("Delete file"),
                                        tr("Do you really want to delete these files?"),
                                        QMessageBox::Yes | QMessageBox::Default,
                                        QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Cancel) return;

        QStringList lst = previewwiget->selectedImages();
        if (lst.isEmpty()) return;
        foreach (QString image,lst)
        {
            if (image == imagewidget->currentImageName())
            {
                QString name = imagewidget->currentImageName();
                if (!imagewidget->isSaved())
                {
                    int r = QMessageBox::warning(this, tr("Delete file"),
                                                    tr("You haven't save changes. Are you sure about deleting this file?"),
                                                    QMessageBox::Yes | QMessageBox::Default,
                                                    QMessageBox::Cancel | QMessageBox::Escape);
                    if (r == QMessageBox::Yes)
                    {
                        if (imagewidget->deleteCurrentItemWithoutAsc()) previewwiget->removeImage(name);
                    }
                }
                else if (imagewidget->deleteCurrentItemWithoutAsc()) previewwiget->removeImage(name);
            }
            else
            {
                previewwiget->removeImage(image);
                imagewidget->removeFromList(image);
                QFile(image).remove();
            }
        }
    }
    if (previewwiget->size() == 0)
    {
        listIsEmpty();
    }
}
