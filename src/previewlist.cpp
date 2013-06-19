#include "previewlist.h"

previewList::previewList(QListWidget * previewListWidget)
{
    listWidget = previewListWidget;
}
void previewList::loadList(QStringList list)
{
    lst.clear();
    foreach(QString item, list)
        lst.append(item);
}

void previewList::run()
{
    foreach (QString image, lst)
    {
        QListWidgetItem * item = new QListWidgetItem(QIcon(QPixmap(image)),"");
        qDebug() << "item width="<<item->icon().actualSize(QPixmap(image).size()).width()
                    <<item->icon().actualSize(QPixmap(image).size()).height();
        listWidget->addItem(item);
    }
    emit finished();
}
