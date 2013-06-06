#include "imageshacklistupload.h"
#include "ui_imageshacklistupload.h"

ImageShackListUpload::ImageShackListUpload(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageShackListUpload)
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->progressBar->hide();
    ui->status->hide();
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

void ImageShackListUpload::loadlist(QStringList list,QString folder, int current)
{
    ui->listWidget->clear();
    int size = folder.size()+1;
    for (int i=0;i<list.size();i++)
        ui->listWidget->addItem(list[i].right(list[i].size()-size));

    this->list = list;
    this->folder = folder;

    scene = new QGraphicsScene;
    pixmap.load(list[0]);
    scene->addPixmap(pixmap.scaled(ui->graphicsView->width(),ui->graphicsView->height(),Qt::KeepAspectRatioByExpanding));
    ui->graphicsView->setScene(scene);

    ui->listWidget->setCurrentRow(current);
}


void ImageShackListUpload::on_acceptButton_clicked()
{
    ui->progressBar->show();
    uploadlist.clear();
    QList<QListWidgetItem *> lst = ui->listWidget->selectedItems();
    for (int i=0;i<lst.size();i++)
    {   uploadlist.append(lst[i]->text());  }
    uploadImageAmount = uploadlist.size();
    uploadImageCount=0;

    /** Clear all unselected values **/
    ui->listWidget->setEnabled(false);
    ui->acceptButton->setEnabled(false);
    ui->status->show();
    startUpload = true;

    linkslist.clear();
    ui->status->setText(tr("Uploading: ")+uploadlist[0]);
    ui->progressBar->setValue(0);
    upload(folder+"/"+uploadlist[0]);
    connect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(update(QMap<QString,QString>)));
}
void ImageShackListUpload::update(QMap<QString,QString> links)
{
    if (!links["Link"].isEmpty())
    {
        if (uploadImageCount < uploadImageAmount)
        {
            ui->status->setText(tr("Uploading: ")+uploadlist[uploadImageCount]);
            ui->progressBar->setValue((int)((double)((uploadImageCount)*100.0)/(double)(uploadImageAmount)));
            linkslist.append(links);
            upload(folder+"/"+uploadlist[uploadImageCount]);
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
        ui->progressBar->hide();
        ui->acceptButton->setEnabled(true);
        ui->listWidget->setEnabled(true);
        linkslist.clear();
        startUpload = false;
        QMessageBox::warning(this,tr("Can't upload image!"),
                             tr("Image can't be uploaded. Server doesn't answer. Please check your internet connection"),
                             QMessageBox::Ok | QMessageBox::Default);
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
        if (uploadImageCount > 0)
        {
            int r = QMessageBox::question(this,tr("Pictures have been uploaded"),
                                  tr("Do you want to see uploaded images?"),
                                  QMessageBox::Yes, QMessageBox::No | QMessageBox::Default,
                                          QMessageBox::Cancel);
            if (r == QMessageBox::Cancel) return;
            if (r == QMessageBox::Yes) emit aborted(true);
            else                       emit aborted(false);
        }
        else
        {
            emit aborted(false);
        }
        disconnect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(update(QMap<QString,QString>)));
        ui->progressBar->hide();
        ui->acceptButton->setEnabled(true);
        ui->listWidget->setEnabled(true);
        startUpload = false;
        uploadImageCount=0;
        upload.abort();
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
            ui->progressBar->hide();
            ui->acceptButton->setEnabled(true);
            ui->listWidget->setEnabled(true);
            startUpload = false;
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
            disconnect(&upload,SIGNAL(linkReady(QMap<QString,QString>)),this,SLOT(update(QMap<QString,QString>)));
            ui->progressBar->hide();
            ui->acceptButton->setEnabled(true);
            ui->listWidget->setEnabled(true);
            startUpload = false;
            uploadImageCount=0;
            upload.abort();
            event->accept();
        }
    }
}

void ImageShackListUpload::on_listWidget_currentRowChanged(int currentRow)
{
    delete scene;
    scene = new QGraphicsScene;
    pixmap.load(list[currentRow]);
    if ((double)pixmap.width()/(double)ui->graphicsView->width() >
            (double)pixmap.height()/(double)ui->graphicsView->height())
        scene->addPixmap(pixmap.scaledToWidth(ui->graphicsView->width()-5));
    else
        scene->addPixmap(pixmap.scaledToHeight(ui->graphicsView->height()-5));
    ui->graphicsView->setScene(scene);
}
