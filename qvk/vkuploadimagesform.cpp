#include "vkuploadimagesform.h"
#include "ui_vkuploadimagesform.h"

vkUploadImagesForm::vkUploadImagesForm(QMap<QString,int> albums,QString addicon) :
    ui(new Ui::vkUploadImagesForm)
{
    ui->setupUi(this);
    this->albums = albums;
    ui->albumUserBox->addItems(albums.keys());
    ui->differentAlbumEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->albumUserBox->setMaximumWidth(300);
    ui->status->setMaximumWidth(635);
    ui->toolButton->setIcon(QIcon(QPixmap(addicon)));
    ui->progressBar->hide();
    ui->status->hide();
    exit = false;
    startUpload = false;

    exitMessageBox.setWindowTitle(tr("Images are uploading"));
    exitMessageBox.setText(tr("Images are uploading now. Do you want to cancel uploading?"));
    butMinimize = exitMessageBox.addButton(tr("Minimize"),QMessageBox::AcceptRole);
    butYes = exitMessageBox.addButton(tr("Yes"),QMessageBox::YesRole);
    butNo = exitMessageBox.addButton(tr("No"),QMessageBox::NoRole);
    exitMessageBox.setIcon(QMessageBox::Question);
}


void vkUploadImagesForm::updatealbums(QMap<QString,int> albums)
{
    ui->albumUserBox->clear();
    ui->albumUserBox->addItems(albums.keys());
    this->albums = albums;
}

QStringList vkUploadImagesForm::getlinkslist()
{
    return uploadlist;
}

void vkUploadImagesForm::loadlist(QStringList list,QString folder, int current)
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

void vkUploadImagesForm::on_listWidget_currentRowChanged(int currentRow)
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

int vkUploadImagesForm::getalbum()
{
    if (ui->differentAlbumEdit->isEnabled()) return ui->differentAlbumEdit->text().toInt();
    return albums[ui->albumUserBox->currentText()];
}

void vkUploadImagesForm::on_userAlbumButton_clicked()
{
    ui->albumUserBox->setEnabled(true);
    ui->differentAlbumEdit->setEnabled(false);
}

void vkUploadImagesForm::on_otherAlbumButton_clicked()
{
    ui->differentAlbumEdit->setEnabled(true);
    ui->albumUserBox->setEnabled(false);
}

void vkUploadImagesForm::on_uploadButton_clicked()
{
    QList<QListWidgetItem *> lst = ui->listWidget->selectedItems();
    uploadlist.clear();
    for (int i=0;i<lst.size();i++)
    {   uploadlist.append(folder+"/"+lst[i]->text());  }

    /** Clear all unselected values **/
    ui->progressBar->show();
    ui->listWidget->setEnabled(false);
    ui->uploadButton->setEnabled(false);
    ui->albumUserBox->setEnabled(false);
    ui->differentAlbumEdit->setEnabled(false);
    ui->otherAlbumButton->setEnabled(false);
    ui->userAlbumButton->setEnabled(false);
    ui->toolButton->setEnabled(false);
    ui->status->show();
    startUpload = true;

    ui->status->setText(tr("Uploading: ")+uploadlist[0]);
    ui->progressBar->setValue(0);

    exit = true;
    emit overed(true);
}

void vkUploadImagesForm::on_cancelButton_clicked()
{
    exit = true;
    emit overed(false);
}

void vkUploadImagesForm::setCurrentUploadedItem(int item)
{
    ui->status->setText(tr("Uploading: ")+uploadlist[item]);
    ui->progressBar->setValue((int)((double)((item)*100.0)/(double)(uploadlist.size())));
    if (item == uploadlist.size()-1)
    {
        startUpload = false;
        exitMessageBox.close();
    }
}

vkUploadImagesForm::~vkUploadImagesForm()
{
    delete ui;
}

void vkUploadImagesForm::on_toolButton_clicked()
{
    QInputDialog dialog;
    QString newfolder = dialog.getText(this,tr("Title of new album"),tr("Please input title of new album"));
    if (!newfolder.isEmpty()) emit addNewAlbum(newfolder);
}

void vkUploadImagesForm::closeEvent(QCloseEvent *event)
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
            emit abort();
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

