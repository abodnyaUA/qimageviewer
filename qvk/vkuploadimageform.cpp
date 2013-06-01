#include "vkuploadimageform.h"
#include "ui_vkuploadimageform.h"

vkUploadImageForm::vkUploadImageForm(QMap<QString,int> albums,QString addicon) :
    ui(new Ui::vkUploadImageForm)
{
    ui->setupUi(this);
    this->albums = albums;
    ui->comboBox->addItems(albums.keys());
    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->comboBox->setMaximumWidth(300);
    ui->toolButton->setIcon(QIcon(QPixmap(addicon)));
    setMaximumWidth(320);
    setMaximumHeight(sizePolicy().verticalStretch());
    exit = false;
}

void vkUploadImageForm::updatealbums(QMap<QString,int> albums)
{
    ui->comboBox->clear();
    ui->comboBox->addItems(albums.keys());
    this->albums = albums;
}

vkUploadImageForm::~vkUploadImageForm()
{
    delete ui;
}

int vkUploadImageForm::getalbum()
{
    if (ui->lineEdit->isEnabled()) return ui->lineEdit->text().toInt();
    return albums[ui->comboBox->currentText()];
}

void vkUploadImageForm::on_userAlbumButton_clicked()
{
    ui->comboBox->setEnabled(true);
    ui->lineEdit->setEnabled(false);
}

void vkUploadImageForm::on_otherAlbumButton_clicked()
{
    ui->lineEdit->setEnabled(true);
    ui->comboBox->setEnabled(false);
}

void vkUploadImageForm::on_uploadButton_clicked()
{
    exit = true;
    emit overed(true);
}

void vkUploadImageForm::on_cancelButton_clicked()
{
    exit = true;
    emit overed(false);
}

void vkUploadImageForm::closeEvent(QCloseEvent *event)
{
    if (!exit)
    {
        emit overed(false);
    }
    event->accept();
}

void vkUploadImageForm::on_toolButton_clicked()
{
    QInputDialog dialog;
    QString newfolder = dialog.getText(this,tr("Title of new album"),tr("Please input title of new album"));
    if (!newfolder.isEmpty()) emit addNewAlbum(newfolder);
}
