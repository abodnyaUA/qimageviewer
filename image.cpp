#include "image.h"
#include "ui_properties.h"

image::image()
{
    isPixmap = false;
    setAlignment(Qt::AlignCenter);
    this->installEventFilter(this);

    createContextMenu();
    wasEdited = false;
    mouseGrabbed = false;
    connect(horizontalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(horizontalSliderMoverd(int)));
    connect(verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(verticalSliderMoverd(int)));
}

void image::loadhotkeys(hotkeysStruct * hotkeys)
{
    this->hotkeys = hotkeys;
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
    actionZoomIn->setIcon(QIcon(QPixmap(":/res/zoom-in.png")));
    connect(actionZoomIn,SIGNAL(triggered()),this,SLOT(zoomInc()));

    actionZoomOut = new QAction(tr("Zoom out"),this);
    actionZoomOut->setIcon(QIcon(QPixmap(":/res/zoom-out.png")));
    connect(actionZoomOut,SIGNAL(triggered()),this,SLOT(zoomDec()));

    actionZoomWindow = new QAction(tr("Zoom as Window"),this);
    actionZoomWindow->setIcon(QIcon(QPixmap(":/res/zoom-window.png")));
    connect(actionZoomWindow,SIGNAL(triggered()),this,SLOT(reloadImage()));

    actionZoomOriginal = new QAction(tr("Zoom Original"),this);
    actionZoomOriginal->setIcon(QIcon(QPixmap(":/res/zoom-original.png")));
    connect(actionZoomOriginal,SIGNAL(triggered()),this,SLOT(setOriginalSize()));

    actionFullscreen = new QAction(tr("Fullscreen"),this);
    actionFullscreen->setIcon(QIcon(QPixmap(":/res/fullscreen.png")));
    connect(actionFullscreen,SIGNAL(triggered()),this,SIGNAL(needFullscreen()));

    actionSlideshow = new QAction(tr("Slideshow"),this);
    actionSlideshow->setIcon(QIcon(QPixmap(":/res/slideshow.png")));
    connect(actionSlideshow,SIGNAL(triggered()),this,SIGNAL(needSlideshow()));

    actionWallpaper = new QAction(tr("Set as wallpaper"),this);
    connect(actionWallpaper,SIGNAL(triggered()),this,SLOT(setAsWallpaper()));
    actionWallpaper->setIcon(QIcon(QPixmap(":/res/wallpaper.png")));

    actionDelete = new QAction(tr("Delete"),this);
    actionDelete->setIcon(QIcon(QPixmap(":/res/delete.png")));
    connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteCurrentItem()));

    actionProperties = new QAction(tr("Properties"),this);
    actionProperties->setIcon(QIcon(QPixmap(":/res/file-properties.png")));
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

    QGraphicsScene * extensionScene = new QGraphicsScene;
    extensionScene->addPixmap((QPixmap(":/res/format-"+fileinfo->suffix().toLower()+".png")).
                              scaledToHeight(ui_prop->formatView->height()-5));

    ui_prop->formatView->setAttribute(Qt::WA_NoSystemBackground);
    ui_prop->formatView->setScene(extensionScene);

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

    zoom = 1.0;
    zoomMin = false;
    zoomMax = false;

    imageScene = new QGraphicsScene;

    if (width() == desk.width() && height() == desk.height() &&
            (imagePixmap->width() >= width() || imagePixmap->height() >= height()))
    {
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
            imageScene->addPixmap((*imagePixmap).scaledToWidth(width()));
        else
            imageScene->addPixmap((*imagePixmap).scaledToHeight(height()));
    }
    else if (imagePixmap->width() > width() || imagePixmap->height() > height())
    {
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
            imageScene->addPixmap((*imagePixmap).scaledToWidth(width()-5));
        else
            imageScene->addPixmap((*imagePixmap).scaledToHeight(height()-5));
    }
    else
    {
        imageScene->addPixmap(*imagePixmap);
        zoomMax = true;
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

/** Binary search **/
int BinSearch(QStringList arr, QString key)
{
    int left = 0;
    int right = arr.size() - 1;
    int mid;

    while (left <= right)
    {
        mid = (left + right) / 2;
        if (arr[mid] == key) return mid;
        if (arr[mid] < key) left = mid + 1;
        if (arr[mid] > key) right = mid - 1;
    }
    return 0;
}

/** load other images in this folder **/
void image::loadimagelist(QStringList list)
{
    imagelist = list;
    imagelist_indx = BinSearch(list,imagename);
}

void image::setOriginalSize()
{
    while (!zoomMax) zoomInc();
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
    zoom = 1.0;
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
                                           "Do you want accept changes?"),
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

/** zoom in **/
void image::zoomInc()
{
    if (!zoomMax)
    {
        int oldwidth = imageScene->width();
        imageScene->clear();
        imageScene->setSceneRect(0,0,1,1);
        imageScene->clear();
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
        {
            if ((zoom+0.2)*(width()*0.95) < imagePixmap->width()) zoom += 0.2;
            if ((zoom+0.2)*(width()*0.95) < imagePixmap->width())
            {
                imageScene->setSceneRect(0,0,(width()*0.95)*zoom,(*imagePixmap).scaledToWidth((width()*0.95)*zoom).height());
                imageScene->addPixmap((*imagePixmap).scaledToWidth((width()*0.95)*zoom));
            }
            else
            {
                imageScene->setSceneRect(0,0,(*imagePixmap).width(),(*imagePixmap).height());
                imageScene->addPixmap(*imagePixmap);
                zoomMax = true;
            }
        }
        else
        {
            if ((zoom+0.2)*(height()*0.98) < imagePixmap->height()) zoom += 0.2;
            if ((zoom+0.2)*(height()*0.98) < imagePixmap->height())
            {
                imageScene->setSceneRect(0,0,(*imagePixmap).scaledToHeight(((height()*0.98))*zoom).width(),((height()*0.98))*zoom);
                imageScene->addPixmap((*imagePixmap).scaledToHeight(((height()*0.98))*zoom));
            }
            else
            {
                imageScene->setSceneRect(0,0,(*imagePixmap).width(),(*imagePixmap).height());
                imageScene->addPixmap(*imagePixmap);
                zoomMax = true;
            }
        }
        setSceneRect(0,0,imageScene->width(),imageScene->height());
        setScene(imageScene);


        ////ЦЕНТРОВКА////
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
        int oldwidth = imageScene->width();
        imageScene->clear();
        imageScene->setSceneRect(0,0,1,1);
        imageScene->clear();
        if ((double)imagePixmap->width()/(double)width() > (double)imagePixmap->height()/(double)height())
        {
            if (zoom > 0.2 &&  (zoom-0.2)*(width()*0.95) > 10) zoom -= 0.2;
            imageScene->setSceneRect(0,0,(width()*0.95)*zoom,(*imagePixmap).scaledToWidth((width()*0.95)*zoom).height());
            imageScene->addPixmap((*imagePixmap).scaledToWidth((width()*0.95)*zoom));
        }
        else
        {
            if (zoom > 0.2 && (zoom-0.2)*(height()*0.98) > 10) zoom -= 0.2;
            imageScene->setSceneRect(0,0,(*imagePixmap).scaledToHeight(((height()*0.98))*zoom).width(),((height()*0.98))*zoom);
            imageScene->addPixmap((*imagePixmap).scaledToHeight(((height()*0.98))*zoom));
        }
        setSceneRect(0,0,imageScene->width(),imageScene->height());
        setScene(imageScene);

        ////ЦЕНТРОВКА////
        qDebug() << "Proportion: " << (imageScene->width() / oldwidth)<<"zoom="<<zoom;
        sumMousePos.setX(sumMousePos.x() * (imageScene->width() / oldwidth));
        sumMousePos.setY(sumMousePos.y() * (imageScene->width() / oldwidth));
        centerOn(sumMousePos.x(),sumMousePos.y());

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
void image::deleteCurrentItem()
{
    int r = QMessageBox::warning(this, tr("Delete file"),
                                    tr("Do you really want to delete this file?"),
                                    QMessageBox::Yes | QMessageBox::Default,
                                    QMessageBox::Cancel | QMessageBox::Escape);
    if (r == QMessageBox::Cancel) return;

    imagelist.removeAt(imagelist_indx);

    QFile(imagename).remove();

    if (imagelist.size() == 0)
    {
        isPixmap = false;
        return;
    }

    if (imagelist_indx == imagelist.size()) imagelist_indx--;
    loadimage(imagelist[imagelist_indx]);
    emit currentImageWasChanged(imagelist_indx);
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
#ifdef Q_OS_WIN32
    #include <windows.h>
    imagePixmap->save("C:/WINDOWS/system32/qimageviewer-wallpaper.bmp");
    char *path = new char[100];
    strcpy(path, "C:/WINDOWS/system32/qimageviewer-wallpaper.bmp");
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


