#include "editformcrop.h"
#include "ui_editformcrop.h"
#include <QDebug>
editformCrop::editformCrop() : ui(new Ui::editformCrop)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    ui->cancelButton->setShortcut(QKeySequence(Qt::Key_Escape));
    left = 0; right = 0; top = 0; down = 0;
    isStartedCrop = false;
    imagepreview = new editformcropimage;
    ui->imageLayout->addWidget(imagepreview);
    connect(imagepreview,SIGNAL(lefttopWasChanged(int,int)),this,SLOT(setnewLeftTop(int,int)));
    connect(imagepreview,SIGNAL(rightdownWasChanged(int,int)),this,SLOT(setnewRightDown(int,int)));
}

void editformCrop::loadImage(QPixmap pixmap_old)
{
    imagepreview->resize(width()-224,height()-48);
    pixmap = new QPixmap(pixmap_old);

    ui->oldSizeLabel->setText(QString::number(pixmap->width())+" x "+QString::number(pixmap->height()));

    zoom = qMin((double)imagepreview->width()/(double)pixmap->width(),
                (double)imagepreview->height()/(double)pixmap->height());
    if (zoom > 1) zoom = 1;

    ui->widthSpinBox->setValue(0);
    ui->heightSpinBox->setValue(0);

    im_width = pixmap->width();
    im_height = pixmap->height();
    qDebug() << "imwidWD =" << imagepreview->width() <<
                "imwidHG =" << imagepreview->height() <<
                "imageWD =" << im_width*zoom <<
                "imageHG =" << im_height*zoom <<
                "zoom = " << zoom <<
                "imaorWD =" << im_width <<
                "imaorHG =" << im_height;

    originalAspectratio = (double)im_width / (double)im_height;
    ui->aspectratioLabel->setText(QString::number(0));
    resizeprewiew();
}

editformCrop::~editformCrop()
{
    delete ui;
}

void editformCrop::resizeprewiew()
{
    scene->clear();
    scene->setSceneRect(0,0,im_width*zoom,im_height*zoom);
    scene->addPixmap(pixmap->scaled(im_width*zoom,im_height*zoom));

    imagepreview->setSceneRect(0,0,scene->width(),scene->height());
    imagepreview->setScene(scene);
    imagepreview->setindets();
}

void editformCrop::on_widthSpinBox_valueChanged(int arg1)
{
    if (isStartedCrop)
    {
        if (im_width-left >= arg1)
            right = left+arg1;
        else
        {
            right = im_width;
            ui->widthSpinBox->setValue(im_width-left);
        }
    }
    else
    {
        right = arg1;
        isStartedCrop = true;
    }
    imagepreview->changeFrame(right,down);
}

void editformCrop::on_heightSpinBox_valueChanged(int arg1)
{
    if (isStartedCrop)
    {
        if (im_height-top >= arg1)
            down = top+arg1;
        else
        {
            down = im_height;
            ui->heightSpinBox->setValue(im_height-top);
        }
    }
    else
    {
        down = arg1;
        isStartedCrop = true;
    }
    imagepreview->changeFrame(right,down);
}

void editformCrop::on_widthSpinBox_editingFinished()
{
    ui->aspectratioLabel->setText(QString::number((double)(right-left) / (double)(down-top)));
    resizeprewiew();
}

void editformCrop::on_heightSpinBox_editingFinished()
{
    ui->aspectratioLabel->setText(QString::number((double)(right-left) / (double)(down-top)));
    resizeprewiew();
}

void editformCrop::setnewLeftTop(int left, int top)
{
    this->left = left;
    this->top = top;
    this->right = left+1;
    this->down = top+1;
    ui->widthSpinBox->setValue(1);
    ui->heightSpinBox->setValue(1);
    resizeprewiew();
    ui->aspectratioLabel->setText(QString::number((double)(right-left) / (double)(down-top)));
}

void editformCrop::setnewRightDown(int right, int down)
{
    this->right = right;
    this->down = down;
    ui->widthSpinBox->setValue(right-left);
    ui->heightSpinBox->setValue(down-top);
    resizeprewiew();
    ui->aspectratioLabel->setText(QString::number((double)(right-left) / (double)(down-top)));
}

void editformCrop::on_acceptButton_clicked()
{
    *pixmap = pixmap->copy(left/zoom,top/zoom,(right-left)/zoom,(down-top)/zoom);
    emit editFinished(true);
}

void editformCrop::on_cancelButton_clicked()
{
    emit editFinished(false);
}

QPixmap * editformCrop::getpixmap()
{
    return pixmap;
}

