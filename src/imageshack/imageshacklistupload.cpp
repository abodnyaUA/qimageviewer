#include "imageshacklistupload.h"
#include "ui_imageshacklistupload.h"

ImageShackListUpload::ImageShackListUpload(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageShackListUpload)
{
    ui->setupUi(this);
    uploadImageCount=0;
    startUpload = false;
    exit = false;
    exitMessageBox.setWindowTitle(tr("Images are uploading"));
    exitMessageBox.setText(tr("Images are uploading now. Do you want to cancel uploading?"));
    butMinimize = exitMessageBox.addButton(tr("Minimize"),QMessageBox::AcceptRole);
    butYes = exitMessageBox.addButton(tr("Yes"),QMessageBox::YesRole);
    butNo = exitMessageBox.addButton(tr("No"),QMessageBox::NoRole);
    exitMessageBox.setIcon(QMessageBox::Question);
}

ImageShackListUpload::~ImageShackListUpload()
{
    delete ui;
}

void ImageShackListUpload::loadlist(QStringList list,QString folder)
{
    uploadlist = list;
    this->folder = folder;
    uploadImageAmount = uploadlist.size();
    uploadImageCount=0;
    startUpload = true;

    linkslist.clear();
    ui->status->setText(tr("Uploading: ")+uploadlist[0].right(uploadlist[0].size() - folder.size()-1));
    ui->progressBar->setValue(0);
    upload(uploadlist[0]);
    connect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(update(QMap<QString,QString>)));
}

void ImageShackListUpload::update(QMap<QString,QString> links)
{
    if (!links["Link"].isEmpty())
    {
        if (uploadImageCount < uploadImageAmount)
        {
            ui->status->setText(tr("Uploading: ")+uploadlist[uploadImageCount].right(uploadlist[uploadImageCount].size() - folder.size()-1));
            ui->progressBar->setValue((int)((double)((uploadImageCount)*100.0)/(double)(uploadImageAmount)));
            linkslist.append(links);
            upload(uploadlist[uploadImageCount]);
            uploadImageCount++;
        }
        else
        {
            disconnect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(update(QMap<QString,QString>)));
            startUpload = false;
            exit = true;
            emit overed(true);
        }
    }
    else
    {
        disconnect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(update(QMap<QString,QString>)));
        linkslist.clear();
        startUpload = false;
        QMessageBox::warning(this,tr("Can't upload image!"),
                             tr("Image can't be uploaded. Server doesn't answer. Please check your internet connection"),
                             QMessageBox::Ok | QMessageBox::Default);
        exit = true;
        emit overed(false);
    }
}

QList< QMap<QString,QString> > ImageShackListUpload::getlinkslist()
{
    return linkslist;
}

void ImageShackListUpload::on_cancelButton_clicked()
{
    if (!startUpload)
    {
        exit = true;
        emit overed(false);
    }
    else
    {
        exitMessageBox.exec();
        if (exitMessageBox.clickedButton() == butYes)
        {
            if (uploadImageCount > 0)
            {
                int r = QMessageBox::question(this,tr("Pictures have been uploaded"),
                                      tr("Do you want to see uploaded images?"),
                                      QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
                if (r == QMessageBox::Yes) emit aborted(true);
                else                       emit aborted(false);
            }
            else
            {
                emit aborted(false);
            }
            disconnect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(update(QMap<QString,QString>)));
            uploadImageCount=0;
            upload.abort();
        }
        else if (exitMessageBox.clickedButton() == butMinimize)
        {
            hide();
        }
    }
}

void ImageShackListUpload::closeEvent(QCloseEvent *event)
{
    if (!startUpload)
    {
        if (!exit)
        {
            emit overed(false);
        }
        event->accept();
    }
    else
    {
        exitMessageBox.exec();
        if (exitMessageBox.clickedButton() == butYes)
        {
            if (uploadImageCount > 0)
            {
                int r = QMessageBox::question(this,tr("Pictures have been uploaded"),
                                      tr("Do you want to see uploaded images?"),
                                      QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
                if (r == QMessageBox::Yes) emit aborted(true);
                else                       emit aborted(false);
            }
            else
            {
                emit aborted(false);
            }
            disconnect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(update(QMap<QString,QString>)));
            uploadImageCount=0;
            upload.abort();
            event->accept();
        }
        else if (exitMessageBox.clickedButton() == butNo)
        {
            event->ignore();
        }
        else if (exitMessageBox.clickedButton() == butMinimize)
        {
            event->accept();
        }
    }
}
