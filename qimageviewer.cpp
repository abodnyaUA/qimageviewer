#include "qimageviewer.h"
#include "ui_qimageviewer.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

void QImageViewer::loadsettings()
{
    QFile file("settings.txt");
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /** DEFAULT SETTINGS **/
        QString uname;
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            /// LANGUAGE ///
            out << "LANGUAGE=sys\n";
            language = "sys";

            /// Default folder ///
            out << "DEFAULT_FOLDER=";
    #ifdef Q_OS_WIN32
            uname = QString::fromLocal8Bit( getenv("USER") );
            out << "C:\\Users\\";
            lastdirectory = "C:\\Users\\"+uname;
    #else
            uname = QString::fromLocal8Bit( getenv("USER") );
            out << "/home/";
            lastdirectory = "/home/"+uname;
    #endif
            out << uname;
            out << "\n";

            /// Enable/Disable fullscreen mode by double-click mouse ///
            out << "MOUSE_FULLSCREEN=1\n";
            imagewidget->setMouseFullscreen(true);

            /// Enable/Disable zooming by mouse ///
            out << "MOUSE_ZOOM=1\n";
            imagewidget->setMouseZoom(true);

            /// Slideshow ///
            out << "SLIDESHOW_TRANSITION=0\n";
            slideshowSmoothTransition = false;
            out << "SLIDESHOW_INTERVAL=1\n";
            slideshowInterval = 1;

            /// Hotkeys ///
            out << "\n#Hotkeys\n";
            cropHotkey = "Ctrl+Shift+C";
            out << "Crop="<<cropHotkey<<"\n";
            resizeHotkey = "Ctrl+R";
            out << "Resize="<<resizeHotkey<<"\n";
            fullscreenHotkey = "F10";
            out << "Fullscreen="<<fullscreenHotkey<<"\n";
            slideshowHotkey = "F5";
            out << "Slideshow="<<slideshowHotkey<<"\n";
            undoHotkey = "Ctrl+Z";
            out << "Undo="<<undoHotkey<<"\n";
            redoHotkey = "Ctrl+Shift+Z";
            out << "Redo="<<redoHotkey<<"\n";
        }
    }
    else
    {
        /** LOAD EXIST SETTINGS **/
        /// Language ///
        QString sets;
        sets = out.readLine();
        language = sets.right(sets.size()-9);

        /// Default folder ///
        sets = out.readLine();
        lastdirectory = sets.right(sets.size()-15);

        /// Enable/Disable fullscreen mode by double-click mouse ///
        sets = out.readLine();
        if (sets[sets.size()-1] == '1')
            imagewidget->setMouseFullscreen(true);
        else imagewidget->setMouseFullscreen(false);

        /// Enable/Disable zooming by mouse ///
        sets = out.readLine();
        if (sets[sets.size()-1] == '1')
            imagewidget->setMouseZoom(true);
        else imagewidget->setMouseZoom(false);

        /// Slideshow ///
        sets = out.readLine();
        if (sets[sets.size()-1] == '1')
            slideshowSmoothTransition = true;
        else slideshowSmoothTransition = false;

        sets = out.readLine();
        slideshowInterval = sets.right(sets.size()-19).toInt();

        /// Hotkeys ///
        sets = out.readLine();
        sets = out.readLine();
        sets = out.readLine();
        cropHotkey = sets.right(sets.size()-5);
        sets = out.readLine();
        resizeHotkey = sets.right(sets.size()-7);
        sets = out.readLine();
        fullscreenHotkey = sets.right(sets.size()-11);
        sets = out.readLine();
        slideshowHotkey = sets.right(sets.size()-10);
        sets = out.readLine();
        undoHotkey = sets.right(sets.size()-5);
        sets = out.readLine();
        redoHotkey = sets.right(sets.size()-5);
    }
    file.close();
}

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
    // Shorctcuts, signals-slots and other stuff for buttons and menu actions //
    createActions();
    // Icons and other design for buttons and menu actions //
    createDesign();

    // Default open or open argv[1] image //
    if (path == "\0")
    {
        defaultpath = QString::null;
        imagewidget->loadimage(":/res/logo.png");
        defaultpath = QString::null;
        ui->image_amount_label->setText("0");
        ui->image_currentIndex_label->setText("0");
        imagewidget->setImage(0);
        setWindowTitle("QImageViewer");
    }
    else
    {
        defaultpath = path;
        fileOpen();
    }


    ui->mainToolBar->hide();
    //ui->mainToolBar->set
}

/** Set all shortcuts, signal-slots and statustips for buttons and menu actions **/
void QImageViewer::createActions()
{
    ///ACTIONS///
    // File //
    ui->openAction->setShortcut(tr("Ctrl+O"));
    ui->openAction->setStatusTip(tr("Open current image"));
    connect(ui->openAction,SIGNAL(triggered()),this,SLOT(fileOpen()));

    ui->saveAction->setShortcut(tr("Ctrl+S"));
    ui->saveAction->setStatusTip(tr("Save current image with same name and format"));
    connect(ui->saveAction,SIGNAL(triggered()),this,SLOT(fileSave()));
    connect(imagewidget,SIGNAL(itsSaved(bool)),this,SLOT(setStatusName(bool)));

    ui->saveAsAction->setShortcut(tr("Ctrl+Shift+S"));
    ui->saveAsAction->setStatusTip(tr("Save current image with new name and format"));
    connect(ui->saveAsAction,SIGNAL(triggered()),this,SLOT(fileSaveAs()));

    ui->settingsAction->setStatusTip(tr("Program settings"));
    connect(ui->settingsAction,SIGNAL(triggered()),this,SLOT(settingsWindow()));

    ui->exitAction->setShortcut(tr("Ctrl+Q"));
    ui->exitAction->setStatusTip(tr("Close program"));
    connect(ui->exitAction,SIGNAL(triggered()),this,SLOT(close()));

    // Edit //
    ui->actionUndo->setShortcut(undoHotkey);
    ui->actionUndo->setStatusTip(tr("Cancel last changes"));
    connect(ui->actionUndo,SIGNAL(triggered()),imagewidget,SLOT(prevBuffer()));
    connect(imagewidget,SIGNAL(itsPossibleToUndo(bool)),this,SLOT(setUndoEnable(bool)));

    ui->actionRedo->setShortcut(redoHotkey);
    ui->actionRedo->setStatusTip(tr("Do last changes"));
    connect(ui->actionRedo,SIGNAL(triggered()),imagewidget,SLOT(nextBuffer()));
    connect(imagewidget,SIGNAL(itsPossibleToRedo(bool)),this,SLOT(setRedoEnable(bool)));

    ui->rotateLeftAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_L));
    ui->rotateLeftAction->setStatusTip(tr("Rotate image to the left"));
    connect(ui->rotateLeftAction,SIGNAL(triggered()),imagewidget,SLOT(rotateLeft()));

    ui->rotateRightAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R));
    ui->rotateRightAction->setStatusTip(tr("Rotate image to the right"));
    connect(ui->rotateRightAction,SIGNAL(triggered()),imagewidget,SLOT(rotateRight()));

    ui->deleteFileAction->setShortcut(QKeySequence(Qt::Key_Delete));
    ui->deleteFileAction->setStatusTip(tr("Delete current image"));
    connect(ui->deleteFileAction,SIGNAL(triggered()),imagewidget,SLOT(deleteCurrentItem()));

    ui->resizeAction->setShortcut(resizeHotkey);
    ui->resizeAction->setStatusTip(tr("Resize current image"));
    connect(ui->resizeAction,SIGNAL(triggered()),this,SLOT(resizeImage()));

    ui->cropAction->setShortcut(cropHotkey);
    ui->cropAction->setStatusTip(tr("Crop current image"));
    connect(ui->cropAction,SIGNAL(triggered()),this,SLOT(cropImage()));

    // Watching //
    ui->fullscreenAction->setShortcut(fullscreenHotkey);
    ui->fullscreenAction->setStatusTip(tr("Enable fullscreen mode"));
    connect(ui->fullscreenAction,SIGNAL(triggered()),this,SLOT(fullScreen()));
    connect(imagewidget,SIGNAL(needFullscreen()),this,SLOT(fullScreenFromImage()));

    ui->slideshowAction->setShortcut(slideshowHotkey);
    ui->slideshowAction->setStatusTip(tr("Start slideshow in fullscreen mode"));
    connect(ui->slideshowAction,SIGNAL(triggered()),this,SLOT(slideShow()));
    connect(imagewidget,SIGNAL(needSlideshow()),this,SLOT(slideShow()));

    ui->wallpaperAction->setStatusTip(tr("Set picture as wallpaper"));
    connect(ui->wallpaperAction,SIGNAL(triggered()),imagewidget,SLOT(setAsWallpaper()));

    ui->zoomInAction->setStatusTip(tr("Zoom in"));
    connect(ui->zoomInAction,SIGNAL(triggered()),imagewidget,SLOT(zoomInc()));

    ui->zoomOutAction->setStatusTip(tr("Zoom out"));
    connect(ui->zoomOutAction,SIGNAL(triggered()),imagewidget,SLOT(zoomDec()));

    ui->zoomOriginalAction->setStatusTip(tr("Zoom to original size"));
    connect(ui->zoomOriginalAction,SIGNAL(triggered()),imagewidget,SLOT(setOriginalSize()));

    ui->zoomWindowAction->setStatusTip(tr("Zoom to window size"));
    connect(ui->zoomWindowAction,SIGNAL(triggered()),imagewidget,SLOT(reloadImage()));

    // Help //
    ui->aboutAction->setShortcut(tr("F1"));
    ui->aboutAction->setStatusTip(tr("Information about program"));
    connect(ui->aboutAction,SIGNAL(triggered()),this,SLOT(helpAbout()));

    // set unEnabled all actions for default run //
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->saveAction->setEnabled(false);
    ui->saveAsAction->setEnabled(false);
    ui->rotateLeftAction->setEnabled(false);
    ui->rotateRightAction->setEnabled(false);
    ui->deleteFileAction->setEnabled(false);
    ui->resizeAction->setEnabled(false);
    ui->fullscreenAction->setEnabled(false);
    ui->slideshowAction->setEnabled(false);
    ui->wallpaperAction->setEnabled(false);
    ui->cropAction->setEnabled(false);

    ///BUTTONS///
    ui->prevButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    ui->prevButton->setToolTip(tr("Open previous image"));
    connect(ui->prevButton,SIGNAL(clicked()),this,SLOT(prevImage()));

    ui->nextButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    ui->nextButton->setToolTip(tr("Open next image"));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(nextImage()));

    ui->fullscreenButton->setToolTip(tr("Enable fullscreen mode"));
    connect(ui->fullscreenButton,SIGNAL(clicked()),this,SLOT(fullScreen()));

    ui->zoomIncButton->setShortcut(tr("+"));
    ui->zoomIncButton->setToolTip(tr("Zoom in image"));
    connect(ui->zoomIncButton,SIGNAL(clicked()),imagewidget,SLOT(zoomInc()));

    ui->zoomDecButton->setShortcut(tr("-"));
    ui->zoomDecButton->setToolTip(tr("Zoom out image"));
    connect(ui->zoomDecButton,SIGNAL(clicked()),imagewidget,SLOT(zoomDec()));

    ui->rotateLeftButton->setToolTip(tr("Rotate picture to the left"));
    connect(ui->rotateLeftButton,SIGNAL(clicked()),imagewidget,SLOT(rotateLeft()));

    ui->rotateRightButton->setToolTip(tr("Rotate picture to the right"));
    connect(ui->rotateRightButton,SIGNAL(clicked()),imagewidget,SLOT(rotateRight()));

    //Changing image
    connect(imagewidget,SIGNAL(currentImageWasChanged(int)),this,SLOT(currentIndexWasChanged(int)));

    // set unEnabled all buttons for default run //
    ui->prevButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
    ui->fullscreenButton->setEnabled(false);
    ui->zoomDecButton->setEnabled(false);
    ui->zoomIncButton->setEnabled(false);
    ui->rotateLeftButton->setEnabled(false);
    ui->rotateRightButton->setEnabled(false);

    // set nofocus on buttons //
    ui->prevButton->setFocusPolicy(Qt::NoFocus);
    ui->nextButton->setFocusPolicy(Qt::NoFocus);
    ui->fullscreenButton->setFocusPolicy(Qt::NoFocus);
    ui->zoomDecButton->setFocusPolicy(Qt::NoFocus);
    ui->zoomIncButton->setFocusPolicy(Qt::NoFocus);
    ui->rotateLeftButton->setFocusPolicy(Qt::NoFocus);
    ui->rotateRightButton->setFocusPolicy(Qt::NoFocus);
}

/** Loading all icons and design for buttons and menu actions **/
void QImageViewer::createDesign()
{
    ui->saveAction->setIcon(QIcon(QPixmap(":/res/file-save.png")));
    ui->openAction->setIcon(QIcon(QPixmap(":/res/file-open.png")));
    ui->settingsAction->setIcon(QIcon(QPixmap(":/res/settings.png")));
    ui->rotateLeftAction->setIcon(QIcon(QPixmap(":/res/rotate-left.png")));
    ui->rotateRightAction->setIcon(QIcon(QPixmap(":/res/rotate-right.png")));
    ui->deleteFileAction->setIcon(QIcon(QPixmap(":/res/delete.png")));
    ui->resizeAction->setIcon(QIcon(QPixmap(":/res/resize.png")));
    ui->cropAction->setIcon(QIcon(QPixmap(":/res/crop.png")));
    ui->zoomInAction->setIcon(QIcon(QPixmap(":/res/zoom-in.png")));
    ui->zoomOutAction->setIcon(QIcon(QPixmap(":/res/zoom-out.png")));
    ui->zoomOriginalAction->setIcon(QIcon(QPixmap(":/res/zoom-original.png")));
    ui->zoomWindowAction->setIcon(QIcon(QPixmap(":/res/zoom-window.png")));
    ui->fullscreenAction->setIcon(QIcon(QPixmap(":/res/fullscreen.png")));
    ui->slideshowAction->setIcon(QIcon(QPixmap(":/res/slideshow.png")));
    ui->aboutAction->setIcon(QIcon(QPixmap(":/res/help.png")));

    ui->prevButton->setIcon(QIcon(QPixmap(":/res/prev.png")));
    ui->nextButton->setIcon(QIcon(QPixmap(":/res/next.png")));
    ui->fullscreenButton->setIcon(QIcon(QPixmap(":/res/fullscreen.png")));
    ui->zoomDecButton->setIcon(QIcon(QPixmap(":/res/zoom-out.png")));
    ui->zoomIncButton->setIcon(QIcon(QPixmap(":/res/zoom-in.png")));
    ui->rotateLeftButton->setIcon(QIcon(QPixmap(":/res/rotate-left.png")));
    ui->rotateRightButton->setIcon(QIcon(QPixmap(":/res/rotate-right.png")));
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
    ui->image_amount_label->setText(QString::number(imagewidget->size()));
    ui->image_currentIndex_label->setText(QString::number(indx+1));
    setWindowTitle("QImageViewer - "+imagewidget->currentImageName());
    statusBar()->showMessage(QString::number(imagewidget->currentPixmap().width())+
                              QString("x")+
                              QString::number(imagewidget->currentPixmap().height()));
}

/** Show previous image by button "Previous" or shortcut Crtl+Left **/
void QImageViewer::prevImage()
{
    if (imagewidget->currentImage() > 0)
    {
        imagewidget->setImage(imagewidget->currentImage()-1);
        ui->image_currentIndex_label->setText(QString::number(imagewidget->currentImage()+1));
    }
    else
    {
        imagewidget->setImage(imagewidget->size()-1);
        ui->image_currentIndex_label->setText(QString::number(imagewidget->currentImage()+1));
    }
    setWindowTitle("QImageViewer - "+imagewidget->currentImageName());
}

/** Show next image by button "Next" or shortcut Crtl+Right **/
void QImageViewer::nextImage()
{
    if (imagewidget->currentImage() < (imagewidget->size()-1))
    {
        imagewidget->setImage(imagewidget->currentImage()+1);
        ui->image_currentIndex_label->setText(QString::number(imagewidget->currentImage()+1));
    }
    else
    {
        imagewidget->setImage(0);
        ui->image_currentIndex_label->setText(QString::number(imagewidget->currentImage()+1));
    }
    setWindowTitle("QImageViewer - "+imagewidget->currentImageName());
}

/** Show next image by shortcut 'Right arrow' **/
void QImageViewer::nextImageArrows()
{
    if (!imagewidget->isZoomed()) nextImage();
}

/** Show previous image by shortcut 'Left arrow' **/
void QImageViewer::prevImageArrows()
{
    if (!imagewidget->isZoomed()) prevImage();
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
    // Remember last directory //
    dialog->selectFile(path);
    lastdirectory = dialog->directory().path();

    imagewidget->loadimage(path);
    setWindowTitle("QImageViewer - "+path);


    ui->saveAction->setEnabled(true);
    ui->saveAsAction->setEnabled(true);
    ui->rotateLeftAction->setEnabled(true);
    ui->rotateRightAction->setEnabled(true);
    ui->deleteFileAction->setEnabled(true);
    ui->resizeAction->setEnabled(true);
    ui->fullscreenAction->setEnabled(true);
    ui->slideshowAction->setEnabled(true);
    ui->wallpaperAction->setEnabled(true);
    ui->cropAction->setEnabled(true);

    ui->prevButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
    ui->fullscreenButton->setEnabled(true);
    ui->zoomDecButton->setEnabled(true);
    ui->zoomIncButton->setEnabled(true);
    ui->rotateLeftButton->setEnabled(true);
    ui->rotateRightButton->setEnabled(true);

    // find other pictures in folder //
    filesFind();
    ui->image_amount_label->setText(QString::number(imagewidget->size()));
    ui->image_currentIndex_label->setText(QString::number(imagewidget->currentImage()+1));
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
        imagewidget->saveimage(path);
        imagewidget->setSaved();
        dialog->selectFile(path);
        qDebug() << lastdirectory;
        qDebug() << dialog->directory().path();
        if (lastdirectory == dialog->directory().path())
        {
            imagewidget->insertImage(path,imagewidget->currentImage()+1);
            nextImage();
            ui->image_amount_label->setText(QString::number(imagewidget->size()));
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
    fullScreenWidget = new fullscreen(imagewidget);
    connect(fullScreenWidget,SIGNAL(fullscreenEnded()),this,SLOT(fullScreenOvered()));
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

    // init fullscreen mode //
    disconnect(fullScreenWidget,SIGNAL(fullscreenEnded()),this,SLOT(fullScreenOvered()));
    delete fullScreenWidget;
}

/** Start slideshow **/
void QImageViewer::slideShow()
{
    fullScreen();
    fullScreenWidget->startSlideShow();
}

/** Open Settings Window **/
void QImageViewer::settingsWindow()
{
    //settings = new Settings;

    settings = new Settings;
    connect(settings,SIGNAL(acceptsettings(QString,QString,bool,bool,bool,double,
                                           QString,QString,QString,QString,QString,QString)),
            this,SLOT(updateSettings(QString,QString,bool,bool,bool,double,
                                     QString,QString,QString,QString,QString,QString)));

    settings->setDefaultSettings(language,
                                 lastdirectory,
                                 imagewidget->getMouseZoom(),
                                 imagewidget->getMouseFullscreen(),
                                 slideshowSmoothTransition,
                                 slideshowInterval,
                                 cropHotkey, resizeHotkey,
                                 fullscreenHotkey, slideshowHotkey,
                                 undoHotkey, redoHotkey);
    settings->show();
}

/** Update program settings **/
void QImageViewer::updateSettings(QString language,
                    QString defaultfolder,
                    bool mouseZoom, bool mouseFullscreen,
                    bool slideshowSmoothTransition, double slideshowInterval,
                                  QString cropHotkey, QString resizeHotkey,
                                  QString fullscreenHotkey, QString slideshowHotkey,
                                  QString undoHotkey, QString redoHotkey)
{
    this->language = language;
    this->lastdirectory = defaultfolder;
    imagewidget->setMouseZoom(mouseZoom);
    imagewidget->setMouseFullscreen(mouseFullscreen);
    this->slideshowSmoothTransition = slideshowSmoothTransition;
    this->slideshowInterval = slideshowInterval;

    this->cropHotkey = cropHotkey;
    this->resizeHotkey = resizeHotkey;
    this->fullscreenHotkey = fullscreenHotkey;
    this->slideshowHotkey = slideshowHotkey;
    this->undoHotkey = undoHotkey;
    this->redoHotkey = redoHotkey;
    disconnect(settings,SIGNAL(acceptsettings(QString,QString,bool,bool,bool,double,
                                              QString,QString,QString,QString,QString,QString)),
            this,SLOT(updateSettings(QString,QString,bool,bool,bool,double,
                                     QString,QString,QString,QString,QString,QString)));
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

/** Show 'About' **/
void QImageViewer::helpAbout()
{
    QMessageBox::about(this, tr("About"),
        tr("This program is using for viewing pictures\n"
           "It was created with using Qt 5.0.0\n\n"
           "Program author is Bodnya Alexey\n"
           "Ukraine, Kiev, KPI, TV-11\n"
           "This program is under license GPLv2\n"
           "It's free to use and all sources are open"));
}

/** event for closing program,save file changes if need **/
void QImageViewer::closeEvent(QCloseEvent *event)
{
    QFile file("settings.txt");
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    QString directory = lastdirectory;
#ifdef Q_OS_WIN32
    for (int i=1;i<directory.size();i++)
    {
        if (directory[i-1] == '/')
        {
            directory.remove(i-1,1);
            directory.insert(i-1,"\\\\");
            i++;
        }
    }
#endif
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    out.seek(0);
    out << "LANGUAGE="<<language<<"\n";
    out << "DEFAULT_FOLDER=" << directory << "\n";

    /// Enable/Disable fullscreen mode by double-click mouse ///
    out << "MOUSE_FULLSCREEN=" << (int)imagewidget->getMouseFullscreen() << "\n";

    /// Enable/Disable zooming by mouse ///
    out << "MOUSE_ZOOM=" << (int)imagewidget->getMouseZoom() << "\n";

    /// Slideshow ///
    out << "SLIDESHOW_TRANSITION=" << (int)slideshowSmoothTransition << "\n";;
    out << "SLIDESHOW_INTERVAL=" << slideshowInterval << "\n";

    /// Hotkeys ///
    out << "\n#Hotkeys\n";
    out << "Crop="<<cropHotkey<<"\n";
    out << "Resize="<<resizeHotkey<<"\n";
    out << "Fullscreen="<<fullscreenHotkey<<"\n";
    out << "Slideshow="<<slideshowHotkey<<"\n";
    out << "Undo="<<undoHotkey<<"\n";
    out << "Redo="<<redoHotkey<<"\n";

    file.close();

    if (!imagewidget->isSaved())
    {
        int r = QMessageBox::warning(this, tr("Warning!"),
                                        tr("This file was changed\n"
                                           "Do you want to save changes?"),
                                        QMessageBox::No,
                                        QMessageBox::Yes | QMessageBox::Default,
                                        QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
        if (r == QMessageBox::Yes)
            imagewidget->saveimage(imagewidget->currentImageName());
    }
    event->accept();
}

/** reload current image if window was resized **/
void QImageViewer::resizeEvent(QResizeEvent *)
{
    if (imagewidget->isReady())
        imagewidget->reloadImage();
    //->setGeometry(QRect(ui->previewLayout->geometry().x(),height()-200,width()-20,120));
}

/** Keyboard event, 'left' and 'right' arrows for changing pictures **/
bool QImageViewer::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && imagewidget->isReady() && !imagewidget->isZoomed())
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int k = keyEvent->key();
        //LEFT KEY
        if ((int)k == (int)Qt::Key_Left)
        {
            prevImageArrows();
            return true;
        }
        //RIGHT KEY
        if ((int)k == (int)Qt::Key_Right)
        {
            nextImageArrows();
            return true;
        }
        return false;
    }
    return false;
}

/** Destructor **/
QImageViewer::~QImageViewer()
{
    delete ui;
}
