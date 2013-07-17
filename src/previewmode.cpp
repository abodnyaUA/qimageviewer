#include "previewmode.h"

PreviewMode::PreviewMode()
{
    setIconSize(QSize(70, 70));
    setViewMode(QListWidget::IconMode);
    //showNormal();
    //setUniformItemSizes(true);
    setMovement(QListView::Static);
    setResizeMode(QListView::Adjust);
    //setWrapping(false);
    setGridSize(QSize(100,100));
    previewThread = new QThread;
    previewLoader = new previewList(this,previewThread);
    connect(this,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onItemDoubleClicked(QListWidgetItem*)));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    installEventFilter(this);
}

void PreviewMode::loadImages(QStringList images)
{
    previewThread->terminate();
    clear();
    this->images = images;
    qDebug() << "Folder"<<folder();
    foreach (QString image, images)
    {
        addItem(image.right(image.size()-folder().size()-1).left(10));
    }
    previewLoader->loadList(images);
    connect(previewThread,SIGNAL(started()),previewLoader,SLOT(run()));
    connect(previewLoader,SIGNAL(finished()),previewThread,SLOT(terminate()));
    connect(previewLoader,SIGNAL(finished()),this,SLOT(updateView()));
    qDebug() << "I'm here";
    previewLoader->moveToThread(previewThread);
    previewThread->start();
}

void PreviewMode::updateView()
{
    qDebug() << "I'm ready";
    for (int i = 0;i<images.size();i++)
        item(i)->setIcon(QIcon(QPixmap::fromImage(previewLoader->icons[i])));
    previewLoader->icons.clear();
    if (count() > 0) item(0)->setIcon(QIcon(QPixmap(images[0]).scaled(100,100)));
    repaint();
    emit ready();
}

QStringList PreviewMode::getList()
{
    return images;
}

void PreviewMode::removeImage(QString name)
{
    int indx = BinSearch(images,name);
    delete item(indx);
    images.removeAt(indx);
}

bool PreviewMode::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        qDebug() << "key was pressed";
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int keyInt = keyEvent->key();
        QString key = QKeySequence(keyInt).toString();
        if (key == "Return" && !selectedItems().isEmpty()) onItemDoubleClicked(selectedItems()[0]);
    }
}

void PreviewMode::onItemDoubleClicked(QListWidgetItem *item)
{
#ifdef Q_OS_LINUX
    emit openImage(BinSearch(images,folder() + "/" + item->text()));
#endif
#ifdef Q_OS_WIN32
    emit openImage(BinSearch(images,folder() + "\\" + item->text()));
#endif
}

int PreviewMode::size()
{
    return images.size();
}
QString PreviewMode::folder()
{
    int lastSlash = 0;
    for (int i=0;i<images[0].size();i++) if (images[0][i] == '/' || images[0][i] == '\\') lastSlash = i;
    return images[0].left(lastSlash);
}


void PreviewMode::reloadPreview(int index)
{
    item(index)->setIcon(QIcon(QPixmap(images[index]).scaled(100,100)));
}

void PreviewMode::reloadPreviews(QList<int> indexes)
{

}

bool PreviewMode::rotateLeft(QString currentImage, bool isSaved)
{
    bool needReload = false;
    foreach(QListWidgetItem *item,selectedItems())
    {
#ifdef Q_OS_MAC
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_LINUX
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_WIN32
        QString imagename = folder() + "\\" + item->text();
#endif
        if (imagename == currentImage && isSaved) needReload = true;
        if (!(imagename == currentImage && !isSaved))
        {
            QTransform transform;
            transform = transform.rotate(-90);
            QPixmap newpixmap(QPixmap(imagename).transformed(transform));
            QFile file(imagename);
            file.open(QIODevice::WriteOnly);
            QString format = (QFileInfo(imagename)).suffix().toUpper();
            newpixmap.save(&file, format.toStdString().c_str());
            file.flush();
            file.close();
            item->setIcon(QIcon(QPixmap(imagename).scaled(100,100)));
#ifdef Q_OS_MAC
            item->setText(imagename.right(imagename.size()-folder().size()-1).left(10));
#endif
        }
    }
    return needReload;
}

bool PreviewMode::rotateRight(QString currentImage, bool isSaved)
{
    bool needReload = false;
    foreach(QListWidgetItem *item,selectedItems())
    {
#ifdef Q_OS_MAC
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_LINUX
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_WIN32
        QString imagename = folder() + "\\" + item->text();
#endif
        if (imagename == currentImage && isSaved) needReload = true;
        if (!(imagename == currentImage && !isSaved))
        {
            QTransform transform;
            transform = transform.rotate(90);
            QPixmap newpixmap(QPixmap(imagename).transformed(transform));
            QFile file(imagename);
            file.open(QIODevice::WriteOnly);
            QString format = (QFileInfo(imagename)).suffix().toUpper();
            newpixmap.save(&file, format.toStdString().c_str());
            file.flush();
            file.close();
            item->setIcon(QIcon(QPixmap(imagename).scaled(100,100)));
#ifdef Q_OS_MAC
            item->setText(imagename.right(imagename.size()-folder().size()-1).left(10));
#endif
        }
    }
    return needReload;
}

bool PreviewMode::flipHorizontal(QString currentImage, bool isSaved)
{
    bool needReload = false;
    foreach(QListWidgetItem *item,selectedItems())
    {
#ifdef Q_OS_MAC
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_LINUX
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_WIN32
        QString imagename = folder() + "\\" + item->text();
#endif
        if (imagename == currentImage && isSaved) needReload = true;
        if (!(imagename == currentImage && !isSaved))
        {
            QImage image(imagename);
            image = image.mirrored(true,false);
            QPixmap newpixmap(QPixmap::fromImage(image));
            QFile file(imagename);
            file.open(QIODevice::WriteOnly);
            QString format = (QFileInfo(imagename)).suffix().toUpper();
            newpixmap.save(&file, format.toStdString().c_str());
            file.flush();
            file.close();
            item->setIcon(QIcon(QPixmap(imagename).scaled(100,100)));
#ifdef Q_OS_MAC
            item->setText(imagename.right(imagename.size()-folder().size()-1).left(10));
#endif
        }
    }
    return needReload;
}

bool PreviewMode::flipVertical(QString currentImage, bool isSaved)
{
    bool needReload = false;
    foreach(QListWidgetItem *item,selectedItems())
    {
#ifdef Q_OS_MAC
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_LINUX
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_WIN32
        QString imagename = folder() + "\\" + item->text();
#endif
        if (imagename == currentImage && isSaved) needReload = true;
        if (!(imagename == currentImage && !isSaved))
        {
            QImage image(imagename);
            image = image.mirrored(false,true);
            QPixmap newpixmap(QPixmap::fromImage(image));
            QFile file(imagename);
            file.open(QIODevice::WriteOnly);
            QString format = (QFileInfo(imagename)).suffix().toUpper();
            newpixmap.save(&file, format.toStdString().c_str());
            file.flush();
            file.close();
            item->setIcon(QIcon(QPixmap(imagename).scaled(100,100)));
#ifdef Q_OS_MAC
            item->setText(imagename.right(imagename.size()-folder().size()-1).left(10));
#endif
        }
    }
    return needReload;
}

QStringList PreviewMode::selectedImages()
{
    QStringList items;
    foreach(QListWidgetItem *item,selectedItems())
    {
#ifdef Q_OS_MAC
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_LINUX
        QString imagename = folder() + "/" + item->text();
#endif
#ifdef Q_OS_WIN32
        QString imagename = folder() + "\\" + item->text();
#endif
        items.append(imagename);
    }
    return items;
}
