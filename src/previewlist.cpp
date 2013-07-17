#include "previewlist.h"

previewList::previewList(QListWidget * previewListWidget, QThread *thread)
{
    listWidget = previewListWidget;
    this->thread = thread;
}
void previewList::loadList(QStringList list)
{
    lst.clear();
    icons.clear();
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
            icons.append(QImage(lst[current]).scaled(100,100));
            current++;
            thread->start();
        }
    }
    if (thread->isRunning()) emit finished();
}
