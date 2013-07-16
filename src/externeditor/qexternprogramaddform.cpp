#include "qexternprogramaddform.h"
#include "ui_qexternprogramaddform.h"

QExternProgramAddForm::QExternProgramAddForm(QList<QExternProgram *> installedSoft) :
    ui(new Ui::QExternProgramAddForm)
{
    ui->setupUi(this);
    /**** list ****/
    foreach (QExternProgram * app, installedSoft)
    {
        QListWidgetItem * item = new QListWidgetItem(app->icon,app->name);
        item->setWhatsThis(app->command);
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow(0);
}

QExternProgramAddForm::~QExternProgramAddForm()
{
    delete ui;
}

void QExternProgramAddForm::on_iconBrowseButton_clicked()
{
    QString path;
    QFileDialog dialog;
    path = dialog.getOpenFileName(this,tr("Opening extern image application"),
                                   "",tr("All image formats (*.jpg *.jpeg *.png *.bmp *.gif *.tiff *.pbm *.pgm *.ppm *.xbm *.xpm)"));
    if(!path.isNull())
    {
        ui->iconLineEdit->setText(path);
        ui->iconLabel->setPixmap(QPixmap(path).scaled(32,32,Qt::KeepAspectRatioByExpanding));
    }
}

void QExternProgramAddForm::on_commandBrowseButton_clicked()
{
    QString path;
    QFileDialog dialog;
    path = dialog.getOpenFileName(this,tr("Opening extern image application"),
                                   "",tr("All file formats (*.*)"));
    if(!path.isNull()) ui->commandLineEdit->setText(path);
}

void QExternProgramAddForm::closeEvent(QCloseEvent *event)
{
    emit cancel();
    event->accept();
}

void QExternProgramAddForm::on_cancelButton_clicked()
{
    close();
}

void QExternProgramAddForm::on_addButton_clicked()
{
    // using command //
    if (ui->tabWidget->currentIndex() == 1)
    {
        if (ui->nameLineEdit->text().isEmpty())
        {
            QMessageBox::warning(this, tr("Invalid parameters"),
                                 tr("Name can't be empty"),
                                        QMessageBox::Ok | QMessageBox::Default);
            return;
        }
        if (ui->commandLineEdit->text().isEmpty())
        {
            QMessageBox::warning(this, tr("Invalid parameters"),
                                 tr("Application path (command) can't be empty"),
                                        QMessageBox::Ok | QMessageBox::Default);
            return;
        }
        name = ui->nameLineEdit->text();
        icon = QIcon(QPixmap(ui->iconLineEdit->text()));
        command = ui->commandLineEdit->text();
    }
    // using application from list //
    else
    {
        name = ui->listWidget->currentItem()->text();
        icon = ui->listWidget->currentItem()->icon();
        command = ui->listWidget->currentItem()->whatsThis();
    }
    emit accept(name,icon,command);
}
