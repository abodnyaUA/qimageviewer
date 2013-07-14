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
    for (int i=0;i<lst.size();i++)
    {
        listWidget->item(i)->setIcon(QIcon(QPixmap(lst[i]).scaled(100,100)));
    }
    emit finished();
}
