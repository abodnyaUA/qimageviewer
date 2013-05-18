#include "qimageviewer.h"
#include "ui_qimageviewer.h"

QImageViewer::QImageViewer(QString path, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QImageViewer)
{
    ui->setupUi(this);

    // init image //
    imagewidget = new image;
    isfullScreenActive = false;

    ui->verticalLayout->addWidget(imagewidget);
    //previewArea.setMinimumHeight(120);
    //previewArea.setWindowState(Qt::WindowMinimized);
    //ui->previewLayout->addWidget(&previewArea);

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

    ui->mainToolBar->hide();
    isSettingsActive = false;
    isEditosManagerActive = false;
    isEditorAddFormActive = false;
    //setWindowState(Qt::WindowMaximized);
}


void QImageViewer::setStatusName(bool arg)
{
    if (arg)
        setWindowTitle("QImageViewer - "+imagewidget->currentImageName());
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
    if( !path.isNull() )
    {
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
        ui->fullscreenAction->setEnabled(true);
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
    }
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
    for (int i=0;i<imagefiles.size();i++) imagefiles[i] = lastdirectory + '/' + imagefiles[i];
    imagewidget->loadimagelist(imagefiles);
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
    fullScreenWidget->setSlideshowInterval(slideshowInterval);
    fullScreenWidget->setSlideshowSmoothTransition(slideshowSmoothTransition);

    fullScreenWidget->setWindowState(Qt::WindowFullScreen);
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
    delete fullScreenWidget;
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

/** Open Settings Window **/
void QImageViewer::settingsWindow()
{
    if (!isSettingsActive)
    {
        isSettingsActive = true;
        settings = new Settings;
        connect(settings,SIGNAL(acceptsettings(QString,QString,bool,bool,bool,double,int,hotkeysStruct,isneedButStruct,QColor)),
                this,SLOT(updateSettings(QString,QString,bool,bool,bool,double,int,hotkeysStruct,isneedButStruct,QColor)));

        settings->setDefaultSettings(language,
                                     lastdirectory,
                                     imagewidget->getMouseZoom(),
                                     imagewidget->getMouseFullscreen(),
                                     slideshowSmoothTransition,
                                     slideshowInterval,
                                     panelalignment,
                                     hotkeys,isneedBut,fullscreencolor);
        settings->show();
    }
}

/** Update program settings **/
void QImageViewer::updateSettings(QString language,
                    QString defaultfolder,
                    bool mouseZoom, bool mouseFullscreen,
                    bool slideshowSmoothTransition, double slideshowInterval,
                    int panelalignment,hotkeysStruct hotkeys, isneedButStruct isneedNew,
                    QColor fullscreencolor)
{
    isSettingsActive = false;
    this->language = language;
    this->lastdirectory = defaultfolder;
    imagewidget->setMouseZoom(mouseZoom);
    imagewidget->setMouseFullscreen(mouseFullscreen);
    this->slideshowSmoothTransition = slideshowSmoothTransition;
    this->slideshowInterval = slideshowInterval;
    this->fullscreencolor = fullscreencolor;

    /// Language ///
    QString lng = language;
    if (lng == "rus") lng = "ru_RU";
    else if (lng == "ukr") lng = "uk_UA";
    else if (lng == "eng") lng = "en_US";
    else if (lng == "pol") lng = "pl_PL";
    else lng = QLocale::system().name();

    /// system locale for messageboxes ///
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + lng,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);

    /// Programm language ///
    QTranslator prTranslator;
    prTranslator.load(":/lng/qimageviewer_" + lng);
    qApp->installTranslator(&prTranslator);

    /// Panel Buttons ///
    // Erase memory for unused buttons //
    if (isneedBut.rotateLeft)
    {
        buttonsList.removeOne(butRotateLeft);
        delete butRotateLeft;
    }
    if (isneedBut.rotateRight)
    {
        buttonsList.removeOne(butRotateRight);
        delete butRotateRight;
    }
    if (isneedBut.flipHorizontal)
    {
        buttonsList.removeOne(butFlipHorizontal);
        delete butFlipHorizontal;
    }
    if (isneedBut.flipVertical)
    {
        buttonsList.removeOne(butFlipVertical);
        delete butFlipVertical;
    }
    if (isneedBut.zoomIn)
    {
        buttonsList.removeOne(butZoomIn);
        delete butZoomIn;
    }
    if (isneedBut.zoomOut)
    {
        buttonsList.removeOne(butZoomOut);
        delete butZoomOut;
    }
    if (isneedBut.zoomWindow)
    {
        buttonsList.removeOne(butZoomWindow);
        delete butZoomWindow;
    }
    if (isneedBut.zoomOriginal)
    {
        buttonsList.removeOne(butZoomOriginal);
        delete butZoomOriginal;
    }
    if (isneedBut.fullscreen)
    {
        buttonsList.removeOne(butFullscreen);
        delete butFullscreen;
    }
    if (isneedBut.slideshow)
    {
        buttonsList.removeOne(butSlideshow);
        delete butSlideshow;
    }
    if (isneedBut.properties)
    {
        buttonsList.removeOne(butProperties);
        delete butProperties;
    }

    // Sharing new memory to the objets //
    if (isneedNew.rotateLeft)
    {
        butRotateLeft = new QPushButton;
        butRotateLeft->setToolTip(tr("Rotate picture to the left"));
        connect(butRotateLeft,SIGNAL(clicked()),imagewidget,SLOT(rotateLeft()));
        butRotateLeft->setFocusPolicy(Qt::NoFocus);
        butRotateLeft->setIcon(QIcon(QPixmap(":/res/rotate-left.png")));
        if (!imagewidget->isReady()) butRotateLeft->setEnabled(false);
        buttonsList << butRotateLeft;
    }
    if (isneedNew.rotateRight)
    {
        butRotateRight = new QPushButton;
        butRotateRight->setToolTip(tr("Rotate picture to the right"));
        connect(butRotateRight,SIGNAL(clicked()),imagewidget,SLOT(rotateRight()));
        butRotateRight->setFocusPolicy(Qt::NoFocus);
        butRotateRight->setIcon(QIcon(QPixmap(":/res/rotate-right.png")));
        if (!imagewidget->isReady()) butRotateRight->setEnabled(false);
        buttonsList << butRotateRight;
    }
    if (isneedNew.flipHorizontal)
    {
        butFlipHorizontal = new QPushButton;
        butFlipHorizontal->setToolTip(tr("Change this image to horizontal mirror"));
        connect(butFlipHorizontal,SIGNAL(clicked()),imagewidget,SLOT(flipHorizontal()));
        butFlipHorizontal->setFocusPolicy(Qt::NoFocus);
        butFlipHorizontal->setIcon(QIcon(QPixmap(":/res/flip-horizontal.png")));
        if (!imagewidget->isReady()) butFlipHorizontal->setEnabled(false);
        buttonsList << butFlipHorizontal;
    }
    if (isneedNew.flipVertical)
    {
        butFlipVertical = new QPushButton;
        butFlipVertical->setToolTip(tr("Change this image to vertical mirror"));
        connect(butFlipVertical,SIGNAL(clicked()),imagewidget,SLOT(flipVertical()));
        butFlipVertical->setFocusPolicy(Qt::NoFocus);
        butFlipVertical->setIcon(QIcon(QPixmap(":/res/flip-vertical.png")));
        if (!imagewidget->isReady()) butFlipVertical->setEnabled(false);
        buttonsList << butFlipVertical;
    }
    if (isneedNew.zoomIn)
    {
        butZoomIn = new QPushButton;
        butZoomIn->setToolTip(tr("Zoom In"));
        connect(butZoomIn,SIGNAL(clicked()),imagewidget,SLOT(zoomInc()));
        butZoomIn->setFocusPolicy(Qt::NoFocus);
        butZoomIn->setIcon(QIcon(QPixmap(":/res/zoom-in.png")));
        if (!imagewidget->isReady()) butZoomIn->setEnabled(false);
        buttonsList << butZoomIn;
    }
    if (isneedNew.zoomOut)
    {
        butZoomOut = new QPushButton;
        butZoomOut->setToolTip(tr("Zoom Out"));
        connect(butZoomOut,SIGNAL(clicked()),imagewidget,SLOT(zoomDec()));
        butZoomOut->setFocusPolicy(Qt::NoFocus);
        butZoomOut->setIcon(QIcon(QPixmap(":/res/zoom-out.png")));
        if (!imagewidget->isReady()) butZoomOut->setEnabled(false);
        buttonsList << butZoomOut;
    }
    if (isneedNew.zoomWindow)
    {
        butZoomWindow = new QPushButton;
        butZoomWindow->setToolTip(tr("Zoom to window size"));
        connect(butZoomWindow,SIGNAL(clicked()),imagewidget,SLOT(reloadImage()));
        butZoomWindow->setFocusPolicy(Qt::NoFocus);
        butZoomWindow->setIcon(QIcon(QPixmap(":/res/zoom-window.png")));
        if (!imagewidget->isReady()) butZoomWindow->setEnabled(false);
        buttonsList << butZoomWindow;
    }
    if (isneedNew.zoomOriginal)
    {
        butZoomOriginal = new QPushButton;
        butZoomOriginal->setToolTip(tr("Zoom to original size"));
        connect(butZoomOriginal,SIGNAL(clicked()),imagewidget,SLOT(setOriginalSize()));
        butZoomOriginal->setFocusPolicy(Qt::NoFocus);
        butZoomOriginal->setIcon(QIcon(QPixmap(":/res/zoom-original.png")));
        if (!imagewidget->isReady()) butZoomOriginal->setEnabled(false);
        buttonsList << butZoomOriginal;
    }
    if (isneedNew.fullscreen)
    {
        butFullscreen = new QPushButton;
        butFullscreen->setToolTip(tr("Enable fullscreen mode"));
        connect(butFullscreen,SIGNAL(clicked()),this,SLOT(fullScreen()));
        butFullscreen->setFocusPolicy(Qt::NoFocus);
        butFullscreen->setIcon(QIcon(QPixmap(":/res/fullscreen.png")));
        if (!imagewidget->isReady()) butFullscreen->setEnabled(false);
        buttonsList << butFullscreen;
    }
    if (isneedNew.slideshow)
    {
        butSlideshow = new QPushButton;
        butSlideshow->setToolTip(tr("Start slideshow in fullscreen mode"));
        connect(butSlideshow,SIGNAL(clicked()),this,SLOT(slideShow()));
        butSlideshow->setFocusPolicy(Qt::NoFocus);
        butSlideshow->setIcon(QIcon(QPixmap(":/res/slideshow.png")));
        if (!imagewidget->isReady()) butSlideshow->setEnabled(false);
        buttonsList << butSlideshow;
    }
    if (isneedNew.properties)
    {
        butProperties = new QPushButton;
        butProperties->setToolTip(tr("Image properties"));
        connect(butProperties,SIGNAL(clicked()),imagewidget,SLOT(viewProperties()));
        butProperties->setFocusPolicy(Qt::NoFocus);
        butProperties->setIcon(QIcon(QPixmap(":/res/file-properties.png")));
        if (!imagewidget->isReady()) butProperties->setEnabled(false);
        buttonsList << butProperties;
    }
    isneedBut = isneedNew;

    /// Panel Aligment ///
    // Erase old panel //
    if (this->panelalignment == 0)
    {
        ui->panelBottomLayout->removeItem(spacerLeft);
        ui->panelBottomLayout->removeItem(spacerRight);
        for (int i=0;i<buttonsList.size();i++)
            ui->panelBottomLayout->removeWidget(buttonsList[i]);
    }
    else if (this->panelalignment == 1)
    {
        ui->panelTopLayout->removeItem(spacerLeft);
        ui->panelTopLayout->removeItem(spacerRight);
        for (int i=0;i<buttonsList.size();i++)
            ui->panelTopLayout->removeWidget(buttonsList[i]);
    }

    // Adding elements to new panel //
    if (panelalignment == 0)
    {
        if (this->panelalignment == 2)
            for (int i=0;i<buttonsList.size();i++)
                buttonsList[i]->show();

        ui->panelBottomLayout->addSpacerItem(spacerLeft);
        for (int i=0;i<buttonsList.size();i++)
            ui->panelBottomLayout->addWidget(buttonsList[i]);
        ui->panelBottomLayout->addSpacerItem(spacerRight);
    }
    else if (panelalignment == 1)
    {
        if (this->panelalignment == 2)
            for (int i=0;i<buttonsList.size();i++)
                buttonsList[i]->show();

        ui->panelTopLayout->addSpacerItem(spacerLeft);
        for (int i=0;i<buttonsList.size();i++)
            ui->panelTopLayout->addWidget(buttonsList[i]);
        ui->panelTopLayout->addSpacerItem(spacerRight);
    }
    else if (panelalignment == 2)
    {
        for (int i=0;i<buttonsList.size();i++)
            buttonsList[i]->hide();
    }
    this->panelalignment = panelalignment;

    /// Hotkeys ///
    this->hotkeys = hotkeys;
    createHotkeys();

    disconnect(settings,SIGNAL(acceptsettings(QString,QString,bool,bool,bool,double,int,hotkeysStruct,isneedButStruct,QColor)),
            this,SLOT(updateSettings(QString,QString,bool,bool,bool,double,int,hotkeysStruct,isneedButStruct,QColor)));
    delete settings;
}

/** Show editResize window, hide this **/
void QImageViewer::resizeImage()
{
    // init editResize window //
    editFormResize = new editformResize;
    connect(editFormResize,SIGNAL(editFinished(bool)),this,SLOT(resizeImageOvered(bool)));

    editFormResize->loadImage(imagewidget->currentPixmap());
    if (this->windowState() == Qt::WindowMaximized) editFormResize->setWindowState(Qt::WindowMaximized);
    else
    {
        editFormResize->resize(this->width(),this->height());
        editFormResize->setWindowState(Qt::WindowNoState);
    }
    editFormResize->show();
    this->hide();
}

/** Show this window, hide editResize window **/
void QImageViewer::resizeImageOvered(bool result)
{
    this->show();
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
    editFormCrop = new editformCrop;
    connect(editFormCrop,SIGNAL(editFinished(bool)),this,SLOT(cropImageOvered(bool)));

    // load new image to editForm //
    editFormCrop->loadImage(imagewidget->currentPixmap());//imagewidget->currentImageName());

    // set window size //
    if (this->windowState() == Qt::WindowMaximized) editFormCrop->setWindowState(Qt::WindowMaximized);
    else
    {
        editFormCrop->resize(this->width(),this->height());
        editFormCrop->setWindowState(Qt::WindowNoState);
    }

    // change active form //
    editFormCrop->show();
    this->hide();
}

/** Show this window, hide editCrop window **/
void QImageViewer::cropImageOvered(bool result)
{
    // change active form //
    this->show();
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
    // init editResize window //
    editFormResizeElements = new editformResizeElements;
    connect(editFormResizeElements,SIGNAL(editFinished(bool)),this,SLOT(resizeImageListOvered(bool)));

    editFormResizeElements->loadlist(imagewidget->getImageList(),lastdirectory,imagewidget->currentImage());
    if (this->windowState() == Qt::WindowMaximized) editFormResizeElements->setWindowState(Qt::WindowMaximized);
    else
    {
        editFormResizeElements->resize(this->width(),this->height());
        editFormResizeElements->setWindowState(Qt::WindowNoState);
    }
    editFormResizeElements->show();
    this->hide();
}

/** Show this window, hide editResize window **/
void QImageViewer::resizeImageListOvered(bool result)
{
    this->show();
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
}

void QImageViewer::newExternEditor()
{
    if (!isEditorAddFormActive)
    {
        editorAddForm = new QExternProgramAddForm;
        connect(editorAddForm,SIGNAL(accept(QString,QString,QString)),this,SLOT(addEditor(QString,QString,QString)));
        connect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
        editorAddForm->show();
        isEditorAddFormActive = true;
    }
}

void QImageViewer::addEditor(QString name, QString icon, QString command)
{
    QExternProgram *editor = new QExternProgram(name,icon,command,imagewidget);
    editors.append(editor);
    QAction * action = new QAction(QIcon(QPixmap(icon)),name,this);
    editorsActions.append(action);
    connect(action,SIGNAL(triggered()),editor,SLOT(exec()));
    ui->menuExtern_editors->insertAction(ui->editorsNewAction,action);
    if (!imagewidget->isReady()) action->setEnabled(false);

    qDebug() << name << icon <<command;
    disconnect(editorAddForm,SIGNAL(accept(QString,QString,QString)),this,SLOT(addEditor(QString,QString,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}

void QImageViewer::abortAddingNewExternEditor()
{
    disconnect(editorAddForm,SIGNAL(accept(QString,QString,QString)),this,SLOT(addEditor(QString,QString,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));

    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}

/** Show Extern editors manager **/
void QImageViewer::exterEditorsManager()
{
    if (!isEditosManagerActive)
    {
        isEditosManagerActive = true;
        editorsManager = new QExternProgramManager(editors,imagewidget);
        connect(editorsManager,SIGNAL(overed(bool)),this,SLOT(exterEditorsManagerOvered(bool)));

        editorsManager->show();
    }
}

/** Close Extern editors manager **/
void QImageViewer::exterEditorsManagerOvered(bool result)
{
    isEditosManagerActive = false;

    if (result)
    {
        // Erase old links//
        for (int i=0;i<editorsActions.size();i++)
        {
            ui->menuExtern_editors->removeAction(editorsActions[i]);
            disconnect(editorsActions[i],SIGNAL(triggered()),editors[i],SLOT(exec()));
        }
        editorsActions.clear();

        editors = editorsManager->editors;
        // Add new links //
        for (int i=0;i<editors.size();i++)
        {
            QAction * action = new QAction(QIcon(QPixmap(editors[i]->icon)),editors[i]->name,this);
            editorsActions.append(action);
            connect(action,SIGNAL(triggered()),editors[i],SLOT(exec()));
            ui->menuExtern_editors->insertAction(ui->editorsNewAction,action);
            if (!imagewidget->isReady()) action->setEnabled(false);
        }
    }
    editorsManager->close();
    disconnect(editorsManager,SIGNAL(overed(bool)),this,SLOT(exterEditorsManagerOvered(bool)));
    delete editorsManager;
}

/** Show editResize window, hide this **/
void QImageViewer::imageshackShare()
{
    ImageShackUploader * newimage = new ImageShackUploader(imagewidget->currentImageName());
    imageshack.append(newimage);
    newimage->show();
}

/** Show 'About' **/
void QImageViewer::helpAbout()
{
    QMessageBox::about(this, tr("About"),
        tr("QImageViewer 0.1.7\n\n"
           "This program is using for viewing pictures\n"
           "It was created with using Qt 5.0.2\n\n"
           "Program author is Bodnya Alexey\n"
           "Ukraine, Kiev, KPI, TV-11\n"
           "This program is under license GPLv2\n"
           "It's free to use and all sources are open.\n"
           "\n"
           "Thanks for testing and helping with tips and ideas to\n"
           "my friends, Typical Proger community and Prog.org.ru community!"));
}


/** reload current image if window was resized **/
void QImageViewer::resizeEvent(QResizeEvent *)
{
    if (imagewidget->isReady())
        imagewidget->reloadImage();
}

/** Destructor **/
QImageViewer::~QImageViewer()
{
    delete ui;
}
