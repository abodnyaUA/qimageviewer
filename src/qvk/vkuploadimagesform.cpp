#include "vkuploadimagesform.h"
#include "ui_vkuploadimagesform.h"

vkUploadImagesForm::vkUploadImagesForm(QMap<QString,int> albums,QString addicon) :
    ui(new Ui::vkUploadImagesForm)
{
    ui->setupUi(this);
    this->albums = albums;
    ui->albumUserBox->addItems(albums.keys());
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

void vkUploadImagesForm::loadlist(QStringList list,QString folder)
{
    uploadlist = list;
    this->folder = folder;
}

int vkUploadImagesForm::getalbum()
{
    return albums[ui->albumUserBox->currentText()];
}

void vkUploadImagesForm::on_uploadButton_clicked()
{

    /** Clear all unselected values **/
    ui->progressBar->show();
    ui->uploadButton->setEnabled(false);
    ui->albumUserBox->setEnabled(false);
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
    if (!startUpload)
    {
        exit = true;
        emit overed(false);
    }
    else
    {
        ui->progressBar->hide();
        ui->uploadButton->setEnabled(true);
        startUpload = false;
        emit abort(false);
        QMessageBox::warning(this,tr("Pictures have been uploaded"),
                                          tr("Some pictures have been uploaded"),
                                          QMessageBox::Ok);
    }
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
            emit abort(true);
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

