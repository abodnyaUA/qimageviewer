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
            out << "PANEL=0\n";
            panelalignment = 0;

            /// Hotkeys ///
            //File//
            out << "\n#Hotkeys\n";
            hotkeys.fileOpen = "Ctrl+O";
            out << "File-Open="<<hotkeys.fileOpen << "\n";
            hotkeys.fileSave = "Ctrl+S";
            out << "File-Save="<<hotkeys.fileSave << "\n";
            hotkeys.fileSaveAs = "Ctrl+Shift+S";
            out << "File-Save-As="<<hotkeys.fileSaveAs << "\n";
            hotkeys.fileSettings = "";
            out << "File-Settings="<<hotkeys.fileSettings << "\n";
            hotkeys.fileQuit = "Ctrl+Q";
            out << "File-Quit="<<hotkeys.fileQuit << "\n";

            //Edit//
            hotkeys.editUndo = "Ctrl+Z";
            out << "Edit-Undo="<<hotkeys.editUndo << "\n";
            hotkeys.editRedo = "Ctrl+Shift+Z";
            out << "Edit-Redo="<<hotkeys.editRedo << "\n";
            hotkeys.editRotateRight = "Ctrl+Shift+R";
            out << "Edit-Rotate-Right="<<hotkeys.editRotateRight << "\n";
            hotkeys.editRotateLeft = "Ctrl+Shift+L";
            out << "Edit-Rotate-Left="<<hotkeys.editRotateLeft << "\n";
            hotkeys.editFlipHorizontal = "";
            out << "Edit-Flip-Horizontal="<<hotkeys.editFlipHorizontal << "\n";
            hotkeys.editFlipVertical = "";
            out << "Edit-Flip-Vertical="<<hotkeys.editFlipVertical << "\n";
            hotkeys.editCrop = "Ctrl+Shift+C";
            out << "Edit-Crop="<<hotkeys.editCrop << "\n";
            hotkeys.editResize = "Ctrl+R";
            out << "Edit-Resize="<<hotkeys.editResize << "\n";
            hotkeys.editResizeItems = "";
            out << "Edit-Resize-Items="<<hotkeys.editResizeItems << "\n";

            //Watch//
            hotkeys.watchPrevious = "Ctrl+Left";
            out << "Watch-Previous="<<hotkeys.watchPrevious << "\n";
            hotkeys.watchNext = "Ctrl+Right";
            out << "Watch-Next="<<hotkeys.watchNext << "\n";
            hotkeys.watchFullscreen = "F11";
            out << "Watch-Fullscreen="<<hotkeys.watchFullscreen << "\n";
            hotkeys.watchSlideshow = "F5";
            out << "Watch-Slideshow="<<hotkeys.watchSlideshow << "\n";
            hotkeys.watchWallpaper = "";
            out << "Watch-Wallpaper="<<hotkeys.watchWallpaper << "\n";
            hotkeys.zoomIn = "+";
            out << "Zoom-In="<<hotkeys.zoomIn << "\n";
            hotkeys.zoomOut = "-";
            out << "Zoom-Out="<<hotkeys.zoomOut << "\n";
            hotkeys.zoomWindow = "";
            out << "Zoom-Window="<<hotkeys.zoomWindow << "\n";
            hotkeys.zoomOriginal = "";
            out << "Zoom-Original="<<hotkeys.zoomOriginal << "\n";

            //About//
            hotkeys.helpAbout = "F1";
            out << "Help-About="<<hotkeys.helpAbout << "\n";
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

        /// Panel ///
        sets = out.readLine();
        panelalignment = sets.right(sets.size()-6).toInt();

        /// Hotkeys ///
        sets = out.readLine(); //empty
        sets = out.readLine(); //"hotkeys"

        //File//
        sets = out.readLine();
        hotkeys.fileOpen = sets.right(sets.size()-10);
        sets = out.readLine();
        hotkeys.fileSave = sets.right(sets.size()-10);
        sets = out.readLine();
        hotkeys.fileSaveAs = sets.right(sets.size()-13);
        sets = out.readLine();
        hotkeys.fileSettings = sets.right(sets.size()-14);
        sets = out.readLine();
        hotkeys.fileQuit = sets.right(sets.size()-10);

        //Edit//
        sets = out.readLine();
        hotkeys.editUndo = sets.right(sets.size()-10);
        sets = out.readLine();
        hotkeys.editRedo = sets.right(sets.size()-10);
        sets = out.readLine();
        hotkeys.editRotateRight = sets.right(sets.size()-18);
        sets = out.readLine();
        hotkeys.editRotateLeft = sets.right(sets.size()-17);
        sets = out.readLine();
        hotkeys.editFlipHorizontal = sets.right(sets.size()-21);
        sets = out.readLine();
        hotkeys.editFlipVertical = sets.right(sets.size()-19);
        sets = out.readLine();
        hotkeys.editCrop = sets.right(sets.size()-10);
        sets = out.readLine();
        hotkeys.editResize = sets.right(sets.size()-12);
        sets = out.readLine();
        hotkeys.editResizeItems = sets.right(sets.size()-18);

        //Watch//
        sets = out.readLine();
        hotkeys.watchPrevious = sets.right(sets.size()-15);
        sets = out.readLine();
        hotkeys.watchNext = sets.right(sets.size()-11);
        sets = out.readLine();
        hotkeys.watchFullscreen = sets.right(sets.size()-17);
        sets = out.readLine();
        hotkeys.watchSlideshow = sets.right(sets.size()-16);
        sets = out.readLine();
        hotkeys.watchWallpaper = sets.right(sets.size()-16);
        sets = out.readLine();
        hotkeys.zoomIn = sets.right(sets.size()-8);
        sets = out.readLine();
        hotkeys.zoomOut = sets.right(sets.size()-9);
        sets = out.readLine();
        hotkeys.zoomWindow = sets.right(sets.size()-12);
        sets = out.readLine();
        hotkeys.zoomOriginal = sets.right(sets.size()-14);

        //Help//
        sets = out.readLine();
        hotkeys.helpAbout = sets.right(sets.size()-11);

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
    // Load hotkeys to image //
    imagewidget->loadhotkeys(&hotkeys);
    // Signals-slots and tool-tips for buttons and menu actions //
    createActions();
    // Shortcuts for buttons and menu actions //
    createHotkeys();
    // Icons and other design for buttons and menu actions //
    createDesign();

    // Default open or open argv[1] image //
    if (path == "\0")
    {
        defaultpath = QString::null;
        imagewidget->loadimage(":/res/logo.png");
        defaultpath = QString::null;
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
    ui->openAction->setStatusTip(tr("Open current image"));
    connect(ui->openAction,SIGNAL(triggered()),this,SLOT(fileOpen()));

    ui->saveAction->setStatusTip(tr("Save current image with same name and format"));
    connect(ui->saveAction,SIGNAL(triggered()),this,SLOT(fileSave()));
    connect(imagewidget,SIGNAL(itsSaved(bool)),this,SLOT(setStatusName(bool)));

    ui->saveAsAction->setStatusTip(tr("Save current image with new name and format"));
    connect(ui->saveAsAction,SIGNAL(triggered()),this,SLOT(fileSaveAs()));

    ui->settingsAction->setStatusTip(tr("Program settings"));
    connect(ui->settingsAction,SIGNAL(triggered()),this,SLOT(settingsWindow()));

    ui->exitAction->setStatusTip(tr("Close program"));
    connect(ui->exitAction,SIGNAL(triggered()),this,SLOT(close()));

    // Edit //
    ui->actionUndo->setStatusTip(tr("Cancel last changes"));
    connect(ui->actionUndo,SIGNAL(triggered()),imagewidget,SLOT(prevBuffer()));
    connect(imagewidget,SIGNAL(itsPossibleToUndo(bool)),this,SLOT(setUndoEnable(bool)));

    ui->actionRedo->setStatusTip(tr("Do last changes"));
    connect(ui->actionRedo,SIGNAL(triggered()),imagewidget,SLOT(nextBuffer()));
    connect(imagewidget,SIGNAL(itsPossibleToRedo(bool)),this,SLOT(setRedoEnable(bool)));

    ui->rotateLeftAction->setStatusTip(tr("Rotate image to the left"));
    connect(ui->rotateLeftAction,SIGNAL(triggered()),imagewidget,SLOT(rotateLeft()));

    ui->rotateRightAction->setStatusTip(tr("Rotate image to the right"));
    connect(ui->rotateRightAction,SIGNAL(triggered()),imagewidget,SLOT(rotateRight()));

    ui->flipHorizontalAction->setStatusTip(tr("Change this image to horizontal mirror"));
    connect(ui->flipHorizontalAction,SIGNAL(triggered()),imagewidget,SLOT(flipHorizontal()));

    ui->flipVerticalAction->setStatusTip(tr("Change this image to vertical mirror"));
    connect(ui->flipVerticalAction,SIGNAL(triggered()),imagewidget,SLOT(flipVertical()));

    ui->deleteFileAction->setStatusTip(tr("Delete current image"));
    connect(ui->deleteFileAction,SIGNAL(triggered()),imagewidget,SLOT(deleteCurrentItem()));

    ui->cropAction->setStatusTip(tr("Crop current image"));
    connect(ui->cropAction,SIGNAL(triggered()),this,SLOT(cropImage()));

    ui->resizeAction->setStatusTip(tr("Resize current image"));
    connect(ui->resizeAction,SIGNAL(triggered()),this,SLOT(resizeImage()));

    ui->resizeitemsAction->setStatusTip(tr("Resize list of images"));
    connect(ui->resizeitemsAction,SIGNAL(triggered()),this,SLOT(resizeImageList()));

    // Watching //
    ui->nextimageAction->setStatusTip(tr("Show next image"));
    connect(ui->nextimageAction,SIGNAL(triggered()),this,SLOT(nextImage()));

    ui->previmageAction->setStatusTip(tr("Show previous image"));
    connect(ui->previmageAction,SIGNAL(triggered()),this,SLOT(prevImage()));

    ui->fullscreenAction->setStatusTip(tr("Enable fullscreen mode"));
    connect(ui->fullscreenAction,SIGNAL(triggered()),this,SLOT(fullScreen()));
    connect(imagewidget,SIGNAL(needFullscreen()),this,SLOT(fullScreenFromImage()));

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
    ui->aboutAction->setStatusTip(tr("Information about program"));
    connect(ui->aboutAction,SIGNAL(triggered()),this,SLOT(helpAbout()));

    ///BUTTONS///
    ui->prevButton->setToolTip(tr("Open previous image"));
    connect(ui->prevButton,SIGNAL(clicked()),this,SLOT(prevImage()));

    ui->nextButton->setToolTip(tr("Open next image"));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(nextImage()));

    ui->fullscreenButton->setToolTip(tr("Enable fullscreen mode"));
    connect(ui->fullscreenButton,SIGNAL(clicked()),this,SLOT(fullScreen()));    

    ui->slideshowButton->setToolTip(tr("Start slideshow in fullscreen mode"));
    connect(ui->slideshowButton,SIGNAL(clicked()),this,SLOT(slideShow()));

    ui->zoomIncButton->setToolTip(tr("Zoom in image"));
    connect(ui->zoomIncButton,SIGNAL(clicked()),imagewidget,SLOT(zoomInc()));

    ui->zoomDecButton->setToolTip(tr("Zoom out image"));
    connect(ui->zoomDecButton,SIGNAL(clicked()),imagewidget,SLOT(zoomDec()));

    ui->zoomOriginalButton->setToolTip(tr("Zoom to original size"));
    connect(ui->zoomOriginalButton,SIGNAL(clicked()),imagewidget,SLOT(setOriginalSize()));

    ui->zoomWindowButton->setToolTip(tr("Zoom to window size"));
    connect(ui->zoomWindowButton,SIGNAL(clicked()),imagewidget,SLOT(reloadImage()));

    ui->rotateLeftButton->setToolTip(tr("Rotate picture to the left"));
    connect(ui->rotateLeftButton,SIGNAL(clicked()),imagewidget,SLOT(rotateLeft()));

    ui->rotateRightButton->setToolTip(tr("Rotate picture to the right"));
    connect(ui->rotateRightButton,SIGNAL(clicked()),imagewidget,SLOT(rotateRight()));

    //Changing image
    connect(imagewidget,SIGNAL(currentImageWasChanged(int)),this,SLOT(currentIndexWasChanged(int)));

    // set nofocus on buttons //
    ui->prevButton->setFocusPolicy(Qt::NoFocus);
    ui->nextButton->setFocusPolicy(Qt::NoFocus);
    ui->fullscreenButton->setFocusPolicy(Qt::NoFocus);
    ui->slideshowButton->setFocusPolicy(Qt::NoFocus);
    ui->zoomDecButton->setFocusPolicy(Qt::NoFocus);
    ui->zoomIncButton->setFocusPolicy(Qt::NoFocus);
    ui->zoomOriginalButton->setFocusPolicy(Qt::NoFocus);
    ui->zoomWindowButton->setFocusPolicy(Qt::NoFocus);
    ui->rotateLeftButton->setFocusPolicy(Qt::NoFocus);
    ui->rotateRightButton->setFocusPolicy(Qt::NoFocus);
}

void QImageViewer::createHotkeys()
{
    //File//
    ui->openAction->setShortcut(QKeySequence::fromString(hotkeys.fileOpen));
    ui->saveAction->setShortcut(QKeySequence::fromString(hotkeys.fileSave));
    ui->saveAsAction->setShortcut(QKeySequence::fromString(hotkeys.fileSaveAs));
    ui->settingsAction->setShortcut(QKeySequence::fromString(hotkeys.fileSettings));
    ui->exitAction->setShortcut(QKeySequence::fromString(hotkeys.fileQuit));
    //Edit//
    ui->actionUndo->setShortcut(QKeySequence::fromString(hotkeys.editUndo));
    ui->actionRedo->setShortcut(QKeySequence::fromString(hotkeys.editRedo));
    ui->rotateLeftAction->setShortcut(QKeySequence::fromString(hotkeys.editRotateLeft));
    ui->rotateRightAction->setShortcut(QKeySequence::fromString(hotkeys.editRotateRight));
    ui->flipHorizontalAction->setShortcut(QKeySequence::fromString(hotkeys.editFlipHorizontal));
    ui->flipVerticalAction->setShortcut(QKeySequence::fromString(hotkeys.editFlipVertical));
    ui->cropAction->setShortcut(QKeySequence::fromString(hotkeys.editCrop));
    ui->resizeAction->setShortcut(QKeySequence::fromString(hotkeys.editResize));
    ui->resizeitemsAction->setShortcut(QKeySequence::fromString(hotkeys.editResizeItems));
    ui->deleteFileAction->setShortcut(QKeySequence(Qt::Key_Delete));
    //Watch//
    ui->previmageAction->setShortcut(QKeySequence::fromString(hotkeys.watchPrevious));
    ui->nextimageAction->setShortcut(QKeySequence::fromString(hotkeys.watchNext));
    ui->fullscreenAction->setShortcut(QKeySequence::fromString(hotkeys.watchFullscreen));
    ui->slideshowAction->setShortcut(QKeySequence::fromString(hotkeys.watchSlideshow));
    ui->wallpaperAction->setShortcut(QKeySequence::fromString(hotkeys.watchWallpaper));
    ui->zoomInAction->setShortcut(QKeySequence::fromString(hotkeys.zoomIn));
    ui->zoomOutAction->setShortcut(QKeySequence::fromString(hotkeys.zoomOut));
    ui->zoomOriginalAction->setShortcut(QKeySequence::fromString(hotkeys.zoomOriginal));
    ui->zoomWindowAction->setShortcut(QKeySequence::fromString(hotkeys.zoomWindow));
    //Help//
    ui->aboutAction->setShortcut(hotkeys.helpAbout);
}

/** Loading all icons and design for buttons and menu actions **/
void QImageViewer::createDesign()
{
    if (panelalignment == 1)
    {
        ui->panelTopLayout->addSpacerItem(ui->panelLeftHorizontalSpacer);
        ui->panelTopLayout->addWidget(ui->zoomIncButton);
        ui->panelTopLayout->addWidget(ui->zoomDecButton);
        ui->panelTopLayout->addWidget(ui->zoomWindowButton);
        ui->panelTopLayout->addWidget(ui->zoomOriginalButton);
        ui->panelTopLayout->addWidget(ui->rotateRightButton);
        ui->panelTopLayout->addWidget(ui->rotateLeftButton);
        ui->panelTopLayout->addWidget(ui->fullscreenButton);
        ui->panelTopLayout->addWidget(ui->slideshowButton);
        ui->panelTopLayout->addSpacerItem(ui->panelRightHorizontalSpacer);
    }
    else if (panelalignment == 2)
    {
        ui->zoomIncButton->hide();
        ui->zoomDecButton->hide();
        ui->zoomWindowButton->hide();
        ui->zoomOriginalButton->hide();
        ui->rotateRightButton->hide();
        ui->rotateLeftButton->hide();
        ui->fullscreenButton->hide();
        ui->slideshowButton->hide();
    }

    //File//
    ui->saveAction->setIcon(QIcon(QPixmap(":/res/file-save.png")));
    ui->openAction->setIcon(QIcon(QPixmap(":/res/file-open.png")));
    ui->settingsAction->setIcon(QIcon(QPixmap(":/res/settings.png")));
    //Edit//
    ui->rotateLeftAction->setIcon(QIcon(QPixmap(":/res/rotate-left.png")));
    ui->rotateRightAction->setIcon(QIcon(QPixmap(":/res/rotate-right.png")));
    ui->flipHorizontalAction->setIcon(QIcon(QPixmap(":/res/flip-horizontal.png")));
    ui->flipVerticalAction->setIcon(QIcon(QPixmap(":/res/flip-vertical.png")));
    ui->deleteFileAction->setIcon(QIcon(QPixmap(":/res/delete.png")));
    ui->resizeAction->setIcon(QIcon(QPixmap(":/res/resize.png")));
    ui->resizeitemsAction->setIcon(QIcon(QPixmap(":/res/resize-items.png")));
    ui->cropAction->setIcon(QIcon(QPixmap(":/res/crop.png")));
    //Watch//
    ui->previmageAction->setIcon(QIcon(QPixmap(":/res/prev.png")));
    ui->nextimageAction->setIcon(QIcon(QPixmap(":/res/next.png")));
    ui->zoomInAction->setIcon(QIcon(QPixmap(":/res/zoom-in.png")));
    ui->zoomOutAction->setIcon(QIcon(QPixmap(":/res/zoom-out.png")));
    ui->zoomOriginalAction->setIcon(QIcon(QPixmap(":/res/zoom-original.png")));
    ui->zoomWindowAction->setIcon(QIcon(QPixmap(":/res/zoom-window.png")));
    ui->fullscreenAction->setIcon(QIcon(QPixmap(":/res/fullscreen.png")));
    ui->slideshowAction->setIcon(QIcon(QPixmap(":/res/slideshow.png")));
    ui->wallpaperAction->setIcon(QIcon(QPixmap(":/res/wallpaper.png")));
    //Help//
    ui->aboutAction->setIcon(QIcon(QPixmap(":/res/help.png")));

    ui->prevButton->setIcon(QIcon(QPixmap(":/res/prev.png")));
    ui->nextButton->setIcon(QIcon(QPixmap(":/res/next.png")));
    ui->fullscreenButton->setIcon(QIcon(QPixmap(":/res/fullscreen.png")));
    ui->slideshowButton->setIcon(QIcon(QPixmap(":/res/slideshow.png")));
    ui->zoomDecButton->setIcon(QIcon(QPixmap(":/res/zoom-out.png")));
    ui->zoomIncButton->setIcon(QIcon(QPixmap(":/res/zoom-in.png")));
    ui->zoomOriginalButton->setIcon(QIcon(QPixmap(":/res/zoom-original.png")));
    ui->zoomWindowButton->setIcon(QIcon(QPixmap(":/res/zoom-window.png")));
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

    ui->prevButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
    ui->fullscreenButton->setEnabled(true);
    ui->slideshowButton->setEnabled(true);
    ui->zoomOriginalButton->setEnabled(true);
    ui->zoomWindowButton->setEnabled(true);
    ui->zoomDecButton->setEnabled(true);
    ui->zoomIncButton->setEnabled(true);
    ui->rotateLeftButton->setEnabled(true);
    ui->rotateRightButton->setEnabled(true);

    // find other pictures in folder //
    filesFind();
    setWindowTitle("QImageViewer - ["+QString::number(imagewidget->currentImage()+1)+
                   tr(" of ") + QString::number(imagewidget->size()) + "] "
                   + imagewidget->currentImageName());
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
            setWindowTitle("QImageViewer - ["+QString::number(imagewidget->currentImage()+1)+
                           tr(" of ") + QString::number(imagewidget->size()) + "] "
                           + imagewidget->currentImageName());
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
    fullScreenWidget = new fullscreen(imagewidget,&hotkeys);
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
    //settings = new Settings;

    settings = new Settings;
    connect(settings,SIGNAL(acceptsettings(QString,QString,bool,bool,bool,double,int,hotkeysStruct)),
            this,SLOT(updateSettings(QString,QString,bool,bool,bool,double,int,hotkeysStruct)));

    settings->setDefaultSettings(language,
                                 lastdirectory,
                                 imagewidget->getMouseZoom(),
                                 imagewidget->getMouseFullscreen(),
                                 slideshowSmoothTransition,
                                 slideshowInterval,
                                 panelalignment,
                                 hotkeys);
    settings->show();
}

/** Update program settings **/
void QImageViewer::updateSettings(QString language,
                    QString defaultfolder,
                    bool mouseZoom, bool mouseFullscreen,
                    bool slideshowSmoothTransition, double slideshowInterval,
                    int panelalignment,hotkeysStruct hotkeys)
{
    this->language = language;
    this->lastdirectory = defaultfolder;
    imagewidget->setMouseZoom(mouseZoom);
    imagewidget->setMouseFullscreen(mouseFullscreen);
    this->slideshowSmoothTransition = slideshowSmoothTransition;
    this->slideshowInterval = slideshowInterval;
    this->panelalignment = panelalignment;

    /// Panel ///

    if (panelalignment == 0)
    {
        ui->panelBottomLayout->addSpacerItem(ui->panelLeftHorizontalSpacer);
        ui->panelBottomLayout->addWidget(ui->zoomIncButton);
        ui->panelBottomLayout->addWidget(ui->zoomDecButton);
        ui->panelBottomLayout->addWidget(ui->zoomWindowButton);
        ui->panelBottomLayout->addWidget(ui->zoomOriginalButton);
        ui->panelBottomLayout->addWidget(ui->rotateRightButton);
        ui->panelBottomLayout->addWidget(ui->rotateLeftButton);
        ui->panelBottomLayout->addWidget(ui->fullscreenButton);
        ui->panelBottomLayout->addWidget(ui->slideshowButton);
        ui->panelBottomLayout->addSpacerItem(ui->panelRightHorizontalSpacer);
    }
    else if (panelalignment == 1)
    {
        ui->panelTopLayout->addSpacerItem(ui->panelLeftHorizontalSpacer);
        ui->panelTopLayout->addWidget(ui->zoomIncButton);
        ui->panelTopLayout->addWidget(ui->zoomDecButton);
        ui->panelTopLayout->addWidget(ui->zoomWindowButton);
        ui->panelTopLayout->addWidget(ui->zoomOriginalButton);
        ui->panelTopLayout->addWidget(ui->rotateRightButton);
        ui->panelTopLayout->addWidget(ui->rotateLeftButton);
        ui->panelTopLayout->addWidget(ui->fullscreenButton);
        ui->panelTopLayout->addWidget(ui->slideshowButton);
        ui->panelTopLayout->addSpacerItem(ui->panelRightHorizontalSpacer);
    }
    else if (panelalignment == 2)
    {
        ui->zoomIncButton->hide();
        ui->zoomDecButton->hide();
        ui->zoomWindowButton->hide();
        ui->zoomOriginalButton->hide();
        ui->rotateRightButton->hide();
        ui->rotateLeftButton->hide();
        ui->fullscreenButton->hide();
        ui->slideshowButton->hide();
    }

    /// Hotkeys ///
    this->hotkeys = hotkeys;

    createHotkeys();

    disconnect(settings,SIGNAL(acceptsettings(QString,QString,bool,bool,bool,double,int,hotkeysStruct)),
            this,SLOT(updateSettings(QString,QString,bool,bool,bool,double,int,hotkeysStruct)));
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

    editFormResizeElements->loadlist(imagewidget->getImageList(),lastdirectory);
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
    if (result)
    {
        if (editFormResizeElements->isSameFolder())
        {
            //Add new images to list, don't change current image
            filesFind();
            setWindowTitle("QImageViewer - ["+QString::number(imagewidget->currentImage()+1)+
                           tr(" of ") + QString::number(imagewidget->size()) + "] "
                           + imagewidget->currentImageName());
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


/** Show 'About' **/
void QImageViewer::helpAbout()
{
    QMessageBox::about(this, tr("About"),
        tr("This program is using for viewing pictures\n"
           "It was created with using Qt 5.0.0\n\n"
           "Program author is Bodnya Alexey\n"
           "Ukraine, Kiev, KPI, TV-11\n"
           "This program is under license GPLv2\n"
           "It's free to use and all sources are open.\n"
           "\n"
           "Thanks for testing and helping with tips and ideas to\n"
           "my friends and Typical Proger community!\n"
           "\n"
           "Special thx Neveron for help and support!"));
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

    /// Panel ///
    out << "PANEL="<<panelalignment<<"\n";

    /// Hotkeys ///
    out << "\n#Hotkeys\n";
    //File//
    out << "File-Open="<<hotkeys.fileOpen << "\n";
    out << "File-Save="<<hotkeys.fileSave << "\n";
    out << "File-Save-As="<<hotkeys.fileSaveAs << "\n";
    out << "File-Settings="<<hotkeys.fileSettings << "\n";
    out << "File-Quit="<<hotkeys.fileQuit << "\n";

    //Edit//
    out << "Edit-Undo="<<hotkeys.editUndo << "\n";
    out << "Edit-Redo="<<hotkeys.editRedo << "\n";
    out << "Edit-Rotate-Right="<<hotkeys.editRotateRight << "\n";
    out << "Edit-Rotate-Left="<<hotkeys.editRotateLeft << "\n";
    out << "Edit-Flip-Horizontal="<<hotkeys.editFlipHorizontal << "\n";
    out << "Edit-Flip-Vertical="<<hotkeys.editFlipVertical << "\n";
    out << "Edit-Crop="<<hotkeys.editCrop << "\n";
    out << "Edit-Resize="<<hotkeys.editResize << "\n";
    out << "Edit-Resize-Items="<<hotkeys.editResizeItems << "\n";

    //Watch//
    out << "Watch-Previous="<<hotkeys.watchPrevious << "\n";
    out << "Watch-Next="<<hotkeys.watchNext << "\n";
    out << "Watch-Fullscreen="<<hotkeys.watchFullscreen << "\n";
    out << "Watch-Slideshow="<<hotkeys.watchSlideshow << "\n";
    out << "Watch-Wallpaper="<<hotkeys.watchWallpaper << "\n";
    out << "Zoom-In="<<hotkeys.zoomIn << "\n";
    out << "Zoom-Out="<<hotkeys.zoomOut << "\n";
    out << "Zoom-Window="<<hotkeys.zoomWindow << "\n";
    out << "Zoom-Original="<<hotkeys.zoomOriginal << "\n";

    //Help//
    out << "Help-About="<<hotkeys.helpAbout << "\n";

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

    //Close all openned windows//
    settings->close();

    event->accept();
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
