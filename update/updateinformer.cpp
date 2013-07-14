#include "updateinformer.h"
#include "ui_updateinformer.h"

UpdateInformer::UpdateInformer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpdateInformer)
{
    ui->setupUi(this);
}

void UpdateInformer::setProgress(double procent, QString file)
{
    procent *= 100;
    ui->progressBar->setValue((int)procent);
    ui->fileName->setText(file);
}

UpdateInformer::~UpdateInformer()
{
    delete ui;
}
