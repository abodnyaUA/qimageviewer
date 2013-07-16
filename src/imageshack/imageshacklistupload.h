#ifndef ImageShackLISTUPLOAD_H
#define ImageShackLISTUPLOAD_H

#include <QWidget>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>
#include "uploadtoimageshack.h"

namespace Ui {
class ImageShackListUpload;
}

class ImageShackListUpload : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageShackListUpload(QWidget *parent = 0);
    void loadlist(QStringList list,QString folder);
    QList< QMap<QString,QString> > getlinkslist();

    ~ImageShackListUpload();

signals:
    void overed(bool);
    void aborted(bool);

private slots:
    void on_cancelButton_clicked();
    void update(QMap<QString,QString>);

private:
    Ui::ImageShackListUpload *ui;
    UploadToImageShack upload;
    QStringList list;
    QString folder;
    QGraphicsScene * scene;
    QPixmap pixmap;
    bool startUpload;
    bool exit;
    QList< QMap<QString,QString> > linkslist;
    QStringList uploadlist;
    int uploadImageAmount,uploadImageCount;
    QMessageBox exitMessageBox;
    QAbstractButton *butYes,*butNo,*butMinimize;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // ImageShackLISTUPLOAD_H
