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
    startUpload = false;
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
    QStringList uploadlist;
    QList<QListWidgetItem *> lst = ui->listWidget->selectedItems();
    for (int i=0;i<lst.size();i++)
    {   uploadlist.append(lst[i]->text());  }
    int uploadImageAmount = uploadlist.size();

    /** Clear all unselected values **/
    ui->listWidget->setEnabled(false);
    ui->cancelButton->hide();
    ui->acceptButton->hide();
    ui->status->show();
    startUpload = true;

    QList<ImageShackUploader *> uploaderList;

    ui->status->setText(tr("Uploading: ")+uploadlist[0]);
    ImageShackUploader * uploader = new ImageShackUploader(folder+"/"+uploadlist[0],false);
    linkslist.append(uploader->getlinks());
    if (!linkslist[0]["Link"].isEmpty())
    {
        uploaderList.append(uploader);
        ui->progressBar->setValue((int)(100.0/(double)(uploadImageAmount)));

        for (int i=1;i<uploadImageAmount;i++)
        {
            ui->status->setText(tr("Uploading: ")+uploadlist[i]);
            ImageShackUploader * uploader = new ImageShackUploader(folder+"/"+uploadlist[i],false);
            linkslist.append(uploader->getlinks());
            uploaderList.append(uploader);
            ui->progressBar->setValue((int)((double)((i+1)*100.0)/(double)(uploadImageAmount)));
        }
        startUpload = false;
        emit editFinished(true);
    }
    else
    {
        ui->status->hide();
        ui->progressBar->hide();
        ui->acceptButton->show();
        ui->cancelButton->show();
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
    emit editFinished(false);
}

void ImageShackListUpload::closeEvent(QCloseEvent *event)
{
    if (startUpload)
    {
        QMessageBox::warning(this,tr("Can't cancel uploading!"),
                           tr("Please wait while all images will be uploaded"),
                           QMessageBox::Ok | QMessageBox::Default);
        event->ignore();
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
