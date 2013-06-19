#include "aboutform.h"
#include "ui_aboutform.h"

aboutForm::aboutForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::aboutForm)
{
    ui->setupUi(this);
    ui->buildDateLabel->setText(tr("build: ")+QDate::currentDate().toString(Qt::ISODate));
    ui->versionLabel->setText(tr("version: ")+QApplication::applicationVersion());
    ui->qtversionLabel->setText("Qt "+QString(qVersion()));
    ui->compillerLabel->setText("GCC "+QString::number(__GNUC__)+"."+QString::number(__GNUC_MINOR__)+"."+QString::number(__GNUC_PATCHLEVEL__));
}

aboutForm::~aboutForm()
{
    delete ui;
}

void aboutForm::on_pushButton_clicked()
{
    exit = true;
    emit overed();
}

void aboutForm::closeEvent(QCloseEvent *event)
{
    if (!exit) emit overed();
    event->accept();
}

void aboutForm::on_label_7_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link,QUrl::TolerantMode));
}

void aboutForm::on_label_6_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("mailto:"+link,QUrl::TolerantMode));
}
