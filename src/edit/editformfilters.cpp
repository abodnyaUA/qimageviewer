#include "editformfilters.h"
#include "ui_editformfilters.h"

editformFilters::editformFilters(int width, int height, Filter::FilterType filter) : ui(new Ui::editformFilters)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    ui->cancelButton->setShortcut(QKeySequence(Qt::Key_Escape));
    wd = width;
    hg = height;
    this->filter = filter;
    if (filter == Filter::Brightness || filter == Filter::Temperature)
    {
        ui->argumentHorizontalSlider->setRange(-255,255);
        ui->argumentSpinBox->setRange(-255,255);
    }
    else
    {
        ui->argumentHorizontalSlider->setRange(0,255);
        ui->argumentSpinBox->setRange(0,255);
    }
}

/** load image to the window **/
void editformFilters::loadImage(QPixmap pixmap_old)
{
    ui->imageView->resize(wd-20,hg-90);
    pixmap = new QPixmap(pixmap_old);

    /** zoom out image to window size if need **/
    zoom = qMin((double)ui->imageView->width()/(double)pixmap->width(),
                (double)ui->imageView->height()/(double)pixmap->height());
    if (zoom > 1) zoom = 1;

    im_width = pixmap->width();
    im_height = pixmap->height();

    ui->argumentSpinBox->setValue(0);
    connect(ui->argumentHorizontalSlider,SIGNAL(valueChanged(int)),ui->argumentSpinBox,SLOT(setValue(int)));
    connect(ui->argumentSpinBox,SIGNAL(valueChanged(int)),ui->argumentHorizontalSlider,SLOT(setValue(int)));
    connect(ui->argumentSpinBox,SIGNAL(valueChanged(int)),this,SLOT(filtersPreview()));

    filtersPreview();
}

editformFilters::~editformFilters()
{
    delete ui;
}

/** reload preview **/
void editformFilters::filtersPreview()
{
    scene->clear();
    scene->setSceneRect(0,0,im_width*zoom,im_height*zoom);
    scene->addPixmap(Filter(pixmap->scaled(im_width*zoom,im_height*zoom),filter,ui->argumentSpinBox->value()));

    ui->imageView->setSceneRect(0,0,scene->width(),scene->height());
    ui->imageView->setScene(scene);
}

/** if "accept button" has clicked, replace old picture and emit exit signal with 'true', what meens 'save changes'**/
void editformFilters::on_acceptButton_clicked()
{
    *pixmap = Filter(*pixmap,filter,ui->argumentSpinBox->value());
    emit editFinished(true);
}

/** emit exit signal with 'false', what meens 'ignore changes' **/
void editformFilters::on_cancelButton_clicked()
{
    emit editFinished(false);
}

/** return pixmap pointer **/
QPixmap * editformFilters::getpixmap()
{
    return pixmap;
}
