#include "editformresize.h"
#include "ui_editformresize.h"
#include <QDebug>
editformResize::editformResize() : ui(new Ui::editformResize)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    ui->cancelButton->setShortcut(QKeySequence(Qt::Key_Escape));
}

/** load image to the window **/
void editformResize::loadImage(QPixmap pixmap_old)
{
    ui->imageView->resize(width()-224,height()-48);
    pixmap = new QPixmap(pixmap_old);

    ui->oldSizeLabel->setText(QString::number(pixmap->width())+" x "+QString::number(pixmap->height()));

    /** zoom out image to window size if need **/
    zoom = qMin((double)ui->imageView->width()/(double)pixmap->width(),
                (double)ui->imageView->height()/(double)pixmap->height());
    if (zoom > 1) zoom = 1;

    ui->widthSpinBox->setValue(pixmap->width());
    ui->heightSpinBox->setValue(pixmap->height());

    im_width = pixmap->width();
    im_height = pixmap->height();

    originalAspectratio = (double)im_width / (double)im_height;
    ui->aspectratioLabel->setText(QString::number(originalAspectratio));
    resizeprewiew();
}

editformResize::~editformResize()
{
    delete ui;
}

/** reload preview **/
void editformResize::resizeprewiew()
{
    scene->clear();
    scene->setSceneRect(0,0,im_width*zoom,im_height*zoom);
    scene->addPixmap(pixmap->scaled(im_width*zoom,im_height*zoom));

    ui->imageView->setSceneRect(0,0,scene->width(),scene->height());
    ui->imageView->setScene(scene);
}

void editformResize::on_widthSpinBox_valueChanged(int arg1)
{
    im_width = arg1;
}

void editformResize::on_heightSpinBox_valueChanged(int arg1)
{
    im_height = arg1;
}

/** if value in spinbox was changed, reload image **/
void editformResize::on_widthSpinBox_editingFinished()
{
    if (ui->aspectratioCheckBox->isChecked())
    {
        im_height = im_width / originalAspectratio;
        ui->heightSpinBox->setValue(im_height);
    }
    else
        ui->aspectratioLabel->setText(QString::number((double)im_width / (double)im_height));
    resizeprewiew();
}

void editformResize::on_heightSpinBox_editingFinished()
{
    if (ui->aspectratioCheckBox->isChecked())
    {
        im_width = im_height * originalAspectratio;
        ui->widthSpinBox->setValue(im_width);
    }
    else
        ui->aspectratioLabel->setText(QString::number((double)im_width / (double)im_height));
    resizeprewiew();
}

/** if "accept button" has clicked, replace old picture and emit exit signal with 'true', what meens 'save changes'**/
void editformResize::on_acceptButton_clicked()
{
    *pixmap = pixmap->scaled(im_width,im_height);
    emit editFinished(true);
}

/** emit exit signal with 'false', what meens 'ignore changes' **/
void editformResize::on_cancelButton_clicked()
{
    emit editFinished(false);
}

/** return pixmap pointer **/
QPixmap * editformResize::getpixmap()
{
    return pixmap;
}

/** lock with with height or not **/
void editformResize::on_aspectratioCheckBox_clicked()
{
    if (ui->aspectratioCheckBox->isChecked())
    {
        originalAspectratio = (double)im_width / (double)im_height;
    }
}
