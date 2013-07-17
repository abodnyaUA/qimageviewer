#ifndef PREVIEWLIST_H
#define PREVIEWLIST_H

#include <QObject>
#include <QHBoxLayout>
#include <QThread>
#include <QListWidget>
#include <QDebug>
#include <QImage>

class previewList : public QObject
{
    Q_OBJECT
public:
    previewList(QListWidget * previewListWidget, QThread *thread);
    void loadList(QStringList list);
    QList<QImage> icons;
signals:
    void finished();
public slots:
    void run();
private:
    int current;
    QThread *thread;
    QStringList lst;
    QHBoxLayout layout;
    QListWidget * listWidget;
};

#endif // PREVIEWLIST_H
