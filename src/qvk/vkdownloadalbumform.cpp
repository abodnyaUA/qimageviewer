#include "vkdownloadalbumform.h"
#include "ui_vkdownloadalbumform.h"

vkDownloadAlbumForm::vkDownloadAlbumForm(QMap<QString,int> albums,int userId, QString defaultfolder) :
    ui(new Ui::vkDownloadAlbumForm)
{
    ui->setupUi(this);
    lastSlashPos = -1;
    this->albums = albums;
    ui->comboBox->addItems(albums.keys());
    this->userId = userId;
    ui->folderDestinationEdit->setText(defaultfolder+'/');
    ui->progressBar->hide();
    setMaximumWidth(320);
    setMaximumHeight(sizePolicy().verticalStretch()-25);
    exit = false;
    exitMessageBox.setWindowTitle(tr("Images are uploading"));
    exitMessageBox.setText(tr("Images are uploading now. Do you want to cancel uploading?"));
    butMinimize = exitMessageBox.addButton(tr("Minimize"),QMessageBox::AcceptRole);
    butYes = exitMessageBox.addButton(tr("Yes"),QMessageBox::YesRole);
    butNo = exitMessageBox.addButton(tr("No"),QMessageBox::NoRole);
    exitMessageBox.setIcon(QMessageBox::Question);
    startDownload = false;
}

vkDownloadAlbumForm::~vkDownloadAlbumForm()
{
    delete ui;
}

QMap<QString,QVariant> vkDownloadAlbumForm::getDownloadData()
{
    QMap<QString,QVariant> album;
    if (ui->AnotherAlbumRadioButton->isChecked())
    {
        QString url = ui->lineEdit->text().right(ui->lineEdit->text().size()-lastSlashPos-6);
        for (int i=0;i<url.size();i++)
        {
            if (url[i] == '_')
            {
                album["user"] = url.left(i).toInt();
                album["album"] = url.right(url.size()-i-1).toInt();
                break;
            }
        }
    }
    else
    {
        album["user"] = userId;
        album["album"] = albums[ui->comboBox->currentText()];
    }
    album["folder"] = ui->folderDestinationEdit->text();
    return album;
}

void vkDownloadAlbumForm::on_acceptButton_clicked()
{
    if (ui->AnotherAlbumRadioButton->isChecked())
    {
        QString url = ui->lineEdit->text();
        lastSlashPos = -1;
        for (int i=0;i<url.size();i++)
            if (url[i] == '/' && i!= url.size()-1) lastSlashPos = i;
        if (lastSlashPos == -1)
        {
            QMessageBox::warning(this,tr("Invalid link"),
                                 tr("Your album's link is invalid. Please check it or select your album"),
                                 QMessageBox::Ok | QMessageBox::Default);
            return;
        }
    }
    if (ui->folderDestinationEdit->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Invalid destination"),
                             tr("Destination folder is empty. Please check it"),
                             QMessageBox::Ok | QMessageBox::Default);
        return;
    }

    ui->progressBar->show();
    ui->progressBar->setValue(0);
    setMaximumHeight(sizePolicy().verticalStretch());
    ui->acceptButton->setEnabled(false);
    ui->AnotherAlbumRadioButton->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->UserAlbumRadioButton->setEnabled(false);
    ui->browseButton->setEnabled(false);
    startDownload = true;
    exit = true;
    emit overed(true);
}

void vkDownloadAlbumForm::setCurrentDownloadedProcent(int procent)
{
    ui->progressBar->setValue(procent);
    if (procent == 100)
    {
        exitMessageBox.close();
    }
}

void vkDownloadAlbumForm::on_cancelButton_clicked()
{
    if (!startDownload)
    {
        exit = true;
        emit overed(false);
    }
    else
    {
        ui->progressBar->hide();
        ui->acceptButton->setEnabled(true);
        ui->AnotherAlbumRadioButton->setEnabled(true);
        ui->comboBox->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        ui->UserAlbumRadioButton->setEnabled(true);
        ui->browseButton->setEnabled(true);
        startDownload= false;
        emit abort(false);
    }
}

void vkDownloadAlbumForm::closeEvent(QCloseEvent *event)
{
    if (!startDownload)
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

void vkDownloadAlbumForm::on_AnotherAlbumRadioButton_clicked()
{
    ui->comboBox->setEnabled(false);
    ui->lineEdit->setEnabled(true);
}

void vkDownloadAlbumForm::on_UserAlbumRadioButton_clicked()
{
    ui->comboBox->setEnabled(true);
    ui->lineEdit->setEnabled(false);
}

void vkDownloadAlbumForm::on_browseButton_clicked()
{
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setFileMode(QFileDialog::Directory);
    QString path = dialog.getExistingDirectory(this,tr("Open directory"),
                                               ui->folderDestinationEdit->text(),
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
    if( !path.isNull() )
        ui->folderDestinationEdit->setText(path+'/');
}
