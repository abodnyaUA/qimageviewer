#include "image.h"
#include "ui_properties.h"

/** reload current image **/
void image::reloadImage()
{
    delete imageScene;
    imageScene = new QGraphicsScene;
    zoom = 1.0;

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
    else imageScene->addPixmap(*imagePixmap);

    setSceneRect(0,0,imageScene->width(),imageScene->height());
    setScene(imageScene);
}

/** load pixmap from argument 0 to form and make buffer bigger **/
void image::addToBuffer(QPixmap * pixmap)
{
    delete imageScene;
    imagePixmap = pixmap;

    zoom = 1.0;

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
        emit itsPossibleToUndo(false);
        setSaved();
    }
    emit itsPossibleToRedo(true);
}

/** 'redo' function **/
void image::nextBuffer()
{
    wasEdited = true;
    if (buffer_indx < buffer.size()-1)
    {
        qDebug() << "current index is changed";
        buffer_indx++;
    }
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
    QTransform trans = transform.rotate(-90);
    QPixmap * newpixmap = new QPixmap(imagePixmap->transformed(trans));
    addToBuffer(newpixmap);
    wasEdited = true;
}

/** rotate image right **/
void image::rotateRight()
{
    QTransform transform;
    QTransform trans = transform.rotate(90);
    QPixmap * newpixmap = new QPixmap(imagePixmap->transformed(trans));
    addToBuffer(newpixmap);
    wasEdited = true;
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
            format != "XPM" && format != "TIFF") format = "PNG";

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
