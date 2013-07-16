#ifndef IMAGEHACKUPLOADER_H
#define IMAGEHACKUPLOADER_H

#include <QWidget>
#include <QCloseEvent>
#include "uploadtoimageshack.h"
namespace Ui {
class ImageShackUploader;
}

class ImageShackUploader : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageShackUploader(QString imagename);
    ~ImageShackUploader();
    
private slots:
    void on_pushButton_clicked();
    void display(QMap<QString,QString>);

private:
    Ui::ImageShackUploader *ui;
    UploadToImageShack upload;

signals:
    void overed();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // IMAGEHACKUPLOADER_H
