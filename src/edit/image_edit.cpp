#include "image.h"
#include "ui_properties.h"

/** reload current image **/
void image::reloadImage()
{
    delete imageScene;
    imageScene = new QGraphicsScene;
    zoom = qMax((double)imagePixmap->width()/(double)width(),
                (double)imagePixmap->height()/(double)height());
    if (zoom > 1) zoom = 1;
    zoomMin = false;
    zoomMax = false;
    zoomOriginal = false;

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
    else imageScene->addPixmap(*imagePixmap);

    setSceneRect(0,0,imageScene->width(),imageScene->height());
    setScene(imageScene);
    sumMousePos.setX( imageScene->width()/2.0 );
    sumMousePos.setY( imageScene->height()/2.0 );
}

/** load pixmap from argument 0 to form and make buffer bigger **/
void image::addToBuffer(QPixmap * pixmap)
{
    if (isMovie)
    {
        imageMovie->stop();
        delete imageMovie;
        isMovie = false;
    }

    delete imageScene;
//    delete imagePixmap;
    imagePixmap = pixmap;

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
    else imageScene->addPixmap(*imagePixmap);

    setSceneRect(0,0,imageScene->width(),imageScene->height());
    setScene(imageScene);

    if (buffer_indx != buffer.size()-1)
    {
        int bufsize = buffer.size()-1;
        for (int i=buffer_indx+1;i<=bufsize;i++)
            buffer.removeLast();
    }
    buffer.append(*imagePixmap);
    buffer_indx = buffer.size()-1;
    emit itsPossibleToUndo(true);
    emit itsPossibleToRedo(false);
    emit itsSaved(false);
    sumMousePos.setX( imageScene->width()/2.0 );
    sumMousePos.setY( imageScene->height()/2.0 );
    wasEdited = true;
}

/** 'undo' function **/
void image::prevBuffer()
{
    wasEdited = true;
    if (buffer_indx>0)
        buffer_indx--;
    *imagePixmap = buffer[buffer_indx];
    reloadImage();
    if (buffer_indx>0)
    {
        emit itsSaved(false);
        emit itsPossibleToUndo(true);
    }
    else
    {
        if (wasMovie)
        {
            imageMovie = new QMovie(imagename);
            imageMovie->start();
            connect(imageMovie,SIGNAL(updated(QRect)),this,SLOT(onMovieUpdated()));
            isMovie = true;
        }
        emit itsPossibleToUndo(false);
        setSaved();
    }
    emit itsPossibleToRedo(true);
}

/** 'redo' function **/
void image::nextBuffer()
{
    wasEdited = true;
    if (isMovie)
    {
        imageMovie->stop();
        delete imageMovie;
        isMovie = false;
    }
    if (buffer_indx == 0)
        emit itsSaved(false);
    if (buffer_indx < buffer.size()-1)
        buffer_indx++;
    *imagePixmap = buffer[buffer_indx];
    reloadImage();
    if (buffer_indx < buffer.size()-1)
        emit itsPossibleToRedo(true);
    else
        emit itsPossibleToRedo(false);
    emit itsPossibleToUndo(true);
}

/** rotate image left **/
void image::rotateLeft()
{
    QTransform transform;
    transform = transform.rotate(-90);
    QPixmap * newpixmap = new QPixmap(imagePixmap->transformed(transform));
    addToBuffer(newpixmap);
}

/** rotate image right **/
void image::rotateRight()
{
    QTransform transform;
    transform = transform.rotate(90);
    QPixmap * newpixmap = new QPixmap(imagePixmap->transformed(transform));
    addToBuffer(newpixmap);
}

/** Create horizontal flip of current image (Reflection) **/
void image::flipHorizontal()
{
    QImage image = imagePixmap->toImage();
    image = image.mirrored(true,false);
    QPixmap * newpixmap = new QPixmap(QPixmap::fromImage(image));
    addToBuffer(newpixmap);
}

/** Create vertical flip of current image (Reflection) **/
void image::flipVertical()
{
    QImage image = imagePixmap->toImage();;
    image = image.mirrored(false,true);
    QPixmap * newpixmap = new QPixmap(QPixmap::fromImage(image));
    addToBuffer(newpixmap);
}

/** save current image **/
void image::saveimage(QString filename)
{
    QString format = (QFileInfo(filename)).suffix().toUpper();
    if (format == "")
    {
        format = "PNG";
        filename += ".png";
    }
    if (format != "PNG" && format != "BMP" && format != "JPG" &&
            format != "JPEG" && format != "PPM" && format != "XBM" &&
            format != "XPM" && format != "TIFF")
    {
        filename = filename.left(filename.size() - format.size()) + QString(".png");
        format = "PNG";
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    imagePixmap->save(&file, format.toStdString().c_str());
}

/** image was saved from other window **/
void image::setSaved()
{
    wasEdited = false;
    emit itsSaved(true);
}

/** image was edit from other window **/
void image::setEdited()
{
    wasEdited = true;
    emit itsSaved(false);
}
