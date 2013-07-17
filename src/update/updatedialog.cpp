#include "updatedialog.h"
#include "ui_updatedialog.h"

UpdateDialog::UpdateDialog(QWidget *parent, QString current, QString avaiable, QString changelog) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);
    ui->currentVersionLabel->setText(current);
    ui->avaiableVersionLabel->setText(avaiable);
    ui->textBrowser->setText(changelog.toUtf8());
    int maxscroll = ui->textBrowser->verticalScrollBar()->maximum();
    ui->textBrowser->verticalScrollBar()->setValue(maxscroll);
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}
