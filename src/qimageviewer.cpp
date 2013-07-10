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
    // Load preview panel //
    createPreviews();

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

void QImageViewer::createPreviews()
{
    previewListWidget = new QListWidget;
    previewListWidget->setIconSize(QSize(70, 70));
    previewListWidget->setViewMode(QListWidget::IconMode);
    previewListWidget->setMaximumHeight(100);
    previewListWidget->resize(previewListWidget->sizePolicy().horizontalPolicy(),
                              previewListWidget->sizePolicy().verticalPolicy());
    previewListWidget->showNormal();
    previewListWidget->setUniformItemSizes(true);
    previewListWidget->setWrapping(false);
    previewListWidget->setGridSize(QSize(100,100));
    ui->mainToolBar->addWidget(previewListWidget);

    //
    ui->mainToolBar->hide();
    //
    previewLoader = new previewList(previewListWidget);
}
void QImageViewer::checkupdates()
{
    if (isUpdateDialogRunning) return;
    isUpdateDialogRunning = true;
    updater = new QNetworkAccessManager;
    updater->get(QNetworkRequest(QUrl("http://qiv.p.ht/version.php?notfuckyou=true")));
    updater->connect(updater,SIGNAL(finished(QNetworkReply*)),this,SLOT(getUpdates(QNetworkReply*)));
}

QByteArray md5 (const QByteArray& data)
{
    return QCryptographicHash::hash(data,QCryptographicHash::Md5).toHex();
}

QStringList getDirFiles(QString dir_name)
{
    QStringList ret_list;
    QDir dir(dir_name);
    QFileInfoList info_list = dir.entryInfoList();
    QList<QFileInfo>::iterator iter=info_list.begin();
    QString path;
    for(iter=info_list.begin();iter != info_list.end();iter++)
    {
        path = iter->absoluteFilePath();
        if(iter->isDir() && !path.endsWith("\\..") && !path.endsWith("\\.")) ret_list += getDirFiles(path);
        else ret_list.append(path);
    }
    return ret_list;
}

void QImageViewer::getUpdates(QNetworkReply* reply)
{
    // Check request
    QByteArray replyContent = reply->readAll();
    if (replyContent == "Invalid request")
    {
        qDebug() << "Invalid request";
        return;
    }

    // Parse answer
    QVariant response = JSON::parse(replyContent);
    // Last avaiable version
    QString lastVersion(response.toMap()["current_version"].toString());
    qDebug() << lastVersion;

    // If it's last version, show message;
    QString currentVersion = QApplication::applicationVersion();
    if (lastVersion <= currentVersion)
    {
        isUpdateDialogRunning = false;
        if (!automateUpdate) QMessageBox::information(this,tr("Last version"),tr("You have last version"),
                                 QMessageBox::Ok);
        return;
    }

    // Get last version

#ifdef Q_OS_WIN32
    // All files for windows version
    QStringList updateFiles(response.toMap()["files"].toString().split(";"));
    updateFiles.removeLast();
    for (int i=0;i<updateFiles.size();i++) updateFiles[i] = updateFiles[i].right(updateFiles[i].size()-37);
    // Take map file:md5
    QMap<QString,QString> updateFilesMap;
    foreach (QString file, updateFiles)
    {
        QStringList detail = file.split(":");
        detail[0] = detail[0].toLower();
        updateFilesMap[detail[0]] = detail[1];
    }
    updateFiles = updateFilesMap.keys();
    qDebug() << "UPDATED:";
    foreach (QString file, updateFiles)
    {
        qDebug() << "file:"<<file;
        qDebug() << "md5: "<<updateFilesMap[file];
    }

    // Check current files
    existDir = QApplication::applicationDirPath();
    QStringList existFiles = getDirFiles(existDir);
    for (int i=0;i<existFiles.size();i++)
    {
        existFiles[i] = existFiles[i].right(existFiles[i].size()-existDir.size()-1);
        existFiles[i] = existFiles[i].toLower();
    }

    QMap<QString,QString> existFilesMap;
    foreach (QString file,existFiles)
    {
        QFile truefile(existDir+"\\"+file);
        truefile.open(QIODevice::ReadOnly);
        if (!truefile.exists()) qDebug() << "file "<<existDir+"\\"+file<<"is missing!!!!!!!!!!!!!";
        existFilesMap[file] = QString(md5(truefile.readAll()));
    }
    qDebug() << "EXIST:";
    foreach (QString file, existFiles)
    {
        qDebug() << "file:"<<file;
        qDebug() << "md5: "<<existFilesMap[file];
    }

    needToUpdate.clear();
    foreach (QString file, updateFiles)
    {
        if (existFilesMap[file] != updateFilesMap[file]) needToUpdate.append(file);
    }

    qDebug() << "===================================";
    if (needToUpdate.isEmpty())
    {
        qDebug() << "NOTHING TO UPDATE";
        return;
    }
    else foreach (QString file, needToUpdate) qDebug() << "Update: "<< file;
#endif
#ifdef Q_OS_LINUX
    existDir = QDir::homePath();
    qDebug() << "http://qiv.p.ht/bin/"+response.toMap()["deb-package"].toString();
    needToUpdate.append(response.toMap()["deb-package"].toString());
#endif
    // Get changelog
    QNetworkAccessManager * m_NetworkMngr = new QNetworkAccessManager(this);
    QNetworkReply *replyChangelog = m_NetworkMngr->get(
                QNetworkRequest(QUrl("http://qiv.p.ht/changelog.txt")));
    QEventLoop loop;
    connect(replyChangelog, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();
    QString changelog(replyChangelog->readAll());

    // Ask about update
    if (UpdateDialog(this,currentVersion,lastVersion,changelog).exec())
    {
        qDebug() << "Okey, lets start";
        QDir().mkdir(existDir+"/update");
        updateInformer = new UpdateInformer;
        updateInformer->show();
        updateFile(0);
    } else isUpdateDialogRunning = false;

    automateUpdate = false;
    delete updater;
}

void QImageViewer::updateFile(int number)
{
    updateInformer->setProgress((double)number / (double)needToUpdate.size(),needToUpdate[number]);
    QNetworkAccessManager * m_NetworkMngr = new QNetworkAccessManager(this);
#ifdef Q_OS_WIN32
    QNetworkReply *reply = m_NetworkMngr->get(
                QNetworkRequest(QUrl("http://qiv.p.ht/windows/"+needToUpdate[number])));
#endif
#ifdef Q_OS_LINUX
    QNetworkReply *reply = m_NetworkMngr->get(
                QNetworkRequest(QUrl("http://qiv.p.ht/bin/"+needToUpdate[number])));
#endif
    QEventLoop loop;
    connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();

    if (needToUpdate[number].contains('/'))
    {
        int slashPos;
        for (int i=needToUpdate[number].size()-1;i>0;i--)
            if (needToUpdate[number][i] == '/')
            {
                slashPos = i;
                break;
            }
        QDir().mkdir(existDir+"/update/"+needToUpdate[number].left(slashPos));
    }
    QFile file(existDir+"/update/"+needToUpdate[number]);
    if (!file.open(QIODevice::WriteOnly)) qDebug() <<"Can't open file";
    file.write(reply->readAll());
    file.flush();
    file.close();
    delete reply;
    if (number < needToUpdate.size()-1) updateFile(number+1);
    else afterUpdates();
}

void QImageViewer::afterUpdates()
{
    qDebug() << "Updating finished";
    updateInformer->close();
    delete updateInformer;
    isUpdateDialogRunning = false;
#ifdef Q_OS_WIN32
    QFile updater(existDir+"\\updater.bat");
    updater.open(QIODevice::WriteOnly);
    QTextStream out(&updater);
    // Clear console
    out << "@echo off\n";
    out << "prompt Please wait some seconds ;)\n";
    // Delay for closing application
    out << "ping -n 3 127.0.0.1 > NUL\n";
    // moving all new files
    foreach(QString file, needToUpdate)
    {
        file.replace('/','\\');
        out << "move /y ";
        out << '"' << existDir+"\\update\\"+file<<'"';
        out << " ";
        out << '"' << existDir+"\\"+file<<'"';
        out << "\n";
    }
    // clear temp files
    out << "rd /s /q " << '"' << existDir+"\\update\\" << '"' << "\n";
    // run new version
    out << '"'<<QApplication::applicationDirPath()+"\\QImageViewer.exe"<<'"' << "\n";
    // delete temp 'bat' updater
    out << "del "<< '"' << existDir+"\\updater.bat" << '"';
    updater.flush();
    updater.close();
    QProcess::startDetached("cmd.exe", QStringList() << "/c" << existDir+"\\updater.bat");
    imagewidget->setSaved();
    close();
#endif
#ifdef Q_OS_LINUX
    QFile updater(existDir+"/updater.sh");
    updater.open(QIODevice::WriteOnly);
    QTextStream out(&updater);
    out << "#!/bin/sh\n";
    out << "gksu "<< '"' << "gdebi -n " << existDir+"/update/"+needToUpdate[0]<< '"' << "\n";
    out << "qimageviewer\n";
    out << "rm "<< '"' << existDir+"/updater.sh" << '"' << "\n";
    out << "rm -rf "<< '"' << existDir+"/update"<< '"' << "\n";
    updater.flush();
    updater.close();
    QProcess::startDetached("bash", QStringList() << existDir+"/updater.sh");
    imagewidget->setSaved();
    close();
#endif
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
#ifdef Q_OS_WIN32
    for (int i=1;i<lastdirectory.size();i++)
        if (lastdirectory[i] == '/')
        {
            lastdirectory.remove(i,1);
            lastdirectory.insert(i,"\\");
        }
    for (int i=0;i<imagefiles.size();i++) imagefiles[i] = lastdirectory + '\\' + imagefiles[i];
#endif
#ifdef Q_OS_LINUX
    for (int i=0;i<imagefiles.size();i++) imagefiles[i] = lastdirectory + '/' + imagefiles[i];
#endif

    previewListWidget->clear();
    previewListWidget->setResizeMode(QListView::Adjust);

    imagewidget->loadimagelist(imagefiles);

    //Previews//
    previewLoader->loadList(imagefiles);
    connect(&previewThread,SIGNAL(started()),previewLoader,SLOT(run()));
    connect(previewLoader,SIGNAL(finished()),&previewThread,SLOT(terminate()));
    previewLoader->moveToThread(&previewThread);
    previewThread.start();
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
    // init editResize window //
    editFormResizeElements = new editformResizeElements;
    editFormResizeElements->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ResizeItems"])));
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
}

#ifdef Q_OS_WIN32
QStringList GetArchivesList(QString dir_name)
{
    QStringList ret_list;
    QDir dir(dir_name);
    QFileInfoList info_list = dir.entryInfoList();
    if(info_list.size() > 2)
    {
        QList<QFileInfo>::iterator iter=info_list.begin();
        QString path;
        for(iter=info_list.begin()+2;iter != info_list.end();iter++)
        {
            path = iter->absoluteFilePath();
            if(iter->isDir()) ret_list += GetArchivesList(path);
            else if (path.endsWith(".lnk",Qt::CaseInsensitive)) ret_list.append(path);
        }
    }
    return ret_list;
}
#endif
void QImageViewer::makeInstalledSoftList()
{
#ifdef Q_OS_LINUX
    QDir dir;
    dir.setPath("/usr/share/applications/");
    QStringList filter("*.desktop");
    QStringList applications = dir.entryList(filter,QDir::NoFilter,QDir::SortByMask);
    for (int i=0;i<applications.size();i++)
    {
        applications[i] = applications[i].left( applications[i].size() - 8 );
        QString iconpath;
        if (QFile("/usr/share/icons/hicolor/32x32/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/32x32/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/36x36/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/36x36/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/48x48/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/48x48/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/64x64/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/64x64/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/72x72/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/72x72/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/96x96/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/96x96/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/24x24/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/24x24/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/22x22/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/22x22/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/16x16/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/16x16/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/pixmaps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/pixmaps/"+applications[i]+".png";

        // Application is GUI and has icon
        if (!iconpath.isEmpty())
        {
            // Read *.desktop file //
            QFile file("/usr/share/applications/"+applications[i]+".desktop");
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            QString desktopFile;
            while (!stream.atEnd()) desktopFile += stream.readLine() + "\n";

            // Application is graphical application //
            if (desktopFile.contains("Graphics"))
            {
                QStringList data = desktopFile.split("\n");

                // Parse name //
                QString name = "";
                foreach (QString str, data)
                {
                    if (str.startsWith("Name="))
                    {
                        name = str.right(str.size()-5);
                        break;
                    }
                }
                // Parse command //
                QString command = "";
                foreach (QString str, data)
                {
                    if (str.startsWith("Exec="))
                    {
                        command = str.right(str.size()-5);
                        break;
                    }
                }
                command = command.left(command.size() - 3);

                // Confirm new application
                installedSoft.append(new QExternProgram(name,QIcon(QPixmap(iconpath)),command,imagewidget));
            }
        }
    }

#endif
#ifdef Q_OS_WIN32
    // CURRENT USER APPLICATIONS //
    // Get links
    QString userApplications = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation)[0];
    QStringList targets = GetArchivesList(userApplications);
    QFileIconProvider iconGetter;

    // Check all links
    foreach (QString app, targets)
    {
        // Parse lnk
        QString str = QFile::symLinkTarget(app);
        if (str.endsWith(".exe",Qt::CaseInsensitive)
                && !str.contains("unins",Qt::CaseInsensitive)
                && !str.contains("cmd.exe",Qt::CaseInsensitive)
                && !str.contains("Example",Qt::CaseInsensitive))
        {
            // Get icon from exe
            QIcon icon = iconGetter.icon(QFileInfo(str));

            // Parse name
            int lastSlash = 0;
            for (int i=0;i<app.size();i++) if (app[i] == '/') lastSlash = i+1;
            app = app.right(app.size()-lastSlash);
            app = app.left(app.size()-4);

            installedSoft.append(new QExternProgram(app,icon,str,imagewidget));
        }
    }
    // ALL USERS APPLICATIONS //
    int secondSlash = 0;
    int thirdSlash = 0;
    int slashAmount = 0;
    //find second slash
    for (int i=0;i<userApplications.size();i++)
    {
        if (userApplications[i] == '/')
        {
            secondSlash = i+1;
            slashAmount++;
        }
        if (slashAmount == 2) break;
    }
    //find third slash
    for (int i=secondSlash+1;i<userApplications.size();i++)
        if (userApplications[i] == '/')
        {
            thirdSlash = i;
            break;
        }
    QString allUsersApplications = userApplications.left(secondSlash) +
            "All Users" + userApplications.right(userApplications.size() - thirdSlash);

    targets = GetArchivesList(allUsersApplications);

    // Check all links
    foreach (QString app, targets)
    {
        // Parse lnk
        QString str = QFile::symLinkTarget(app);
        if (str.endsWith(".exe",Qt::CaseInsensitive)
                && !str.contains("unins",Qt::CaseInsensitive)
                && !str.contains("cmd.exe",Qt::CaseInsensitive)
                && !str.contains("Example",Qt::CaseInsensitive))
        {
            // Get icon from exe
            QIcon icon = iconGetter.icon(QFileInfo(str));

            // Parse name
            int lastSlash = 0;
            for (int i=0;i<app.size();i++) if (app[i] == '/') lastSlash = i+1;
            app = app.right(app.size()-lastSlash);
            app = app.left(app.size()-4);

            installedSoft.append(new QExternProgram(app,icon,str,imagewidget));
        }
    }

#endif

}

void QImageViewer::newExternEditor()
{
    if (!isEditorAddFormActive)
    {
        if (installedSoft.isEmpty()) makeInstalledSoftList();
        editorAddForm = new QExternProgramAddForm(installedSoft);
        editorAddForm->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ExternEditorNew"])));
        connect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
        connect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
        editorAddForm->show();
        isEditorAddFormActive = true;
    }
}

void QImageViewer::addEditor(QString name, QIcon icon, QString command)
{
    QExternProgram *editor = new QExternProgram(name,icon,command,imagewidget);
    editors.append(editor);
    QAction * action = new QAction(icon,name,this);
    editorsActions.append(action);
    connect(action,SIGNAL(triggered()),editor,SLOT(exec()));
    ui->menuExtern_editors->insertAction(ui->editorsNewAction,action);
    if (!imagewidget->isReady()) action->setEnabled(false);

    QString dir;
#ifdef Q_OS_LINUX
    dir = QDir::homePath()+"/.config/QImageViewer/extern/";
#endif
#ifdef Q_OS_WIN32
    dir = QApplication::applicationDirPath()+"\\extern\\";
#endif
    if (!QDir(dir).exists())
    {
        QDir creator;
        creator.mkdir(dir);
    }
    QFile file(dir+name+".png");
    file.open(QIODevice::WriteOnly);
    QPixmap pixmap = icon.pixmap(32,32);
    pixmap.save(&file, QString("PNG").toStdString().c_str());

    qDebug() << name << icon <<command;
    disconnect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}

void QImageViewer::abortAddingNewExternEditor()
{
    disconnect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
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
        if (installedSoft.isEmpty()) makeInstalledSoftList();
        editorsManager = new QExternProgramManager(&installedSoft, editors,imagewidget,iconpacks[currenticonpack],icon);
        editorsManager->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ExternEditorsManager"])));
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
            QAction * action = new QAction(editors[i]->icon,editors[i]->name,this);
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
