#include "image.h"
#include "ui_properties.h"

image::image()
{
    isPixmap = false;
    isActiveFullscreen = false;
    setAlignment(Qt::AlignCenter);
    this->installEventFilter(this);

    wasEdited = false;
    mouseGrabbed = false;
    mousezoomCtrlPressed = false;
    connect(horizontalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(horizontalSliderMoverd(int)));
    connect(verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(verticalSliderMoverd(int)));
}

void image::loadhotkeys(hotkeysStruct * hotkeys)
{
    this->hotkeys = hotkeys;
}
void image::loadiconpack(QString iconpack)
{
    iconiconpack = iconpack;
    createContextMenu();
}
void image::loadiconnames(QMap<QString,QString> icons)
{
    this->icon = icons;
}

void image::setMouseZoom(bool arg)
{    mouseZoom = arg;   }
void image::setMouseFullscreen(bool arg)
{    mouseFullscreen = arg; }
bool image::getMouseZoom()
{    return mouseZoom;   }
bool image::getMouseFullscreen()
{    return mouseFullscreen; }

/** Setting context menu **/
void image::createContextMenu()
{
    actionZoomIn = new QAction(tr("Zoom in"),this);
    actionZoomIn->setIcon(QIcon(QPixmap(iconiconpack+icon["ZoomIn"])));
    connect(actionZoomIn,SIGNAL(triggered()),this,SLOT(zoomInc()));

    actionZoomOut = new QAction(tr("Zoom out"),this);
    actionZoomOut->setIcon(QIcon(QPixmap(iconiconpack+icon["ZoomOut"])));
    connect(actionZoomOut,SIGNAL(triggered()),this,SLOT(zoomDec()));

    actionZoomWindow = new QAction(tr("Zoom as Window"),this);
    actionZoomWindow->setIcon(QIcon(QPixmap(iconiconpack+icon["ZoomWindow"])));
    connect(actionZoomWindow,SIGNAL(triggered()),this,SLOT(reloadImage()));

    actionZoomOriginal = new QAction(tr("Zoom Original"),this);
    actionZoomOriginal->setIcon(QIcon(QPixmap(iconiconpack+icon["ZoomOriginal"])));
    connect(actionZoomOriginal,SIGNAL(triggered()),this,SLOT(setOriginalSize()));

    actionFullscreen = new QAction(tr("Fullscreen"),this);
    actionFullscreen->setIcon(QIcon(QPixmap(iconiconpack+icon["Fullscreen"])));
    connect(actionFullscreen,SIGNAL(triggered()),this,SIGNAL(needFullscreen()));

    actionSlideshow = new QAction(tr("Slideshow"),this);
    actionSlideshow->setIcon(QIcon(QPixmap(iconiconpack+icon["Slideshow"])));
    connect(actionSlideshow,SIGNAL(triggered()),this,SIGNAL(needSlideshow()));

    actionWallpaper = new QAction(tr("Set as wallpaper"),this);
    connect(actionWallpaper,SIGNAL(triggered()),this,SLOT(setAsWallpaper()));
    actionWallpaper->setIcon(QIcon(QPixmap(iconiconpack+icon["Wallpaper"])));

    actionDelete = new QAction(tr("Delete"),this);
    actionDelete->setIcon(QIcon(QPixmap(iconiconpack+icon["Delete"])));
    connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteCurrentItem()));

    actionProperties = new QAction(tr("Properties"),this);
    actionProperties->setIcon(QIcon(QPixmap(iconiconpack+icon["Properties"])));
    connect(actionProperties,SIGNAL(triggered()),this,SLOT(viewProperties()));

    ui_prop = new Ui::Properties;
    propertiesWidget = new QDialog;
    ui_prop->setupUi(propertiesWidget);
}

/** event for show context menu **/
void image::contextMenuEvent(QContextMenuEvent *event)
{
    if (isPixmap)
    {
        QMenu menu(this);
        menu.addAction(actionZoomIn);
        menu.addAction(actionZoomOut);
        menu.addAction(actionZoomOriginal);
        menu.addAction(actionZoomWindow);
        menu.insertSeparator(0);
        menu.addAction(actionFullscreen);
        menu.addAction(actionSlideshow);
        menu.insertSeparator(0);
        menu.addAction(actionWallpaper);
        menu.insertSeparator(0);
        menu.addAction(actionDelete);
        menu.insertSeparator(0);
        menu.addAction(actionProperties);
        menu.exec(event->globalPos());
    }
}

/** show 'image properities' **/
void image::viewProperties()
{
    QFileInfo * fileinfo = new QFileInfo(imagename);

    propertiesWidget->show();

    ui_prop->formatView->setPixmap((QPixmap(":/res/format-"+fileinfo->suffix().toLower()+".png")).
                                   scaledToHeight(128));

    ui_prop->fileNameLabel->setText(fileinfo->fileName());
    ui_prop->fileDirLabel->setText(fileinfo->dir().absolutePath());

    ui_prop->imageSizeLabel->setText(QString::number(imagePixmap->width())+"x"+QString::number(imagePixmap->height()));

    ui_prop->createDataLabel->setText(fileinfo->lastModified().toString());
    ui_prop->changesDataLabel->setText(fileinfo->created().toString());

    quint64 size = fileinfo->size();
    double size_show = size;
    QString sizeFormat = tr("byte");
    if (size / 1024 != 0)
    {   size_show = size / 1024.0;
        size /= 1024;
        sizeFormat = tr("KB");  }
    if (size / 1024 != 0)
    {   size_show = size / 1024.0;
        size /= 1024;
        sizeFormat = tr("MB");  }
    ui_prop->fileSizeLabel->setText(QString::number(size_show)+" "+sizeFormat);

    connect(ui_prop->pushButton,SIGNAL(clicked()),propertiesWidget,SLOT(hide()));
}

/** load new image and zoom out it if need**/
void image::loadimage(QString path)
{
    if (isPixmap)
    {
        delete imagePixmap;
        delete imageScene;
    }
    imagePixmap = new QPixmap(path);

    zoom = qMax((double)imagePixmap->width()/(double)width(),
                (double)imagePixmap->height()/(double)height());
    if (zoom > 1) zoom = 1;
    zoomMin = false;
    zoomMax = false;

    imageScene = new QGraphicsScene;

    if (width() == desk.width() && height() == desk.height() &&
            (imagePixmap->width() >= width() || imagePixmap->height() >= height()))
    {
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
            imageScene->addPixmap((*imagePixmap).scaledToWidth(width(), Qt::SmoothTransformation));
        else
            imageScene->addPixmap((*imagePixmap).scaledToHeight(height(), Qt::SmoothTransformation));
    }
    else if (imagePixmap->width() > width() || imagePixmap->height() > height())
    {
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
            imageScene->addPixmap((*imagePixmap).scaledToWidth(width()-5, Qt::SmoothTransformation));
        else
            imageScene->addPixmap((*imagePixmap).scaledToHeight(height()-5, Qt::SmoothTransformation));
    }
    else
    {
        imageScene->addPixmap(*imagePixmap);
    }

    setSceneRect(0,0,imageScene->width(),imageScene->height());
    setScene(imageScene);

    isPixmap = true;
    imagename = path;

    buffer.clear();
    buffer.append(*imagePixmap);
    buffer_indx = 0;
    emit itsPossibleToUndo(false);
    emit itsPossibleToRedo(false);
    setStatusTip(QString::number(imagePixmap->width())+QString("x")+QString::number(imagePixmap->height()));
    emit currentImageWasChanged(imagelist_indx);
    sumMousePos.setX( imageScene->width()/2.0 );
    sumMousePos.setY( imageScene->height()/2.0 );
}

/** load other images in this folder **/
void image::loadimagelist(QStringList list)
{
    imagelist = list;
    imagelist_indx = BinSearch(list,imagename);
}

/** return current index **/
int image::currentImage()
{
    return imagelist_indx;
}

QPixmap image::currentPixmap()
{
    return *imagePixmap;
}

/** return current image's full path **/
QString image::currentImageName()
{
    return imagename;
}

/** return true if image was loaded **/
bool image::isReady()
{
    return isPixmap;
}

/** set true if image was loaded **/
void image::setReady(bool arg)
{
    isPixmap = arg;
}

/** return true if image was saved and all changes was accepted **/
bool image::isSaved()
{
    if (wasEdited) return false;
    return true;
}

QStringList image::getImageList()
{
    return imagelist;
}

/** reset image to window size **/
void image::resetZoom()
{
    zoomOriginal = false;
    zoomMin = false;
    zoomMax = false;
    zoom = qMin((double)width()/(double)imagePixmap->width(),
                (double)height()/(double)imagePixmap->height());
    if (zoom > 1)
    {
        zoom = 1;
        zoomOriginal = true;
    }
    sumMousePos.setX( imageScene->width()/2.0 );
    sumMousePos.setY( imageScene->height()/2.0 );
}

/** set new image from list **/
void image::setImage(int indx)
{
    if (wasEdited)
    {
        int r = QMessageBox::warning(this, tr("Warning!"),
                                        tr("This file was changed\n"
                                           "Do you want to save the changes?"),
                                        QMessageBox::No,
                                        QMessageBox::Yes | QMessageBox::Default,
                                        QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Cancel) return;
        if (r == QMessageBox::Yes) saveimage(imagename);
        wasEdited = false;
    }
    if (indx >= 0 && indx < imagelist.size())
    {
        imagelist_indx = indx;
        loadimage(imagelist[imagelist_indx]);
    }
}

/** zoom to original size **/
void image::setOriginalSize()
{
    int oldwidth = imageScene->width();
    imageScene->clear();
    imageScene->setSceneRect(0,0,1,1);
    imageScene->clear();
    zoom = qMax((double)imagePixmap->width()/(double)width(),
                (double)imagePixmap->height()/(double)height());
    imageScene->setSceneRect(0,0,(*imagePixmap).width(),(*imagePixmap).height());
    imageScene->addPixmap(*imagePixmap);
    setSceneRect(0,0,imageScene->width(),imageScene->height());
    setScene(imageScene);


    ////Center position////
    qDebug() << "Proportion: " << (imageScene->width() / oldwidth)<<"zoom="<<zoom;
    sumMousePos.setX(sumMousePos.x() * (imageScene->width() / oldwidth));
    sumMousePos.setY(sumMousePos.y() * (imageScene->width() / oldwidth));
    centerOn(sumMousePos.x(),sumMousePos.y());
}

/** zoom in **/
void image::zoomInc()
{
    if (!zoomMax)
    {
        int oldwidth = imageScene->width();
        imageScene->clear();
        imageScene->setSceneRect(0,0,1,1);
        imageScene->clear();
        zoomOriginal = false;
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
        {
            if ((zoom+0.2)*(width()*0.95) < imagePixmap->width()*5) zoom += 0.2;
            imageScene->setSceneRect(0,0,(width()*0.95)*zoom,(*imagePixmap).scaledToWidth((width()*0.95)*zoom).height());
            imageScene->addPixmap((*imagePixmap).scaledToWidth((width()*0.95)*zoom, Qt::SmoothTransformation));
            if ((zoom+0.2)*(width()*0.95) > imagePixmap->width()*5.0) zoomMax = true;
        }
        else
        {
            if ((zoom+0.2)*(height()*0.98) < imagePixmap->height()*5) zoom += 0.2;
            imageScene->setSceneRect(0,0,(*imagePixmap).scaledToHeight(height()*0.98*zoom, Qt::SmoothTransformation).width(),height()*0.98*zoom);
            imageScene->addPixmap((*imagePixmap).scaledToHeight(height()*0.98*zoom, Qt::SmoothTransformation));
            if ((zoom+0.2)*(height()*0.98) > imagePixmap->height()*5.0) zoomMax = true;
        }
        setSceneRect(0,0,imageScene->width(),imageScene->height());
        setScene(imageScene);

        ////Center position////
        qDebug() << "Proportion: " << (imageScene->width() / oldwidth)<<"zoom="<<zoom;
        sumMousePos.setX(sumMousePos.x() * (imageScene->width() / oldwidth));
        sumMousePos.setY(sumMousePos.y() * (imageScene->width() / oldwidth));
        centerOn(sumMousePos.x(),sumMousePos.y());
        zoomMin = false;
    }
}

/** zoom out **/
void image::zoomDec()
{
    if (!zoomMin)
    {
        zoomOriginal = false;
        int oldwidth = imageScene->width();
        imageScene->clear();
        imageScene->setSceneRect(0,0,1,1);
        imageScene->clear();
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
        {
            if (zoom > 0.2 &&  (zoom-0.2)*(width()*0.95) > 10) zoom -= 0.2;
            imageScene->setSceneRect(0,0,(width()*0.95)*zoom,(*imagePixmap).scaledToWidth(width()*0.95*zoom, Qt::SmoothTransformation).height());
            imageScene->addPixmap((*imagePixmap).scaledToWidth((width()*0.95)*zoom));
        }
        else
        {
            if (zoom > 0.2 && (zoom-0.2)*(height()*0.98) > 10) zoom -= 0.2;
            imageScene->setSceneRect(0,0,(*imagePixmap).scaledToHeight(height()*0.98*zoom, Qt::SmoothTransformation).width(),height()*0.98*zoom);
            imageScene->addPixmap((*imagePixmap).scaledToHeight(height()*0.98*zoom, Qt::SmoothTransformation));
        }
        setSceneRect(0,0,imageScene->width(),imageScene->height());
        setScene(imageScene);

        ////Center position////
        qDebug() << "Proportion: " << (imageScene->width() / oldwidth)<<"zoom="<<zoom;
        sumMousePos.setX(sumMousePos.x() * (imageScene->width() / oldwidth));
        sumMousePos.setY(sumMousePos.y() * (imageScene->width() / oldwidth));
        centerOn(sumMousePos.x(),sumMousePos.y());
        qDebug() << "!zoom! = " << zoom;

        if (fabs(zoom - 0.2) < 0.001) zoomMin = true;
        zoomMax = false;
    }
}

/** return images' amount **/
int image::size()
{
    return imagelist.size();
}

/** delete current image from disc **/
bool image::deleteCurrentItem()
{
    int r = QMessageBox::warning(this, tr("Delete file"),
                                    tr("Do you really want to delete this file?"),
                                    QMessageBox::Yes | QMessageBox::Default,
                                    QMessageBox::Cancel | QMessageBox::Escape);
    if (r == QMessageBox::Cancel) return false;
    return deleteCurrentItemWithoutAsc();
}

bool image::deleteCurrentItemWithoutAsc()
{
    imagelist.removeAt(imagelist_indx);

    QFile(imagename).remove();

    if (imagelist.size() == 0)
    {
        loadimage(":/res/logo.png");
        isPixmap = false;
        return true;
    }
    wasEdited = false;
    if (imagelist_indx == imagelist.size()) imagelist_indx--;
    loadimage(imagelist[imagelist_indx]);
    emit currentImageWasChanged(imagelist_indx);
    return true;
}

void image::removeFromList(QString name)
{
    imagelist.removeOne(name);
}

/** insert image in list **/
void image::insertImage(QString filename,int pos)
{
    if (pos >=0 & pos<imagelist.size())
    {
        imagelist.insert(pos,filename);
    }
    buffer.clear();
    buffer.append(*imagePixmap);
    buffer_indx = 0;
    emit itsPossibleToUndo(false);
    emit itsPossibleToRedo(false);
    emit itsSaved(true);
}

/** return true if image now is bigger than window **/
bool image::isZoomed()
{
    if (zoom <= 1.0) return false;
    return true;
}
void image::setFullscreen(QColor fullscreencolor)
{
    this->fullscreencolor = fullscreencolor;
    isActiveFullscreen = true;
}

void image::unsetFullscreen()
{
    isActiveFullscreen = false;
}

void image::setAsWallpaperExtern(QString image)
{
    QString name = imagename;
    imagename = image;
    setAsWallpaper();
    imagename = name;
}

#ifdef Q_OS_WIN32
static wchar_t* charToWChar(const char* text)
{
    size_t size = strlen(text) + 1;
    wchar_t* wa = new wchar_t[size];
    mbstowcs(wa,text,size);
    return wa;
}
#endif
void image::setAsWallpaper()
{
    int r = QMessageBox::question(this, tr("Setting wallpaper"),
                                    tr("Do you want to set this image as desktop wallpaper?"),
                                    QMessageBox::Yes | QMessageBox::Default,
                                    QMessageBox::Cancel | QMessageBox::Escape);
    if (r == QMessageBox::Cancel) return;
#ifdef Q_OS_WIN32
    #include <windows.h>
    imagePixmap->save("C:/WINDOWS/qimageviewer-wallpaper.bmp");
    char *path = new char[100];
    strcpy(path, "C:/WINDOWS/qimageviewer-wallpaper.bmp");
    SystemParametersInfo(SPI_SETDESKWALLPAPER,0,charToWChar(path),(SPIF_SENDWININICHANGE + SPIF_UPDATEINIFILE));
    delete [] path;
#endif
#ifdef Q_OS_LINUX
    bool exist = false;
    //Create file
    QFile file("temp.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write("");
    file.close();

    //PCManFM (LXDE)
    system("dpkg -l pcmanfm > temp.txt");

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream out(&file);
    QString state = out.readLine();
    if (state.size()>0)
    {
        exist = true;
        QString command;
        command = "pcmanfm --set-wallpaper="+imagename;
        system(command.toLocal8Bit().data());
    }
    file.close();

    if (!exist)
    {
        //GNOME
        system("dpkg -l gconftool-2 > temp.txt");
        QFile file("temp.txt");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream out(&file);
        QString state = out.readLine();
        if (state.size()>0)
        {
            exist = true;
            QString command;
            command = "gconftool-2 -t str --set /desktop/gnome/background/picture_filename "+imagename;
            system(command.toLocal8Bit().data());
        }
        file.close();
    }

    if (!exist)
    {
        //XCFE
        system("dpkg -l xfconf-query > temp.txt");
        QFile file("temp.txt");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream out(&file);
        QString state = out.readLine();
        if (state.size()>0)
        {
            exist = true;
            QString command;
            command = "xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/image-path -s "+imagename;
            system(command.toLocal8Bit().data());
        }
        file.close();
    }
    file.remove();
#endif
}


