#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) : ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->languageComboBox->setItemIcon(0,(QIcon(QPixmap(":/res/flag-sys.png"))));
    ui->languageComboBox->setItemIcon(1,(QIcon(QPixmap(":/res/flag-usa.png"))));
    ui->languageComboBox->setItemIcon(2,(QIcon(QPixmap(":/res/flag-rus.png"))));
    ui->languageComboBox->setItemIcon(3,(QIcon(QPixmap(":/res/flag-ukr.png"))));
}

void Settings::setDefaultSettings(QString language,
                                  QString defaultfolder,
                                  bool mouseZoom, bool mouseFullscreen,
                                  bool slideshowSmoothTransition, double slideshowInterval,
                                  QString cropHotkey, QString resizeHotkey,
                                  QString fullscreenHotkey, QString slideshowHotkey,
                                  QString undoHotkey, QString redoHotkey)
{
    old_lang = language;
    old_defaultfolder = defaultfolder;
    old_mouseZoom = mouseZoom;
    old_mouseFullscreen = mouseFullscreen;
    old_slideshowInterval = slideshowInterval;
    old_slideshowSmoothTransition = slideshowSmoothTransition;
    old_cropHotkey = cropHotkey;
    old_resizeHotkey = resizeHotkey;
    old_fullscreenHotkey = fullscreenHotkey;
    old_slideshowHotkey = slideshowHotkey;
    old_undoHotkey = undoHotkey;
    old_redoHotkey = redoHotkey;

    if (language == "eng") ui->languageComboBox->setCurrentIndex(1);
    else if (language == "rus") ui->languageComboBox->setCurrentIndex(2);
    else if (language == "ukr") ui->languageComboBox->setCurrentIndex(3);
    else ui->languageComboBox->setCurrentIndex(0);

    ui->defaultfolderLineEdit->setText(defaultfolder);
    ui->mouseFullscreenCheckBox->setChecked(mouseZoom);
    ui->mouseZoomCheckBox->setChecked(mouseFullscreen);
    ui->slideshowIntervalSpinBox->setValue(slideshowInterval);
    ui->slideshowTransitionCheckBox->setChecked(slideshowSmoothTransition);

    ui->cropHotkeyEdit->setText(cropHotkey);
    ui->resizeHotkeyEdit->setText(resizeHotkey);
    ui->fullscreenHotkeyEdit->setText(fullscreenHotkey);
    ui->slideshowHotkeyEdit->setText(slideshowHotkey);
    ui->undoHotkeyEdit->setText(undoHotkey);
    ui->redoHotkeyEdit->setText(redoHotkey);
}

Settings::~Settings()
{
}

void Settings::on_acceptButton_clicked()
{
    QString lang;
    switch (ui->languageComboBox->currentIndex())
    {
    case 0:lang = "sys";break;
    case 1:lang = "eng";break;
    case 2:lang = "rus";break;
    case 3:lang = "ukr";break;
    }

    if (old_lang != lang)
        QMessageBox::question(this, tr("Warning!"),
                              tr("Language will be changing after restart program"),
                              QMessageBox::Ok | QMessageBox::Default);

    old_lang = lang;
    old_defaultfolder = ui->defaultfolderLineEdit->text();
    old_mouseZoom = ui->mouseZoomCheckBox->isChecked();
    old_mouseFullscreen = ui->mouseFullscreenCheckBox->isChecked();
    old_slideshowInterval = ui->slideshowIntervalSpinBox->value();
    old_slideshowSmoothTransition = ui->slideshowTransitionCheckBox->isChecked();

    old_cropHotkey = ui->cropHotkeyEdit->text();
    old_resizeHotkey = ui->resizeHotkeyEdit->text(),
    old_fullscreenHotkey = ui->fullscreenHotkeyEdit->text();
    old_slideshowHotkey = ui->slideshowHotkeyEdit->text();
    old_undoHotkey = ui->undoHotkeyEdit->text();
    old_redoHotkey = ui->redoHotkeyEdit->text();

    close();
}

void Settings::closeEvent(QCloseEvent *event)
{

    emit acceptsettings(old_lang,
                        old_defaultfolder,
                        old_mouseZoom,
                        old_mouseFullscreen,
                        old_slideshowSmoothTransition,
                        old_slideshowInterval,
                        old_cropHotkey, old_resizeHotkey,
                        old_fullscreenHotkey, old_slideshowHotkey,
                        old_undoHotkey, old_redoHotkey);
    event->accept();
}

void Settings::on_resetButton_clicked()
{
#ifdef Q_OS_LINUX
    ui->defaultfolderLineEdit->setText("/home/"+QString::fromLocal8Bit( getenv("USER") ));
#endif
#ifdef Q_OS_WIN32
    ui->defaultfolderLineEdit->setText("C:\\Users\\"+QString::fromLocal8Bit( getenv("USER") ));
#endif
    ui->mouseFullscreenCheckBox->setChecked(true);
    ui->mouseZoomCheckBox->setChecked(true);
    ui->slideshowIntervalSpinBox->setValue(1);
    ui->slideshowTransitionCheckBox->setChecked(true);
}

void Settings::on_cropResetButton_clicked()
{    ui->cropHotkeyEdit->setText("Ctrl+Shift+C");}
void Settings::on_resizeResetButton_clicked()
{    ui->resizeHotkeyEdit->setText("Ctrl+R");}
void Settings::on_fullscreenResetButton_clicked()
{    ui->fullscreenHotkeyEdit->setText("F10");}
void Settings::on_slideshowResetButton_clicked()
{    ui->slideshowHotkeyEdit->setText("F5");}
void Settings::on_undoResetButton_clicked()
{    ui->resizeHotkeyEdit->setText("Ctrl+Z");}
void Settings::on_redoResetButton_clicked()
{    ui->resizeHotkeyEdit->setText("Ctrl+Shift+Z");}

void Settings::on_defaultfolderBrowseButton_clicked()
{
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setFileMode(QFileDialog::Directory);
    QString path = dialog.getExistingDirectory(this,tr("Open directory"),
                                               ui->defaultfolderLineEdit->text(),
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
    ui->defaultfolderLineEdit->setText(path);
}

void Settings::on_slideshowIntervalButton_clicked()
{
    ui->slideshowIntervalSpinBox->setValue(1000);
}

void Settings::on_cancelButton_clicked()
{
    close();
}
