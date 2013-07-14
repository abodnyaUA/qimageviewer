#include "imageshacklistview.h"
#include "ui_imageshacklistview.h"

ImageShackListView::ImageShackListView(QList< QMap<QString,QString> > lst) :
    ui(new Ui::ImageShackListView)
{
    ui->setupUi(this);
    for (int i=0;i<lst.size();i++)
    {
        ui->linkPlainTextEdit->appendPlainText(lst[i]["Link"]);
        ui->fullDirectPlainTextEdit->appendPlainText(lst[i]["ImageLink"]);
        ui->fullForumsPlainTextEdit->appendPlainText(lst[i]["ImageBB"]);
        ui->fullSitesPlainTextEdit->appendPlainText(lst[i]["ImageHtml"]);
        ui->thumbForumsPlainTextEdit->appendPlainText(lst[i]["ThumbBB"]);
        ui->thumbsSitesPlainTextEdit->appendPlainText(lst[i]["ThumbLink"]);
    }
}

ImageShackListView::~ImageShackListView()
{
    delete ui;
}

void ImageShackListView::on_pushButton_clicked()
{
    close();
}
