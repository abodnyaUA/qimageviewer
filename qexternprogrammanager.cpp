#include "qexternprogrammanager.h"
#include "ui_qexternprogrammanager.h"

QExternProgramManager::QExternProgramManager(QList<QExternProgram *> editors, image * imagewidget) :
    ui(new Ui::QExternProgramManager)
{
    ui->setupUi(this);
    this->editors = editors;
    if (!editors.isEmpty())
    {
        for (int i=0;i<this->editors.size();i++)
        {
            QListWidgetItem * newitem = new QListWidgetItem(QIcon(QPixmap(editors[i]->icon)),editors[i]->name);
            ui->listWidget->addItem(newitem);
        }
        ui->commandBrowseButton->setEnabled(true);
        ui->commandLineEdit->setEnabled(true);
        ui->nameLineEdit->setEnabled(true);
        ui->iconBrowseButton->setEnabled(true);
        ui->iconLineEdit->setEnabled(true);
        ui->acceptButton->setEnabled(true);
        ui->removeButton->setEnabled(true);

        ui->commandLineEdit->setText(editors[0]->command);
        ui->nameLineEdit->setText(editors[0]->name);
        ui->iconLineEdit->setText(editors[0]->icon);
        ui->iconLabel->setPixmap(QPixmap(editors[0]->icon).scaled(32,32,Qt::KeepAspectRatioByExpanding));

        ui->listWidget->setCurrentRow(0);

    }
    isEditorAddFormActive = false;
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->installEventFilter(this);
}

QExternProgramManager::~QExternProgramManager()
{
    delete ui;
}

void QExternProgramManager::on_acceptButton_clicked()
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
                             tr("Program path (command) can't be empty"),
                                    QMessageBox::Ok | QMessageBox::Default);
        return;
    }

    editors[ui->listWidget->currentRow()]->icon = ui->iconLineEdit->text();
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
        ui->acceptButton->setEnabled(false);
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
    if (ui->listWidget->count()>0) on_acceptButton_clicked();
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
    ui->iconLineEdit->setText(editors[ui->listWidget->currentRow()]->icon);
    ui->iconLabel->setPixmap(QPixmap(ui->iconLineEdit->text()).scaled(32,32,Qt::KeepAspectRatioByExpanding));
}


void QExternProgramManager::newExternEditor()
{
    editorAddForm = new QExternProgramAddForm;
    connect(editorAddForm,SIGNAL(accept(QString,QString,QString)),this,SLOT(addEditor(QString,QString,QString)));
    connect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
    editorAddForm->show();
    isEditorAddFormActive = true;
}

void QExternProgramManager::addEditor(QString name, QString icon, QString command)
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
        ui->acceptButton->setEnabled(true);
        ui->removeButton->setEnabled(true);

        ui->commandLineEdit->setText(editors[0]->command);
        ui->nameLineEdit->setText(editors[0]->name);
        ui->iconLineEdit->setText(editors[0]->icon);
        ui->iconLabel->setPixmap(QPixmap(editors[0]->icon).scaled(32,32,Qt::KeepAspectRatioByExpanding));
    }

    QListWidgetItem * newitem = new QListWidgetItem(QIcon(QPixmap(editor->icon)),editor->name);
    editorsItems.append(newitem);
    ui->listWidget->addItem(newitem);

    qDebug() << name << icon <<command;
    disconnect(editorAddForm,SIGNAL(accept(QString,QString,QString)),this,SLOT(addEditor(QString,QString,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}

void QExternProgramManager::abortAddingNewExternEditor()
{
    disconnect(editorAddForm,SIGNAL(accept(QString,QString,QString)),this,SLOT(addEditor(QString,QString,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));

    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}


void QExternProgramManager::on_iconBrowseButton_clicked()
{
    QString path;
    QFileDialog dialog;
    path = dialog.getOpenFileName(this,tr("Opening extern image program"),
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
    path = dialog.getOpenFileName(this,tr("Opening extern image program"),
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
