#ifndef ImageShackLISTUPLOAD_H
#define ImageShackLISTUPLOAD_H

#include <QWidget>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>
#include "imageshackuploader.h"

namespace Ui {
class ImageShackListUpload;
}

class ImageShackListUpload : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageShackListUpload(QWidget *parent = 0);
    void loadlist(QStringList list,QString folder, int current);
    QList< QMap<QString,QString> > getlinkslist();

    ~ImageShackListUpload();

signals:
    void editFinished(bool);

private slots:
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::ImageShackListUpload *ui;
    QStringList list;
    QString folder;
    QGraphicsScene * scene;
    QPixmap pixmap;
    bool startUpload;
    bool exit;
    QList< QMap<QString,QString> > linkslist;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // ImageShackLISTUPLOAD_H
