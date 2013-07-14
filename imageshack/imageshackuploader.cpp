#include "imageshackuploader.h"
#include "ui_imageshackuploader.h"

ImageShackUploader::ImageShackUploader(QString imagename)
{
    upload(imagename);
    ui = new Ui::ImageShackUploader;
    ui->setupUi(this);
    QPixmap * preview = new QPixmap(imagename);
    if (preview->width() > preview->height())
        ui->previewLabel->setPixmap(preview->scaledToWidth(150));
    else
        ui->previewLabel->setPixmap(preview->scaledToHeight(150));

    hide();
    connect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(display(QMap<QString,QString>)));
}

void ImageShackUploader::display(QMap<QString,QString> links)
{
    if (!links["Link"].isEmpty())
    {
        ui->linkLineEdit->setText(links["Link"]);
        ui->fullForumsLineEdit->setText(links["ImageBB"]);
        ui->fullSitesLineEdit->setText(links["ImageHtml"]);
        ui->fullDirectLineEdit->setText(links["ImageLink"]);
        ui->thumbForumsLineEdit->setText(links["ThumbBB"]);
        ui->thumbsSitesLineEdit->setText(links["ThumbLink"]);
        show();
    }
    else
    {
        QMessageBox::warning(this,tr("Can't upload image!"),
                             tr("Image can't be uploaded. Server doesn't answer. Please check your internet connection"),
                             QMessageBox::Ok | QMessageBox::Default);
    }
}

ImageShackUploader::~ImageShackUploader()
{
    delete ui;
}

void ImageShackUploader::on_pushButton_clicked()
{
    close();
}

void ImageShackUploader::closeEvent(QCloseEvent *event)
{
    emit overed();
    event->accept();
}
