#include "qexternprogrammanager.h"
#include "ui_qexternprogrammanager.h"

QExternProgramManager::QExternProgramManager(QList<QExternProgram *> * installedSoft,
                                             QList<QExternProgram *> editors, image * imagewidget,
                                             QString theme, QMap<QString, QString> icon) :
    ui(new Ui::QExternProgramManager)
{
    ui->setupUi(this);
    this->editors = editors;
    this->imagewidget = imagewidget;
    this->installedSoft = installedSoft;
    if (!editors.isEmpty())
    {
        for (int i=0;i<this->editors.size();i++)
        {
            QListWidgetItem * newitem = new QListWidgetItem(editors[i]->icon,editors[i]->name);
            ui->listWidget->addItem(newitem);
        }
        ui->commandBrowseButton->setEnabled(true);
        ui->commandLineEdit->setEnabled(true);
        ui->nameLineEdit->setEnabled(true);
        ui->iconBrowseButton->setEnabled(true);
        ui->iconLineEdit->setEnabled(true);
        ui->updateButton->setEnabled(true);
        ui->removeButton->setEnabled(true);

        ui->commandLineEdit->setText(editors[0]->command);
        ui->nameLineEdit->setText(editors[0]->name);
        ui->iconLineEdit->setText(editors[0]->icon.name());
        ui->iconLabel->setPixmap(editors[0]->icon.pixmap(32,32));

        ui->listWidget->setCurrentRow(0);

    }
    isEditorAddFormActive = false;
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->installEventFilter(this);
    ui->addButton->setIcon(QIcon(QPixmap(theme + icon["Add"])));
    ui->removeButton->setIcon(QIcon(QPixmap(theme + icon["Remove"])));
    ui->updateButton->setIcon(QIcon(QPixmap(theme + icon["Update"])));
}

QExternProgramManager::~QExternProgramManager()
{
    delete ui;
}

void QExternProgramManager::on_updateButton_clicked()
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

    if (!ui->iconLineEdit->text().isEmpty())
        editors[ui->listWidget->currentRow()]->icon = QIcon(QPixmap(ui->iconLineEdit->text()));
    editors[ui->listWidget->currentRow()]->name = ui->nameLineEdit->text();
    editors[ui->listWidget->currentRow()]->command = ui->commandLineEdit->text();
    ui->listWidget->currentItem()->setIcon(QIcon(QPixmap(ui->iconLineEdit->text())));
    ui->listWidget->currentItem()->setText(ui->nameLineEdit->text());
}

void QExternProgramManager::on_removeButton_clicked()
{
    int pos = ui->listWidget->currentRow();
    editors.removeAt(pos);

    if (editors.size()>0) ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
    else ui->listWidget->clear();

    if (editors.isEmpty())
    {
        ui->commandBrowseButton->setEnabled(false);
        ui->commandLineEdit->setEnabled(false);
        ui->nameLineEdit->setEnabled(false);
        ui->iconBrowseButton->setEnabled(false);
        ui->iconLineEdit->setEnabled(false);
        ui->updateButton->setEnabled(false);
        ui->removeButton->setEnabled(false);
        ui->iconLabel->setPixmap(QPixmap(""));
    }
    else
    {
        on_listWidget_itemChanged(ui->listWidget->item(ui->listWidget->currentRow()));
    }

}

void QExternProgramManager::on_addButton_clicked()
{
    if (!isEditorAddFormActive)
    {
        newExternEditor();
        isEditorAddFormActive = true;
    }
}

void QExternProgramManager::on_okButton_clicked()
{
    if (ui->listWidget->count()>0) on_updateButton_clicked();
    emit overed(true);
}
void QExternProgramManager::on_cancelButton_clicked()
{
    emit overed(false);
}

void QExternProgramManager::on_listWidget_currentRowChanged(int currentRow)
{
    //EMPTY FOR DISABLE FUCKING BUGS
}
bool QExternProgramManager::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int keyInt = keyEvent->key();
        //DOWN KEY
        if ((int)keyInt == (int)Qt::Key_Down && ui->listWidget->currentRow() < editors.size()-1)
        {
            ui->listWidget->setCurrentRow(ui->listWidget->currentRow()+1);
            on_listWidget_itemChanged(ui->listWidget->item(ui->listWidget->currentRow()));
            return true;
        }
        //UP KEY
        if ((int)keyInt == (int)Qt::Key_Up && ui->listWidget->currentRow() > 0)
        {
            ui->listWidget->setCurrentRow(ui->listWidget->currentRow()-1);
            on_listWidget_itemChanged(ui->listWidget->item(ui->listWidget->currentRow()));
            return true;
        }
        return false;
    }
    return false;
}

void QExternProgramManager::on_listWidget_itemChanged(QListWidgetItem *item)
{
    ui->commandLineEdit->setText(editors[ui->listWidget->currentRow()]->command);
    ui->nameLineEdit->setText(editors[ui->listWidget->currentRow()]->name);
    ui->iconLineEdit->setText(editors[ui->listWidget->currentRow()]->icon.name());
    ui->iconLabel->setPixmap(editors[ui->listWidget->currentRow()]->icon.pixmap(32,32));
}


void QExternProgramManager::newExternEditor()
{
    editorAddForm = new QExternProgramAddForm(*installedSoft);
    connect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
    connect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
    editorAddForm->show();
    isEditorAddFormActive = true;
}

void QExternProgramManager::addEditor(QString name, QIcon icon, QString command)
{

    QExternProgram *editor = new QExternProgram(name,icon,command,imagewidget);
    editors.append(editor);

    if (editors.size() == 1)
    {
        ui->commandBrowseButton->setEnabled(true);
        ui->commandLineEdit->setEnabled(true);
        ui->nameLineEdit->setEnabled(true);
        ui->iconBrowseButton->setEnabled(true);
        ui->iconLineEdit->setEnabled(true);
        ui->updateButton->setEnabled(true);
        ui->removeButton->setEnabled(true);

        ui->commandLineEdit->setText(editors[0]->command);
        ui->nameLineEdit->setText(editors[0]->name);
        ui->iconLineEdit->setText(editors[0]->icon.name());
        ui->iconLabel->setPixmap(editors[0]->icon.pixmap(32,32));
    }

    QListWidgetItem * newitem = new QListWidgetItem(editor->icon,editor->name);
    editorsItems.append(newitem);
    ui->listWidget->addItem(newitem);

    QString dir;
#ifdef Q_OS_LINUX
    dir = QDir::homePath()+"/.config/QImageViewer/extern/";
#endif
#ifdef Q_OS_WIN32
    dir = QApplication::applicationDirPath()+"\\extern\\";
#endif
    if (!QDir(dir).exists())
    {
        QDir creator;
        creator.mkdir(dir);
    }
    QFile file(dir+name+".png");
    file.open(QIODevice::WriteOnly);
    QPixmap pixmap = icon.pixmap(32,32);
    pixmap.save(&file, QString("PNG").toStdString().c_str());

    qDebug() << name << icon.name() <<command;
    disconnect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}

void QExternProgramManager::abortAddingNewExternEditor()
{
    disconnect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));

    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}


void QExternProgramManager::on_iconBrowseButton_clicked()
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

void QExternProgramManager::on_commandBrowseButton_clicked()
{
    QString path;
    QFileDialog dialog;
    path = dialog.getOpenFileName(this,tr("Opening extern image application"),
                                   "",tr("All file formats (*.*)"));
    if(!path.isNull()) ui->commandLineEdit->setText(path);
}

void QExternProgramManager::closeEvent(QCloseEvent *event)
{
    if (isEditorAddFormActive) editorAddForm->close();
    event->accept();
}

void QExternProgramManager::on_listWidget_itemClicked(QListWidgetItem *item)
{
    on_listWidget_itemChanged(ui->listWidget->item(ui->listWidget->currentRow()));
}
