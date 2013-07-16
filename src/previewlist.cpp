#include "previewlist.h"

previewList::previewList(QListWidget * previewListWidget, QThread *thread)
{
    listWidget = previewListWidget;
    this->thread = thread;
}
void previewList::loadList(QStringList list)
{
    lst.clear();
    foreach(QString item, list)
        lst.append(item);
    current = 0;
}

void previewList::run()
{
    while (thread->isRunning() && current!=lst.size())
    {
        if (listWidget->count() > current)
        {
            qDebug() << "current = "<<current<<"; size = "<<listWidget->count();
            listWidget->item(current)->setIcon(QIcon(QPixmap(lst[current]).scaled(100,100)));
            current++;
            thread->start();
        }
    }
    if (thread->isRunning()) emit finished();
}
