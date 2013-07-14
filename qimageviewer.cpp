#include "qimageviewer.h"
#include "ui_qimageviewer.h"

QImageViewer::QImageViewer(QString path, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QImageViewer)
{
    ui->setupUi(this);

    // init image //
    imagewidget = new image;
    previewwiget = new PreviewMode;
    isfullScreenActive = false;

    ui->verticalLayout->addWidget(imagewidget);
    ui->verticalLayout->addWidget(previewwiget);
    previewwiget->hide();
    mode = ModeImage;

    ui->mainToolBar->hide();

    dialog = new QFileDialog(this);
    // Default Folder //
    loadsettings();
    // Load hotkeys to image //
    imagewidget->loadhotkeys(&hotkeys);
    // Signals-slots and status-tips for menu actions //
    createActions();
    // Shortcuts for menu actions //
    createHotkeys();
    // Icons and other design for menu actions //
    createDesign();
    // Panel with buttons //
    createPanel();

    // Default open or open argv[1] image //
    if (path == "\0")
    {
        defaultpath = QString::null;
        imagewidget->loadimage(":/res/logo.png");
        defaultpath = QString::null;
        imagewidget->setReady(false);
        imagewidget->setImage(0);
        setWindowTitle("QImageViewer");
    }
    else
    {
        defaultpath = path;
        fileOpen();
    }

    isSettingsActive = false;
    isEditosManagerActive = false;
    isEditorAddFormActive = false;
    isImageShackListUploaderOpened = false;
    isVkUploadImageFormActive = false;
    isVkUploadImagesFormActive = false;
    isVkDownloadAlbumFormActive = false;
    isAboutFormOpened = false;
    isUpdateDialogRunning = false;
    isResizeListActive = false;
    isReadyPreviews = false;

    setWindowState(Qt::WindowMaximized);
    imagewidget->setAcceptDrops(false);
    setAcceptDrops(true);
    if (checkAutoUpdates)
    {
        if (timesToUpdate > 10) timesToUpdate = 0;
        if (timesToUpdate == 0)
        {
            automateUpdate = true;
            checkupdates();
        }
    }
    automateUpdate = false;
}

void QImageViewer::previewsReady()
{
    butMODE->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Mode_Image"])));
    isReadyPreviews = true;
    ui->actionMode->setEnabled(true);
}

void QImageViewer::setStatusName(bool arg)
{
    if (arg)
    {
        previewwiget->reloadPreview(imagewidget->currentImage());
        setWindowTitle("QImageViewer - "+imagewidget->currentImageName());
    }
    else
        setWindowTitle("QImageViewer - *"+imagewidget->currentImageName());
}

void QImageViewer::setUndoEnable(bool arg)
{
    ui->actionUndo->setEnabled(arg);
}

void QImageViewer::setRedoEnable(bool arg)
{
    ui->actionRedo->setEnabled(arg);
}

/** Current image was changing from imagewidget **/
void QImageViewer::currentIndexWasChanged(int indx)
{
    setWindowTitle("QImageViewer - ["+QString::number(indx+1)+
                   tr(" of ") + QString::number(imagewidget->size()) + "] "
                   + imagewidget->currentImageName());
    statusBar()->showMessage(QString::number(imagewidget->currentPixmap().width())+
                              QString("x")+
                              QString::number(imagewidget->currentPixmap().height()));
}

/** Show previous image by button "Previous" or shortcut Crtl+Left **/
void QImageViewer::prevImage()
{
    if (imagewidget->currentImage() > 0)
        imagewidget->setImage(imagewidget->currentImage()-1);
    else
        imagewidget->setImage(imagewidget->size()-1);
    setWindowTitle("QImageViewer - ["+QString::number(imagewidget->currentImage()+1)+
                   tr(" of ") + QString::number(imagewidget->size()) + "] "
                   + imagewidget->currentImageName());
}

/** Show next image by button "Next" or shortcut Crtl+Right **/
void QImageViewer::nextImage()
{
    if (imagewidget->currentImage() < (imagewidget->size()-1))
        imagewidget->setImage(imagewidget->currentImage()+1);
    else
        imagewidget->setImage(0);
    setWindowTitle("QImageViewer - ["+QString::number(imagewidget->currentImage()+1)+
                   tr(" of ") + QString::number(imagewidget->size()) + "] "
                   + imagewidget->currentImageName());
}

/** Open new image **/
void QImageViewer::fileOpen()
{
    QString path;
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if (defaultpath == QString::null)
    {
        path = dialog->getOpenFileName(this,tr("Opening image file"),
               lastdirectory,tr("All pictures formats (*.jpg *.jpeg *.png *.bmp *.gif *.tiff *.pbm *.pgm *.ppm *.xbm *.xpm);;"
                                "Joint Photographic Experts Group (*.jpeg *.jpg);;"
                                "Portable Network Graphics(*.png);;"
                                "Windows Bitmap (*.bmp);;"
                                "Graphic Interchange Format (*.gif);;"
                                "Tagged Image File Format (*.tiff);;"
                                "Portable Bitmap (*.pbm);;"
                                "Portable Graymap (*.pgm);;"
                                "Portable Pixmap (*.ppm);;"
                                "X11 Bitmap (*.xbm *.xpm"));
    }
    else
    {
        path = defaultpath;
        defaultpath = QString::null;
    }
    if( !path.isNull() && QFile(path).exists())
    {
        if (mode == ModePreview) changeMode();
        // Remember last directory //
        dialog->selectFile(path);
        lastdirectory = dialog->directory().path();

        imagewidget->loadimage(path);
        setWindowTitle("QImageViewer - "+path);

        ui->saveAction->setEnabled(true);
        ui->saveAsAction->setEnabled(true);
        ui->rotateLeftAction->setEnabled(true);
        ui->rotateRightAction->setEnabled(true);
        ui->flipHorizontalAction->setEnabled(true);
        ui->flipVerticalAction->setEnabled(true);
        ui->deleteFileAction->setEnabled(true);
        ui->resizeAction->setEnabled(true);
        ui->resizeitemsAction->setEnabled(true);
        ui->previmageAction->setEnabled(true);
        ui->nextimageAction->setEnabled(true);
        ui->slideshowAction->setEnabled(true);
        ui->wallpaperAction->setEnabled(true);
        ui->cropAction->setEnabled(true);
        ui->zoomInAction->setEnabled(true);
        ui->zoomOutAction->setEnabled(true);
        ui->zoomOriginalAction->setEnabled(true);
        ui->zoomWindowAction->setEnabled(true);
        for (int i=0;i<editorsActions.size();i++)
            editorsActions[i]->setEnabled(true);
        ui->shareImageShackAction->setEnabled(true);
        ui->shareImageShackListAction->setEnabled(true);
        if (!vkToken.isEmpty())
        {
            ui->vkUploadImageAction->setEnabled(true);
            ui->vkUploadImagesAction->setEnabled(true);
        }
        butMODE->setEnabled(true);
        ui->actionMode->setEnabled(false);
        isReadyPreviews = false;
        butMODE->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Mode_Loading"])));

        ui->prevButton->setEnabled(true);
        ui->nextButton->setEnabled(true);
        if (isneedBut.rotateLeft)    butRotateLeft->setEnabled(true);
        if (isneedBut.rotateRight)   butRotateRight->setEnabled(true);
        if (isneedBut.flipHorizontal)butFlipHorizontal->setEnabled(true);
        if (isneedBut.flipVertical)  butFlipVertical->setEnabled(true);
        if (isneedBut.zoomIn)        butZoomIn->setEnabled(true);
        if (isneedBut.zoomOut)       butZoomOut->setEnabled(true);
        if (isneedBut.zoomWindow)    butZoomWindow->setEnabled(true);
        if (isneedBut.zoomOriginal)  butZoomOriginal->setEnabled(true);
        if (isneedBut.fullscreen)    butFullscreen->setEnabled(true);
        if (isneedBut.slideshow)     butSlideshow->setEnabled(true);
        if (isneedBut.properties)    butProperties->setEnabled(true);

        // find other pictures in folder //
        filesFind();
        setWindowTitle("QImageViewer - ["+QString::number(imagewidget->currentImage()+1)+
                       tr(" of ") + QString::number(imagewidget->size()) + "] "
                       + imagewidget->currentImageName());

        previewwiget->loadImages(imagewidget->getImageList());
    }
    defaultpath = QString::null;
}

/** Find pictures in current folder **/
void QImageViewer::filesFind()
{
    QStringList filter("*.png");
    filter.append("*.gif");
    filter.append("*.bmp");
    filter.append("*.jpg");
    filter.append("*.jpeg");
    filter.append("*.tiff");
    filter.append("*.ppm");
    filter.append("*.xbm");
    filter.append("*.xpm");
    filter.append("*.pbm");
    filter.append("*.pgm");
    QStringList imagefiles = dialog->directory().entryList(filter,QDir::NoFilter,QDir::SortByMask);
    qSort(imagefiles.begin(),imagefiles.end());
#ifdef Q_OS_WIN32
    for (int i=1;i<lastdirectory.size();i++)
        if (lastdirectory[i] == '/')
        {
            lastdirectory.remove(i,1);
            lastdirectory.insert(i,"\\");
        }
    for (int i=0;i<imagefiles.size();i++) imagefiles[i] = lastdirectory + '\\' + imagefiles[i];
#else
    for (int i=0;i<imagefiles.size();i++) imagefiles[i] = lastdirectory + '/' + imagefiles[i];
#endif

//    previewListWidget->clear();
//    previewListWidget->setResizeMode(QListView::Adjust);

    imagewidget->loadimagelist(imagefiles);

    //Previews//
//    previewLoader->loadList(imagefiles);
//    connect(&previewThread,SIGNAL(started()),previewLoader,SLOT(run()));
//    connect(previewLoader,SIGNAL(finished()),&previewThread,SLOT(terminate()));
//    previewLoader->moveToThread(&previewThread);
//    previewThread.start();
}

/** Save current file with same name **/
void QImageViewer::fileSave()
{
    if (imagewidget->isReady())
    {
        imagewidget->saveimage(imagewidget->currentImageName());
        imagewidget->setSaved();
    }
}

/** Save current file with new name **/
void QImageViewer::fileSaveAs()
{
    if (imagewidget->isReady())
    {
        QString path = dialog->getSaveFileName(this,tr("Saving files"),lastdirectory,
                                       tr("All picture formats (*.jpg *.jpeg *.png *.bmp *.tiff *.ppm *.xbm *.xpm);;"
                                          "Joint Photographic Experts Group (*.jpeg *.jpg);;"
                                          "Portable Network Graphics (*.png);;"
                                          "Windows Bitmap (*.bmp);;"
                                          "Tagged Image File Format (*.tiff);;"
                                          "Portable Pixmap (*.ppm);;"
                                          "X11 Bitmap (*.xbm *.xpm)"));
        if( !path.isNull() )
        {
            imagewidget->saveimage(path);
            imagewidget->setSaved();
            dialog->selectFile(path);
            qDebug() << lastdirectory;
            qDebug() << dialog->directory().path();
            if (lastdirectory == dialog->directory().path())
            {
                imagewidget->insertImage(path,imagewidget->currentImage()+1);
                nextImage();
                setWindowTitle("QImageViewer - ["+QString::number(imagewidget->currentImage()+1)+
                               tr(" of ") + QString::number(imagewidget->size()) + "] "
                               + imagewidget->currentImageName());
            }
        }
    }
}

void QImageViewer::fullScreenFromImage()
{
    if (isfullScreenActive)
        fullScreenOvered();
    else fullScreen();
}

/** Show Fullscreen window, hide this **/
void QImageViewer::fullScreen()
{
    // init fullscreen mode //
    fullScreenWidget = new fullscreen(imagewidget,&hotkeys,fullscreencolor);
    connect(fullScreenWidget,SIGNAL(fullscreenEnded()),this,SLOT(fullScreenOvered()));
    connect(fullScreenWidget,SIGNAL(needOpen()),this,SLOT(fileOpen()));
    connect(fullScreenWidget,SIGNAL(needSave()),this,SLOT(fileSave()));
    connect(fullScreenWidget,SIGNAL(needSaveAs()),this,SLOT(fileSaveAs()));
    connect(fullScreenWidget,SIGNAL(needQuit()),this,SLOT(close()));
    connect(fullScreenWidget,SIGNAL(needCrop()),this,SLOT(cropImage()));
    connect(fullScreenWidget,SIGNAL(needResize()),this,SLOT(resizeImage()));
    connect(fullScreenWidget,SIGNAL(needResizeItems()),this,SLOT(resizeImageList()));
    connect(fullScreenWidget,SIGNAL(needRotateLeft()),imagewidget,SLOT(rotateLeft()));
    connect(fullScreenWidget,SIGNAL(needRotateRight()),imagewidget,SLOT(rotateRight()));
    connect(fullScreenWidget,SIGNAL(needFlipHorizontal()),imagewidget,SLOT(flipHorizontal()));
    connect(fullScreenWidget,SIGNAL(needFlipVertical()),imagewidget,SLOT(flipVertical()));
    connect(fullScreenWidget,SIGNAL(needUndo()),imagewidget,SLOT(prevBuffer()));
    connect(fullScreenWidget,SIGNAL(needRedo()),imagewidget,SLOT(nextBuffer()));
    connect(fullScreenWidget,SIGNAL(needDelete()),imagewidget,SLOT(deleteCurrentItem()));
    fullScreenWidget->setSlideshowInterval(slideshowInterval);
    fullScreenWidget->setSlideshowSmoothTransition(slideshowSmoothTransition);

    fullScreenWidget->resize(desk.width(),desk.height());
    fullScreenWidget->showFullScreen();

    fullScreenWidget->lay->addWidget(imagewidget);
    imagewidget->resize(desk.width(),desk.height());
    imagewidget->resetZoom();
    imagewidget->reloadImage();
    imagewidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imagewidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fullScreenWidget->show();
    this->hide();
    isfullScreenActive = true;
    imagewidget->setFullscreen(fullscreencolor);
}

/** Hide fullscreen window, show this **/
void QImageViewer::fullScreenOvered()
{
    isfullScreenActive = false;
    this->show();
    fullScreenWidget->close();
    ui->verticalLayout->addWidget(imagewidget);
    imagewidget->resize(width()-22,height()-115);
    imagewidget->resetZoom();
    imagewidget->reloadImage();
    imagewidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    imagewidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // remove fullscreen mode //
    disconnect(fullScreenWidget,SIGNAL(fullscreenEnded()),this,SLOT(fullScreenOvered()));
    disconnect(fullScreenWidget,SIGNAL(needOpen()),this,SLOT(fileOpen()));
    disconnect(fullScreenWidget,SIGNAL(needSave()),this,SLOT(fileSave()));
    disconnect(fullScreenWidget,SIGNAL(needSaveAs()),this,SLOT(fileSaveAs()));
    disconnect(fullScreenWidget,SIGNAL(needQuit()),this,SLOT(close()));
    disconnect(fullScreenWidget,SIGNAL(needCrop()),this,SLOT(cropImage()));
    disconnect(fullScreenWidget,SIGNAL(needResize()),this,SLOT(resizeImage()));
    disconnect(fullScreenWidget,SIGNAL(needResizeItems()),this,SLOT(resizeImageList()));
    disconnect(fullScreenWidget,SIGNAL(needRotateLeft()),imagewidget,SLOT(rotateLeft()));
    disconnect(fullScreenWidget,SIGNAL(needRotateRight()),imagewidget,SLOT(rotateRight()));
    disconnect(fullScreenWidget,SIGNAL(needFlipHorizontal()),imagewidget,SLOT(flipHorizontal()));
    disconnect(fullScreenWidget,SIGNAL(needFlipVertical()),imagewidget,SLOT(flipVertical()));
    disconnect(fullScreenWidget,SIGNAL(needUndo()),imagewidget,SLOT(prevBuffer()));
    disconnect(fullScreenWidget,SIGNAL(needRedo()),imagewidget,SLOT(nextBuffer()));
    disconnect(fullScreenWidget,SIGNAL(needDelete()),imagewidget,SLOT(deleteCurrentItem()));
    delete fullScreenWidget;
    imagewidget->unsetFullscreen();
}

/** Start slideshow **/
void QImageViewer::slideShow()
{
    if (!isfullScreenActive)
    {
        fullScreen();
        fullScreenWidget->startSlideShow();
    }
    else
    {
        fullScreenOvered();
    }
}


/** Show editResize window, hide this **/
void QImageViewer::resizeImage()
{
    // init editResize window //
    editFormResize = new editformResize(width(),height());
    editFormResize->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Resize"])));
    connect(editFormResize,SIGNAL(editFinished(bool)),this,SLOT(resizeImageOvered(bool)));

    if (this->windowState() == Qt::WindowMaximized) editFormResize->setWindowState(Qt::WindowMaximized);
    else
    {
        editFormResize->resize(this->width(),this->height());
        editFormResize->setWindowState(Qt::WindowNoState);
    }
    editFormResize->loadImage(imagewidget->currentPixmap());
    editFormResize->show();
    this->hide();
}

/** Show this window, hide editResize window **/
void QImageViewer::resizeImageOvered(bool result)
{
    if (!isfullScreenActive) this->show();
    if (result)
    {
        imagewidget->addToBuffer(editFormResize->getpixmap());
        imagewidget->setEdited();
    }
    if (editFormResize->windowState() == Qt::WindowMaximized) this->setWindowState(Qt::WindowMaximized);
    else
    {
        this->resize(editFormResize->width(),editFormResize->height());
        this->setWindowState(Qt::WindowNoState);
    }
    editFormResize->close();
    disconnect(editFormResize,SIGNAL(editFinished(bool)),this,SLOT(resizeImageOvered(bool)));
    delete editFormResize;
}

/** Show editCrop window, hide this **/
void QImageViewer::cropImage()
{
    // init editCrop window //
    editFormCrop = new editformCrop(width(),height());
    editFormCrop->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Crop"])));
    connect(editFormCrop,SIGNAL(editFinished(bool)),this,SLOT(cropImageOvered(bool)));

    // set window size //
    if (this->windowState() == Qt::WindowMaximized) editFormCrop->setWindowState(Qt::WindowMaximized);
    else
    {
        editFormCrop->resize(this->width(),this->height());
        editFormCrop->setWindowState(Qt::WindowNoState);
    }


    // load new image to editForm //
    editFormCrop->loadImage(imagewidget->currentPixmap());

    // change active form //
    editFormCrop->show();
    this->hide();
}

/** Show this window, hide editCrop window **/
void QImageViewer::cropImageOvered(bool result)
{
    // change active form //
    if (!isfullScreenActive) this->show();
    // accept changes if 'Accept' button was pressed //
    if (result)
    {
        imagewidget->addToBuffer(editFormCrop->getpixmap());
        imagewidget->setEdited();
    }
    // resize window //
    if (editFormCrop->windowState() == Qt::WindowMaximized) this->setWindowState(Qt::WindowMaximized);
    else
    {
        this->resize(editFormCrop->width(),editFormCrop->height());
        this->setWindowState(Qt::WindowNoState);
    }
    // close editForm //
    editFormCrop->close();
    // destroy editForm, free memory //
    disconnect(editFormCrop,SIGNAL(editFinished(bool)),this,SLOT(cropImageOvered(bool)));
    delete editFormCrop;
}


/** Show editResize window, hide this **/
void QImageViewer::resizeImageList()
{
    if (!isResizeListActive)
    {
        // init editResize window //
        isResizeListActive = true;
        QStringList lst = previewwiget->selectedImages();
        if (lst.isEmpty()) return;
        editFormResizeElements = new editformResizeElements;
        editFormResizeElements->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ResizeItems"])));
        connect(editFormResizeElements,SIGNAL(editFinished(bool)),this,SLOT(resizeImageListOvered(bool)));

        editFormResizeElements->loadlist(lst,lastdirectory);
        if (this->windowState() == Qt::WindowMaximized) editFormResizeElements->setWindowState(Qt::WindowMaximized);
        else
        {
            editFormResizeElements->resize(this->width(),this->height());
            editFormResizeElements->setWindowState(Qt::WindowNoState);
        }
        editFormResizeElements->show();
        this->hide();
    }
}

/** Show this window, hide editResize window **/
void QImageViewer::resizeImageListOvered(bool result)
{
    if (!isfullScreenActive) this->show();
    if (result && editFormResizeElements->getLast() != QString::null)
    {
        if (editFormResizeElements->isSameFolder())
        {
            //Add new images to list, don't change current image
            defaultpath = editFormResizeElements->getLast();
            fileOpen();
//            setWindowTitle("QImageViewer - ["+QString::number(imagewidget->currentImage()+1)+
//                           tr(" of ") + QString::number(imagewidget->size()) + "] "
//                           + imagewidget->currentImageName());
        }
    }
    if (editFormResizeElements->windowState() == Qt::WindowMaximized) this->setWindowState(Qt::WindowMaximized);
    else
    {
        this->resize(editFormResizeElements->width(),editFormResizeElements->height());
        this->setWindowState(Qt::WindowNoState);
    }
    editFormResizeElements->close();
    disconnect(editFormResizeElements,SIGNAL(editFinished(bool)),this,SLOT(resizeImageListOvered(bool)));
    delete editFormResizeElements;
    isResizeListActive = false;
}

/** Show 'About' **/
void QImageViewer::helpAbout()
{
    aboutform = new aboutForm;
    aboutform->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Help"])));
    connect(aboutform,SIGNAL(overed()),this,SLOT(helpAboutOvered()));
    isAboutFormOpened = true;
    aboutform->show();
}

void QImageViewer::helpAboutOvered()
{
    isAboutFormOpened = false;
    disconnect(aboutform,SIGNAL(overed()),this,SLOT(helpAboutOvered()));
    aboutform->close();
    delete aboutform;
}


/** reload current image if window was resized **/
void QImageViewer::resizeEvent(QResizeEvent *)
{
    if (imagewidget->isReady())
        imagewidget->reloadImage();
}

void QImageViewer::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void QImageViewer::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    QString first = "";
    while (urls.size())
    {
        QString url = urls.first().toLocalFile();
        if ((url.endsWith("jpg",Qt::CaseInsensitive) || url.endsWith("png",Qt::CaseInsensitive) ||
             url.endsWith("bmp",Qt::CaseInsensitive) || url.endsWith("jpeg",Qt::CaseInsensitive) ||
             url.endsWith("ppm",Qt::CaseInsensitive) || url.endsWith("xbm",Qt::CaseInsensitive) ||
             url.endsWith("xbm",Qt::CaseInsensitive) || url.endsWith("tiff",Qt::CaseInsensitive) ||
             url.endsWith("pbm",Qt::CaseInsensitive) || url.endsWith("pgm",Qt::CaseInsensitive))
                && first.isEmpty())
        {
            first = url;
            qDebug() << "FIND!"<< url;
            break;
        }
        else
        {
            urls.removeFirst();
        }
    }
    if (!first.isEmpty())
    {
        defaultpath = first;
        fileOpen();
    }
}

/** Destructor **/
QImageViewer::~QImageViewer()
{
    delete ui;
}
