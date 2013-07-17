#include "editformresizeelements.h"
#include "ui_editformresizeelements.h"

editformResizeElements::editformResizeElements() :
    ui(new Ui::editformResizeElements)
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

editformResizeElements::~editformResizeElements()
{
    delete ui;
}

void editformResizeElements::loadlist(QStringList list,QString folder)
{
    int size = folder.size()+1;
    for (int i=0;i<list.size();i++)
        ui->listWidget->addItem(list[i].right(list[i].size()-size));

    this->list = list;
    this->folder = folder;

    scene = new QGraphicsScene;
    pixmap.load(list[0]);
    scene->addPixmap(pixmap.scaled(ui->graphicsView->width(),ui->graphicsView->height(),Qt::KeepAspectRatioByExpanding));
    ui->graphicsView->setScene(scene);
    ui->previewWidthLabel->setText(QString::number(pixmap.width()));
    ui->previewHeightLabel->setText(QString::number(pixmap.height()));

    ui->listWidget->setCurrentRow(0);
    ui->listWidget->selectAll();
}

void editformResizeElements::on_typeSizeRadioButton_clicked()
{
    ui->sizeHeightSpinBox->setEnabled(true);
    ui->sizeWidthSpinBox->setEnabled(true);
    ui->sizePercentSpinBox->setDisabled(true);
}

void editformResizeElements::on_typePercentRadioButton_clicked()
{
    ui->sizePercentSpinBox->setEnabled(true);
    ui->sizeHeightSpinBox->setDisabled(true);
    ui->sizeWidthSpinBox->setDisabled(true);
}

QString editformResizeElements::getLast()
{
    return last_element;
}

void editformResizeElements::on_acceptButton_clicked()
{
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setFileMode(QFileDialog::Directory);
    QString prefix = dialog.getExistingDirectory(this,tr("Select new folder to save changes"),
                                               folder,
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
    if( !prefix.isNull() )
    {
        if (prefix == folder)
        {
            int r = QMessageBox::question(this,tr("Files are exist"), tr("Do you want to replace it?"),
                                  QMessageBox::Yes,QMessageBox::No);
            if (r == QMessageBox::No)
            {
#ifdef Q_OS_WIN32
                prefix+="\\edit-";
#else
                prefix+="/edit-";
#endif
                samefolder = true;
            }
        }
        else
        {
#ifdef Q_OS_WIN32
            prefix+="\\";
#else
            prefix+="/";
            samefolder = false;
#endif
        }
        int size = folder.size()+1;

        last_element = QString::null;
        for (int i=0;i<list.size() && last_element == QString::null;i++)
        {
            if (ui->listWidget->isItemSelected(ui->listWidget->item(i)))
                last_element = folder+"/"+ui->listWidget->item(i)->text();
        }

        if (ui->typeSizeRadioButton->isChecked())
        {
            for (int i=0;i<list.size();i++)
            {
                if (ui->listWidget->isItemSelected(ui->listWidget->item(i)))
                {
                    QPixmap temp(list[i]);
                    temp = temp.scaled(ui->sizeWidthSpinBox->value(),ui->sizeHeightSpinBox->value(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                    temp.save(prefix+list[i].right(list[i].size()-size));
                    qDebug() << "current="<<list[i] << "w="<<temp.width()<<"h="<<temp.height();
                }
            }
        }

        if (ui->typePercentRadioButton->isChecked())
        {
            for (int i=0;i<list.size();i++)
            {
                if (ui->listWidget->isItemSelected(ui->listWidget->item(i)))
                {
                    QPixmap temp(list[i]);
                    temp = temp.scaledToWidth(temp.width()*ui->sizePercentSpinBox->value()/100.0,Qt::SmoothTransformation);
                    temp.save(prefix+list[i].right(list[i].size()-size));
                }
            }
        }
        emit editFinished(true);
    }
}

void editformResizeElements::on_cancelButton_clicked()
{
    emit editFinished(false);
}

bool editformResizeElements::isSameFolder()
{
    return samefolder;
}

void editformResizeElements::on_listWidget_currentRowChanged(int currentRow)
{
    delete scene;
    scene = new QGraphicsScene;
    pixmap.load(list[currentRow]);
    if ((double)pixmap.width()/(double)ui->graphicsView->width() >
            (double)pixmap.height()/(double)ui->graphicsView->height())
        scene->addPixmap(pixmap.scaledToWidth(ui->graphicsView->width()-5, Qt::SmoothTransformation));
    else
        scene->addPixmap(pixmap.scaledToHeight(ui->graphicsView->height()-5, Qt::SmoothTransformation));
    ui->graphicsView->setScene(scene);
    ui->previewWidthLabel->setText(QString::number(pixmap.width()));
    ui->previewHeightLabel->setText(QString::number(pixmap.height()));
}
