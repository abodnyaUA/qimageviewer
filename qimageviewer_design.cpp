#include "qimageviewer.h"
#include "ui_qimageviewer.h"

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
            fullscreencolor = QColor(255,255,255);
            out << "FULLSCREEN_COLOR=\n" <<
                   "RED=" << fullscreencolor.red() << "\n" <<
                   "GREEN=" << fullscreencolor.green() << "\n" <<
                   "BLUE=" << fullscreencolor.blue() << "\n";

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
            hotkeys.watchFullscreen = "F10";
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

            /// Panel ///
            out << "\n#Panel\n";
            isneedBut.rotateLeft = true;
            out << "RotateLeft="<< (int)isneedBut.rotateLeft << "\n";
            isneedBut.rotateRight = true;
            out << "RotateRight="<< (int)isneedBut.rotateRight << "\n";
            isneedBut.flipHorizontal = true;
            out << "FlipHorizontal="<< (int)isneedBut.flipHorizontal << "\n";
            isneedBut.flipVertical = true;
            out << "FlipVertical="<< (int)isneedBut.flipVertical << "\n";
            isneedBut.zoomIn = true;
            out << "Zoom In="<< (int)isneedBut.zoomIn << "\n";
            isneedBut.zoomOut = true;
            out << "Zoom Out="<< (int)isneedBut.zoomOut << "\n";
            isneedBut.zoomWindow = true;
            out << "Zoom Window="<< (int)isneedBut.zoomWindow << "\n";
            isneedBut.zoomOriginal = true;
            out << "Zoom Original="<< (int)isneedBut.zoomOriginal << "\n";
            isneedBut.fullscreen = true;
            out << "Fullscreen="<< (int)isneedBut.fullscreen << "\n";
            isneedBut.slideshow = true;
            out << "Slideshow="<< (int)isneedBut.slideshow << "\n";
            isneedBut.properties = false;
            out << "Properties="<< (int)isneedBut.properties << "\n";

            /// Panel ///
            out << "\n#Extern editors\n";
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

        /// Fullscreen ///
        sets = out.readLine();
        sets = out.readLine();
        int r = sets.right(sets.size()-4).toInt();
        sets = out.readLine();
        int g = sets.right(sets.size()-6).toInt();
        sets = out.readLine();
        int b = sets.right(sets.size()-5).toInt();
        fullscreencolor = QColor::fromRgb(r,g,b);

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

        /// Panel ///
        sets = out.readLine(); //empty
        sets = out.readLine(); //"Panel"

        sets = out.readLine();
        isneedBut.rotateLeft = (bool)sets.right(sets.size()-11).toInt();
        sets = out.readLine();
        isneedBut.rotateRight = (bool)sets.right(sets.size()-12).toInt();
        sets = out.readLine();
        isneedBut.flipHorizontal = (bool)sets.right(sets.size()-15).toInt();
        sets = out.readLine();
        isneedBut.flipVertical = (bool)sets.right(sets.size()-13).toInt();
        sets = out.readLine();
        isneedBut.zoomIn = (bool)sets.right(sets.size()-8).toInt();
        sets = out.readLine();
        isneedBut.zoomOut = (bool)sets.right(sets.size()-9).toInt();
        sets = out.readLine();
        isneedBut.zoomWindow = (bool)sets.right(sets.size()-12).toInt();
        sets = out.readLine();
        isneedBut.zoomOriginal = (bool)sets.right(sets.size()-14).toInt();
        sets = out.readLine();
        isneedBut.fullscreen = (bool)sets.right(sets.size()-11).toInt();
        sets = out.readLine();
        isneedBut.slideshow = (bool)sets.right(sets.size()-10).toInt();
        sets = out.readLine();
        isneedBut.properties = (bool)sets.right(sets.size()-11).toInt();

        /// Extern editors ///
        sets = out.readLine(); //empty
        sets = out.readLine(); //"Extern editors"

        /**
          Name=Gimp
          Icon=
          Programm=
          **/
        while (!out.atEnd())
        {
            sets = out.readLine();
            QString name = sets.right(sets.size()-5);
            sets = out.readLine();
            QString icon = sets.right(sets.size()-5);
            sets = out.readLine();
            QString command = sets.right(sets.size()-9);

            QExternProgram *editor = new QExternProgram(name,icon,command,imagewidget);
            editors.append(editor);
            QAction * action = new QAction(QIcon(QPixmap(icon)),name,this);
            editorsActions.append(action);
            connect(action,SIGNAL(triggered()),editor,SLOT(exec()));
            ui->menuExtern_editors->insertAction(ui->editorsNewAction,action);

            sets = out.readLine(); //empty
        }
        if (editors.size() != 0) ui->menuExtern_editors->insertSeparator(ui->editorsNewAction);
    }
    file.close();
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

    // Extern editors //
    ui->editorsNewAction->setStatusTip(tr("Add new extern editor. You will can open current image with other editor"));
    connect(ui->editorsNewAction,SIGNAL(triggered()),this,SLOT(newExternEditor()));

    ui->editorsManagerAction->setStatusTip(tr("Set up your extern settings"));
    connect(ui->editorsManagerAction,SIGNAL(triggered()),this,SLOT(exterEditorsManager()));

    for (int i=0;i<editorsActions.size();i++)
        editorsActions[i]->setEnabled(false);

    // Share //
    ui->shareImageShackAction->setStatusTip(tr("Share this image with ImageShack.us"));
    connect(ui->shareImageShackAction,SIGNAL(triggered()),this,SLOT(imageshackShare()));

    // Help //
    ui->aboutAction->setStatusTip(tr("Information about program"));
    connect(ui->aboutAction,SIGNAL(triggered()),this,SLOT(helpAbout()));

    ///BUTTONS///
    ui->prevButton->setToolTip(tr("Open previous image"));
    connect(ui->prevButton,SIGNAL(clicked()),this,SLOT(prevImage()));

    ui->nextButton->setToolTip(tr("Open next image"));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(nextImage()));

    //Changing image
    connect(imagewidget,SIGNAL(currentImageWasChanged(int)),this,SLOT(currentIndexWasChanged(int)));

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

}

void QImageViewer::createPanel()
{
    if (isneedBut.rotateLeft)
    {
        butRotateLeft = new QPushButton;
        butRotateLeft->setToolTip(tr("Rotate picture to the left"));
        connect(butRotateLeft,SIGNAL(clicked()),imagewidget,SLOT(rotateLeft()));
        butRotateLeft->setFocusPolicy(Qt::NoFocus);
        butRotateLeft->setIcon(QIcon(QPixmap(":/res/rotate-left.png")));
        butRotateLeft->setEnabled(false);
        buttonsList << butRotateLeft;
    }
    if (isneedBut.rotateRight)
    {
        butRotateRight = new QPushButton;
        butRotateRight->setToolTip(tr("Rotate picture to the right"));
        connect(butRotateRight,SIGNAL(clicked()),imagewidget,SLOT(rotateRight()));
        butRotateRight->setFocusPolicy(Qt::NoFocus);
        butRotateRight->setIcon(QIcon(QPixmap(":/res/rotate-right.png")));
        butRotateRight->setEnabled(false);
        buttonsList << butRotateRight;
    }
    if (isneedBut.flipHorizontal)
    {
        butFlipHorizontal = new QPushButton;
        butFlipHorizontal->setToolTip(tr("Change this image to horizontal mirror"));
        connect(butFlipHorizontal,SIGNAL(clicked()),imagewidget,SLOT(flipHorizontal()));
        butFlipHorizontal->setFocusPolicy(Qt::NoFocus);
        butFlipHorizontal->setIcon(QIcon(QPixmap(":/res/flip-horizontal.png")));
        butFlipHorizontal->setEnabled(false);
        buttonsList << butFlipHorizontal;
    }
    if (isneedBut.flipVertical)
    {
        butFlipVertical = new QPushButton;
        butFlipVertical->setToolTip(tr("Change this image to vertical mirror"));
        connect(butFlipVertical,SIGNAL(clicked()),imagewidget,SLOT(flipVertical()));
        butFlipVertical->setFocusPolicy(Qt::NoFocus);
        butFlipVertical->setIcon(QIcon(QPixmap(":/res/flip-vertical.png")));
        butFlipVertical->setEnabled(false);
        buttonsList << butFlipVertical;
    }
    if (isneedBut.zoomIn)
    {
        butZoomIn = new QPushButton;
        butZoomIn->setToolTip(tr("Zoom In"));
        connect(butZoomIn,SIGNAL(clicked()),imagewidget,SLOT(zoomInc()));
        butZoomIn->setFocusPolicy(Qt::NoFocus);
        butZoomIn->setIcon(QIcon(QPixmap(":/res/zoom-in.png")));
        butZoomIn->setEnabled(false);
        buttonsList << butZoomIn;
    }
    if (isneedBut.zoomOut)
    {
        butZoomOut = new QPushButton;
        butZoomOut->setToolTip(tr("Zoom Out"));
        connect(butZoomOut,SIGNAL(clicked()),imagewidget,SLOT(zoomDec()));
        butZoomOut->setFocusPolicy(Qt::NoFocus);
        butZoomOut->setIcon(QIcon(QPixmap(":/res/zoom-out.png")));
        butZoomOut->setEnabled(false);
        buttonsList << butZoomOut;
    }
    if (isneedBut.zoomWindow)
    {
        butZoomWindow = new QPushButton;
        butZoomWindow->setToolTip(tr("Zoom to window size"));
        connect(butZoomWindow,SIGNAL(clicked()),imagewidget,SLOT(reloadImage()));
        butZoomWindow->setFocusPolicy(Qt::NoFocus);
        butZoomWindow->setIcon(QIcon(QPixmap(":/res/zoom-window.png")));
        butZoomWindow->setEnabled(false);
        buttonsList << butZoomWindow;
    }
    if (isneedBut.zoomOriginal)
    {
        butZoomOriginal = new QPushButton;
        butZoomOriginal->setToolTip(tr("Zoom to original size"));
        connect(butZoomOriginal,SIGNAL(clicked()),imagewidget,SLOT(setOriginalSize()));
        butZoomOriginal->setFocusPolicy(Qt::NoFocus);
        butZoomOriginal->setIcon(QIcon(QPixmap(":/res/zoom-original.png")));
        butZoomOriginal->setEnabled(false);
        buttonsList << butZoomOriginal;
    }
    if (isneedBut.fullscreen)
    {
        butFullscreen = new QPushButton;
        butFullscreen->setToolTip(tr("Enable fullscreen mode"));
        connect(butFullscreen,SIGNAL(clicked()),this,SLOT(fullScreen()));
        butFullscreen->setFocusPolicy(Qt::NoFocus);
        butFullscreen->setIcon(QIcon(QPixmap(":/res/fullscreen.png")));
        butFullscreen->setEnabled(false);
        buttonsList << butFullscreen;
    }
    if (isneedBut.slideshow)
    {
        butSlideshow = new QPushButton;
        butSlideshow->setToolTip(tr("Start slideshow in fullscreen mode"));
        connect(butSlideshow,SIGNAL(clicked()),this,SLOT(slideShow()));
        butSlideshow->setFocusPolicy(Qt::NoFocus);
        butSlideshow->setIcon(QIcon(QPixmap(":/res/slideshow.png")));
        butSlideshow->setEnabled(false);
        buttonsList << butSlideshow;
    }
    if (isneedBut.properties)
    {
        butProperties = new QPushButton;
        butProperties->setToolTip(tr("Image properties"));
        connect(butProperties,SIGNAL(clicked()),imagewidget,SLOT(viewProperties()));
        butProperties->setFocusPolicy(Qt::NoFocus);
        butProperties->setIcon(QIcon(QPixmap(":/res/file-properties.png")));
        butProperties->setEnabled(false);
        buttonsList << butProperties;
    }

    spacerLeft = new QSpacerItem(40,20,QSizePolicy::Expanding);
    spacerRight = new QSpacerItem(40,20,QSizePolicy::Expanding);

    if (panelalignment == 0)
    {
        ui->panelBottomLayout->addSpacerItem(spacerLeft);
        for (int i=0;i<buttonsList.size();i++)
            ui->panelBottomLayout->addWidget(buttonsList[i]);
        ui->panelBottomLayout->addSpacerItem(spacerRight);
    }
    else if (panelalignment == 1)
    {
        ui->panelTopLayout->addSpacerItem(spacerLeft);
        for (int i=0;i<buttonsList.size();i++)
            ui->panelTopLayout->addWidget(buttonsList[i]);
        ui->panelTopLayout->addSpacerItem(spacerRight);
    }

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

    /// Fullscreen background ///
    out << "FULLSCREEN_COLOR=\n" <<
           "RED=" << fullscreencolor.red() << "\n" <<
           "GREEN=" << fullscreencolor.green() << "\n" <<
           "BLUE=" << fullscreencolor.blue() << "\n";

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

    /// Panel ///
    out << "\n#Panel\n";
    out << "RotateLeft="<< (int)isneedBut.rotateLeft << "\n";
    out << "RotateRight="<< (int)isneedBut.rotateRight << "\n";
    out << "FlipHorizontal="<< (int)isneedBut.flipHorizontal << "\n";
    out << "FlipVertical="<< (int)isneedBut.flipVertical << "\n";
    out << "Zoom In="<< (int)isneedBut.zoomIn << "\n";
    out << "Zoom Out="<< (int)isneedBut.zoomOut << "\n";
    out << "Zoom Window="<< (int)isneedBut.zoomWindow << "\n";
    out << "Zoom Original="<< (int)isneedBut.zoomOriginal << "\n";
    out << "Fullscreen="<< (int)isneedBut.fullscreen << "\n";
    out << "Slideshow="<< (int)isneedBut.slideshow << "\n";
    out << "Properties="<< (int)isneedBut.properties << "\n";

    /// Extern Editors ///
    out << "\n#Extern editors\n";
    for (int i=0;i<editors.size();i++)
    {
        out << "Name=" << editors[i]->name << "\n";
        out << "Icon=" << editors[i]->icon << "\n";
        out << "Programm=" << editors[i]->command << "\n";
        out << "\n";
    }
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
    if (isSettingsActive) settings->close();
    if (isEditorAddFormActive) editorAddForm->close();
    if (isEditosManagerActive) editorsManager->close();
    for (int i=0; i<imageshack.size();i++)
        if (imageshack[i] != 0) imageshack[i]->close();

    event->accept();
}
