#include "qimageviewer.h"
#include "ui_qimageviewer.h"

void QImageViewer::loadsettings()
{
    qsettings = new QSettings("QImageViewer","QImageViewer");
    language = qsettings->value("Programm/Language","sys").toString();
    lastdirectory = qsettings->value("Programm/Directory",QDir::homePath()).toString();
#ifdef Q_OS_WIN32
    iconpacksfolder = QDir::currentPath()+"\\themes\\";
#else
    iconpacksfolder = QDir::homePath()+"/.config/QImageViewer/themes/";
#endif
    currenticonpackString = qsettings->value("Programm/CurrentIconPack",":/res/").toString();

    imagewidget->setMouseFullscreen((bool)(qsettings->value("Navigation/MouseFullscreen","1").toInt()));
    imagewidget->setMouseZoom((bool)(qsettings->value("Navigation/MouseZoom","0").toInt()));

    panelalignment = qsettings->value("Panel/Alignment","0").toInt();

    slideshowSmoothTransition = ((bool)(qsettings->value("Slideshow/SmoothTransition","0").toInt()));
    slideshowInterval = qsettings->value("Slideshow/Interval","1").toInt();

    int fullscreencolor_red,fullscreencolor_green,fullscreencolor_blue;
    fullscreencolor_red = qsettings->value("FullscreenColor/red","255").toInt();
    fullscreencolor_green = qsettings->value("FullscreenColor/green","255").toInt();
    fullscreencolor_blue = qsettings->value("FullscreenColor/blue","255").toInt();
    fullscreencolor = QColor(fullscreencolor_red,fullscreencolor_green,fullscreencolor_blue);

    /// Hotkeys ///
    //File//
    hotkeys.fileOpen = qsettings->value("Hotkeys/FileOpen","Ctrl+O").toString();
    hotkeys.fileSave = qsettings->value("Hotkeys/FileSave","Ctrl+S").toString();
    hotkeys.fileSaveAs = qsettings->value("Hotkeys/FileSaveAs","Ctrl+Shift+S").toString();
    hotkeys.fileSettings = qsettings->value("Hotkeys/FileSettings","").toString();
    hotkeys.fileQuit = qsettings->value("Hotkeys/FileQuit","Ctrl+Q").toString();

    //Edit//
    hotkeys.editUndo = qsettings->value("Hotkeys/EditUndo","Ctrl+Z").toString();
    hotkeys.editRedo = qsettings->value("Hotkeys/EditRedo","Ctrl+Shift+Z").toString();
    hotkeys.editRotateRight = qsettings->value("Hotkeys/EditRotateLeft","Ctrl+Shift+R").toString();
    hotkeys.editRotateLeft = qsettings->value("Hotkeys/EditRotateRight","Ctrl+Shift+L").toString();
    hotkeys.editFlipHorizontal = qsettings->value("Hotkeys/EditFlipHorizontal","").toString();
    hotkeys.editFlipVertical = qsettings->value("Hotkeys/EditFlipVertical","").toString();
    hotkeys.editCrop = qsettings->value("Hotkeys/EditCrop","Ctrl+Shift+C").toString();
    hotkeys.editResize = qsettings->value("Hotkeys/EditResize","Ctrl+R").toString();
    hotkeys.editResizeItems = qsettings->value("Hotkeys/EditResizeItems","").toString();

    //Watch//
    hotkeys.watchPrevious = qsettings->value("Hotkeys/WatchPrevious","Ctrl+Left").toString();
    hotkeys.watchNext = qsettings->value("Hotkeys/WatchNext","Ctrl+Right").toString();
    hotkeys.watchFullscreen = qsettings->value("Hotkeys/WatchFullscreen","F10").toString();
    hotkeys.watchSlideshow = qsettings->value("Hotkeys/WatchSlideshow","F5").toString();
    hotkeys.watchWallpaper = qsettings->value("Hotkeys/WatchWallpaper","").toString();
    hotkeys.zoomIn = qsettings->value("Hotkeys/ZoomIn","+").toString();
    hotkeys.zoomOut = qsettings->value("Hotkeys/ZoomOut","-").toString();
    hotkeys.zoomWindow = qsettings->value("Hotkeys/ZoomWindow","").toString();
    hotkeys.zoomOriginal = qsettings->value("Hotkeys/ZoomOriginal","").toString();

    //About//
    hotkeys.helpAbout = qsettings->value("Hotkeys/HelpAbout","F1").toString();

    /// Panel ///
    isneedBut.rotateLeft = (bool)(qsettings->value("Panel/ButtonRotateLeft","1").toInt());
    isneedBut.rotateRight = (bool)(qsettings->value("Panel/ButtonRotateRight","1").toInt());
    isneedBut.flipHorizontal = (bool)(qsettings->value("Panel/ButtonFlipHorizontal","1").toInt());
    isneedBut.flipVertical = (bool)(qsettings->value("Panel/ButtonFlipVertical","1").toInt());
    isneedBut.zoomIn = (bool)(qsettings->value("Panel/ButtonZoomIn","1").toInt());
    isneedBut.zoomOut = (bool)(qsettings->value("Panel/ButtonZoomOut","1").toInt());
    isneedBut.zoomWindow = (bool)(qsettings->value("Panel/ButtonZoomWindow","1").toInt());
    isneedBut.zoomOriginal = (bool)(qsettings->value("Panel/ButtonZoomOriginal","1").toInt());
    isneedBut.fullscreen = (bool)(qsettings->value("Panel/ButtonFullscreen","1").toInt());
    isneedBut.slideshow = (bool)(qsettings->value("Panel/ButtonSlideshow","1").toInt());
    isneedBut.properties = (bool)(qsettings->value("Panel/ButtonProperties","0").toInt());

    int indx=0;
    while (!qsettings->value("ExternProgram"+QString::number(indx)+"/Name","").toString().isEmpty())
    {
        QString name = qsettings->value("ExternProgram"+QString::number(indx)+"/Name","").toString();
        QString icon = qsettings->value("ExternProgram"+QString::number(indx)+"/Icon","").toString();
        QString command = qsettings->value("ExternProgram"+QString::number(indx)+"/Command","").toString();

        QExternProgram *editor = new QExternProgram(name,icon,command,imagewidget);
        editors.append(editor);
        QAction * action = new QAction(QIcon(QPixmap(icon)),name,this);
        editorsActions.append(action);
        connect(action,SIGNAL(triggered()),editor,SLOT(exec()));
        ui->menuExtern_editors->insertAction(ui->editorsNewAction,action);
        indx++;
    }
    if (editors.size() != 0) ui->menuExtern_editors->insertSeparator(ui->editorsNewAction);
    savesettings();
}

void QImageViewer::savesettings()
{
    QString directory = lastdirectory;
#ifdef Q_OS_WIN32
    for (int i=0;i<directory.size();i++)
    {
        if (directory[i] == '/')
        {
            directory.remove(i,1);
            directory.insert(i,"\\");
            i++;
        }
    }
#endif
    qsettings->setValue("Programm/Language",language);
    qsettings->setValue("Programm/Directory",directory);
    qsettings->setValue("Programm/CurrentIconPack",currenticonpackString);

    qsettings->setValue("Navigation/MouseFullscreen",QString::number(imagewidget->getMouseFullscreen()));
    qsettings->setValue("Navigation/MouseZoom",QString::number(imagewidget->getMouseZoom()));

    qsettings->setValue("Panel/Alignment",panelalignment);

    qsettings->setValue("Slideshow/SmoothTransition",QString::number(slideshowSmoothTransition));
    qsettings->setValue("Slideshow/Interval",slideshowInterval);

    qsettings->setValue("FullscreenColor/red",fullscreencolor.red());
    qsettings->setValue("FullscreenColor/green",fullscreencolor.green());
    qsettings->setValue("FullscreenColor/blue",fullscreencolor.blue());


    /// Hotkeys ///
    //File//
    qsettings->setValue("Hotkeys/FileOpen",hotkeys.fileOpen);
    qsettings->setValue("Hotkeys/FileSave",hotkeys.fileSave);
    qsettings->setValue("Hotkeys/FileSaveAs",hotkeys.fileSaveAs);
    qsettings->setValue("Hotkeys/FileSettings",hotkeys.fileSettings);
    qsettings->setValue("Hotkeys/FileQuit",hotkeys.fileQuit);

    //Edit//
    qsettings->setValue("Hotkeys/EditUndo",hotkeys.editUndo);
    qsettings->setValue("Hotkeys/EditRedo",hotkeys.editRedo);
    qsettings->setValue("Hotkeys/EditRotateLeft",hotkeys.editRotateRight);
    qsettings->setValue("Hotkeys/EditRotateRight",hotkeys.editRotateLeft);
    qsettings->setValue("Hotkeys/EditFlipHorizontal",hotkeys.editFlipHorizontal);
    qsettings->setValue("Hotkeys/EditFlipVertical",hotkeys.editFlipVertical);
    qsettings->setValue("Hotkeys/EditCrop",hotkeys.editCrop);
    qsettings->setValue("Hotkeys/EditResize",hotkeys.editResize);
    qsettings->setValue("Hotkeys/EditResizeItems",hotkeys.editResizeItems);

    //Watch//
    qsettings->setValue("Hotkeys/WatchPrevious",hotkeys.watchPrevious);
    qsettings->setValue("Hotkeys/WatchNext",hotkeys.watchNext);
    qsettings->setValue("Hotkeys/WatchFullscreen",hotkeys.watchFullscreen);
    qsettings->setValue("Hotkeys/WatchSlideshow",hotkeys.watchSlideshow);
    qsettings->setValue("Hotkeys/WatchWallpaper",hotkeys.watchWallpaper);
    qsettings->setValue("Hotkeys/ZoomIn",hotkeys.zoomIn);
    qsettings->setValue("Hotkeys/ZoomOut",hotkeys.zoomOut);
    qsettings->setValue("Hotkeys/ZoomWindow",hotkeys.zoomWindow);
    qsettings->setValue("Hotkeys/ZoomOriginal",hotkeys.zoomOriginal);

    //About//
    qsettings->setValue("Hotkeys/HelpAbout",hotkeys.helpAbout);

    /// Panel ///
    qsettings->setValue("Panel/ButtonRotateLeft",QString::number(isneedBut.rotateLeft));
    qsettings->setValue("Panel/ButtonRotateRight",QString::number(isneedBut.rotateRight));
    qsettings->setValue("Panel/ButtonFlipHorizontal",QString::number(isneedBut.flipHorizontal));
    qsettings->setValue("Panel/ButtonFlipVertical",QString::number(isneedBut.flipVertical));
    qsettings->setValue("Panel/ButtonZoomIn",QString::number(isneedBut.zoomIn));
    qsettings->setValue("Panel/ButtonZoomOut",QString::number(isneedBut.zoomOut));
    qsettings->setValue("Panel/ButtonZoomWindow",QString::number(isneedBut.zoomWindow));
    qsettings->setValue("Panel/ButtonZoomOriginal",QString::number(isneedBut.zoomOriginal));
    qsettings->setValue("Panel/ButtonFullscreen",QString::number(isneedBut.fullscreen));
    qsettings->setValue("Panel/ButtonSlideshow",QString::number(isneedBut.slideshow));
    qsettings->setValue("Panel/ButtonProperties",QString::number(isneedBut.properties));

    // remove old extern editors
    int indx = 0;
    while (!qsettings->value("ExternProgram"+QString::number(indx)+"/Name","").toString().isEmpty())
    {
        qsettings->remove("ExternProgram"+QString::number(indx)+"/Name");
        qsettings->remove("ExternProgram"+QString::number(indx)+"/Icon");
        qsettings->remove("ExternProgram"+QString::number(indx)+"/Command");
        indx++;
    }
    //add new ectern editors
    for (int i=0;i<editors.size();i++)
    {
        qsettings->setValue("ExternProgram"+QString::number(i)+"/Name",editors[i]->name);
        qsettings->setValue("ExternProgram"+QString::number(i)+"/Icon",editors[i]->icon);
        qsettings->setValue("ExternProgram"+QString::number(i)+"/Command",editors[i]->command);
    }

    qsettings->sync();
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
    ui->editorsNewAction->setStatusTip(tr("Add new extern editor. You will be able to open current image with another editor"));
    connect(ui->editorsNewAction,SIGNAL(triggered()),this,SLOT(newExternEditor()));

    ui->editorsManagerAction->setStatusTip(tr("Set up your extern settings"));
    connect(ui->editorsManagerAction,SIGNAL(triggered()),this,SLOT(exterEditorsManager()));

    for (int i=0;i<editorsActions.size();i++)
        editorsActions[i]->setEnabled(false);

    // Share //
    ui->shareImageShackAction->setStatusTip(tr("Share this image with ImageShack.us"));
    connect(ui->shareImageShackAction,SIGNAL(triggered()),this,SLOT(imageshackShare()));

    ui->shareImageShackListAction->setStatusTip(tr("Share list of images with ImageShack.us"));
    connect(ui->shareImageShackListAction,SIGNAL(triggered()),this,SLOT(imageshackShareList()));

    // Help //
    ui->aboutAction->setStatusTip(tr("Information about program"));
    connect(ui->aboutAction,SIGNAL(triggered()),this,SLOT(helpAbout()));

    ///BUTTONS///
    ui->prevButton->setToolTip(tr("Open previous image"));
    connect(ui->prevButton,SIGNAL(clicked()),this,SLOT(prevImage()));
    ui->prevButton->setFocusPolicy(Qt::NoFocus);

    ui->nextButton->setToolTip(tr("Open next image"));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(nextImage()));
    ui->nextButton->setFocusPolicy(Qt::NoFocus);

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
    // Load icon packs
    QFileDialog dialog;
    if (QDir(iconpacksfolder).exists())
    {
        dialog.selectFile(iconpacksfolder);
        iconpacks = dialog.directory().entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot,QDir::SortByMask);
        for (int i=0;i<iconpacks.size();i++)
        {
#ifdef Q_OS_LINUX
            iconpacks[i] = iconpacksfolder + iconpacks[i] + "/";
#endif
#ifdef Q_OS_WIN32
            iconpacks[i] = iconpacksfolder + iconpacks[i]+ "\\";
#endif
        }
        if (!iconpacks.isEmpty()) iconpacks.insert(0,":/res/");
        else iconpacks.append(":/res/");

        currenticonpack = 0;
        for (int i=0;i<iconpacks.size();i++)
            if (iconpacks[i] == currenticonpackString) currenticonpack = i;
    }
    else
    {
        iconpacks.append(":/res/");
        currenticonpack = 0;
        currenticonpackString = ":/res/";
    }

    //Set Map for icons
    icon["FileOpen"] = "file-open.png";
    icon["FileSave"] = "file-save.png";
    icon["RotateLeft"] = "rotate-left.png";
    icon["RotateRight"] = "rotate-right.png";
    icon["FlipHorizontal"] = "flip-horizontal.png";
    icon["FlipVertical"] = "flip-vertical.png";
    icon["ZoomIn"] = "zoom-in.png";
    icon["ZoomOut"] = "zoom-out.png";
    icon["ZoomWindow"] = "zoom-window.png";
    icon["ZoomOriginal"] = "zoom-original.png";
    icon["Fullscreen"] = "fullscreen.png";
    icon["Wallpaper"] = "wallpaper.png";
    icon["Slideshow"] = "slideshow.png";
    icon["Properties"] = "file-properties.png";
    icon["Resize"] = "resize.png";
    icon["ResizeItems"] = "resize-items.png";
    icon["Crop"] = "crop.png";
    icon["Add"] = "add.png";
    icon["Remove"] = "remove.png";
    icon["Update"] = "update.png";
    icon["Settings"] = "settings.png";
    icon["Hotkey"] = "hotkey.png";
    icon["Delete"] = "delete.png";
    icon["Help"] = "help.png";
    icon["Imageshack"] = "imageshack.png";
    icon["Next"] = "next.png";
    icon["Previous"] = "prev.png";
    icon["ExternEditorsManager"] = "extern-editor.png";
    icon["ExternEditorNew"] = "extern-editor-new.png";

    //Menu actions
    ui->openAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["FileOpen"])));
    ui->saveAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["FileSave"])));
    ui->settingsAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Settings"])));
    ui->resizeAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Resize"])));
    ui->resizeitemsAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ResizeItems"])));
    ui->cropAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Crop"])));
    ui->rotateLeftAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["RotateLeft"])));
    ui->rotateRightAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["RotateRight"])));
    ui->flipHorizontalAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["FlipHorizontal"])));
    ui->flipVerticalAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["FlipVertical"])));
    ui->fullscreenAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Fullscreen"])));
    ui->slideshowAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Slideshow"])));
    ui->deleteFileAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Delete"])));
    ui->nextButton->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Next"])));
    ui->prevButton->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Previous"])));
    ui->nextimageAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Next"])));
    ui->previmageAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Previous"])));
    ui->zoomInAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ZoomIn"])));
    ui->zoomOutAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ZoomOut"])));
    ui->zoomOriginalAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ZoomOriginal"])));
    ui->zoomWindowAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ZoomWindow"])));
    ui->wallpaperAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Wallpaper"])));
    ui->shareImageShackAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Imageshack"])));
    ui->shareImageShackListAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Imageshack"])));
    ui->aboutAction->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Help"])));

    imagewidget->loadiconnames(icon);
    imagewidget->loadiconpack(iconpacks[currenticonpack]);
}

void QImageViewer::createPanel()
{
    if (isneedBut.rotateLeft)
    {
        butRotateLeft = new QPushButton;
        butRotateLeft->setToolTip(tr("Rotate picture to the left"));
        connect(butRotateLeft,SIGNAL(clicked()),imagewidget,SLOT(rotateLeft()));
        butRotateLeft->setFocusPolicy(Qt::NoFocus);
        butRotateLeft->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["RotateLeft"])));
        butRotateLeft->setEnabled(false);
        buttonsList << butRotateLeft;
    }
    if (isneedBut.rotateRight)
    {
        butRotateRight = new QPushButton;
        butRotateRight->setToolTip(tr("Rotate picture to the right"));
        connect(butRotateRight,SIGNAL(clicked()),imagewidget,SLOT(rotateRight()));
        butRotateRight->setFocusPolicy(Qt::NoFocus);
        butRotateRight->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["RotateRight"])));
        butRotateRight->setEnabled(false);
        buttonsList << butRotateRight;
    }
    if (isneedBut.flipHorizontal)
    {
        butFlipHorizontal = new QPushButton;
        butFlipHorizontal->setToolTip(tr("Change this image to horizontal mirror"));
        connect(butFlipHorizontal,SIGNAL(clicked()),imagewidget,SLOT(flipHorizontal()));
        butFlipHorizontal->setFocusPolicy(Qt::NoFocus);
        butFlipHorizontal->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["FlipHorizontal"])));
        butFlipHorizontal->setEnabled(false);
        buttonsList << butFlipHorizontal;
    }
    if (isneedBut.flipVertical)
    {
        butFlipVertical = new QPushButton;
        butFlipVertical->setToolTip(tr("Change this image to vertical mirror"));
        connect(butFlipVertical,SIGNAL(clicked()),imagewidget,SLOT(flipVertical()));
        butFlipVertical->setFocusPolicy(Qt::NoFocus);
        butFlipVertical->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["FlipVertical"])));
        butFlipVertical->setEnabled(false);
        buttonsList << butFlipVertical;
    }
    if (isneedBut.zoomIn)
    {
        butZoomIn = new QPushButton;
        butZoomIn->setToolTip(tr("Zoom In"));
        connect(butZoomIn,SIGNAL(clicked()),imagewidget,SLOT(zoomInc()));
        butZoomIn->setFocusPolicy(Qt::NoFocus);
        butZoomIn->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ZoomIn"])));
        butZoomIn->setEnabled(false);
        buttonsList << butZoomIn;
    }
    if (isneedBut.zoomOut)
    {
        butZoomOut = new QPushButton;
        butZoomOut->setToolTip(tr("Zoom Out"));
        connect(butZoomOut,SIGNAL(clicked()),imagewidget,SLOT(zoomDec()));
        butZoomOut->setFocusPolicy(Qt::NoFocus);
        butZoomOut->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ZoomOut"])));
        butZoomOut->setEnabled(false);
        buttonsList << butZoomOut;
    }
    if (isneedBut.zoomWindow)
    {
        butZoomWindow = new QPushButton;
        butZoomWindow->setToolTip(tr("Zoom to window size"));
        connect(butZoomWindow,SIGNAL(clicked()),imagewidget,SLOT(reloadImage()));
        butZoomWindow->setFocusPolicy(Qt::NoFocus);
        butZoomWindow->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ZoomWindow"])));
        butZoomWindow->setEnabled(false);
        buttonsList << butZoomWindow;
    }
    if (isneedBut.zoomOriginal)
    {
        butZoomOriginal = new QPushButton;
        butZoomOriginal->setToolTip(tr("Zoom to original size"));
        connect(butZoomOriginal,SIGNAL(clicked()),imagewidget,SLOT(setOriginalSize()));
        butZoomOriginal->setFocusPolicy(Qt::NoFocus);
        butZoomOriginal->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ZoomOriginal"])));
        butZoomOriginal->setEnabled(false);
        buttonsList << butZoomOriginal;
    }
    if (isneedBut.fullscreen)
    {
        butFullscreen = new QPushButton;
        butFullscreen->setToolTip(tr("Enable fullscreen mode"));
        connect(butFullscreen,SIGNAL(clicked()),this,SLOT(fullScreen()));
        butFullscreen->setFocusPolicy(Qt::NoFocus);
        butFullscreen->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Fullscreen"])));
        butFullscreen->setEnabled(false);
        buttonsList << butFullscreen;
    }
    if (isneedBut.slideshow)
    {
        butSlideshow = new QPushButton;
        butSlideshow->setToolTip(tr("Start slideshow in fullscreen mode"));
        connect(butSlideshow,SIGNAL(clicked()),this,SLOT(slideShow()));
        butSlideshow->setFocusPolicy(Qt::NoFocus);
        butSlideshow->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Slideshow"])));
        butSlideshow->setEnabled(false);
        buttonsList << butSlideshow;
    }
    if (isneedBut.properties)
    {
        butProperties = new QPushButton;
        butProperties->setToolTip(tr("Image properties"));
        connect(butProperties,SIGNAL(clicked()),imagewidget,SLOT(viewProperties()));
        butProperties->setFocusPolicy(Qt::NoFocus);
        butProperties->setIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["Properties"])));
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
    savesettings();

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
