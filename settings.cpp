#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QStringList iconpacks, QMap<QString, QString> icon) : ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->languageComboBox->setItemIcon(0,(QIcon(QPixmap(":/res/flag-sys.png"))));
    ui->languageComboBox->setItemIcon(1,(QIcon(QPixmap(":/res/flag-usa.png"))));
    ui->languageComboBox->setItemIcon(2,(QIcon(QPixmap(":/res/flag-rus.png"))));
    ui->languageComboBox->setItemIcon(3,(QIcon(QPixmap(":/res/flag-ukr.png"))));
    ui->languageComboBox->setItemIcon(4,(QIcon(QPixmap(":/res/flag-pol.png"))));
    ui->acceptButton->setShortcut(QKeySequence(Qt::Key_Return));
    ui->cancelButton->setShortcut(QKeySequence(Qt::Key_Escape));
    isActiveHotkeyWidget = false;
    this->iconpacks = iconpacks;
    this->icon = icon;

    ui->iconpackBox->addItem("Original");
    for (int i=1;i<iconpacks.size();i++)
    {
        QDir dir(iconpacks[i]);
        ui->iconpackBox->addItem(dir.dirName());
    }

}

void Settings::setDefaultSettings(QString language,
                                  QString defaultfolder,
                                  bool mouseZoom, bool mouseFullscreen,
                                  bool slideshowSmoothTransition, double slideshowInterval,
                                  int panelalignment,hotkeysStruct hotkeys, isneedButStruct isneedBut,
                                  QColor fullscreencolor,
                                  int currenticonpack)
{
    old_lang = language;
    old_defaultfolder = defaultfolder;
    old_mouseZoom = mouseZoom;
    old_mouseFullscreen = mouseFullscreen;
    old_slideshowInterval = slideshowInterval;
    old_slideshowSmoothTransition = slideshowSmoothTransition;
    old_panelalignment = panelalignment;
    old_fullscreencolor = fullscreencolor;
    old_currenticonpack = currenticonpack;

    old_hotkeys = hotkeys;
    old_isneedBut = isneedBut;

    if (language == "eng") ui->languageComboBox->setCurrentIndex(1);
    else if (language == "rus") ui->languageComboBox->setCurrentIndex(2);
    else if (language == "ukr") ui->languageComboBox->setCurrentIndex(3);
    else if (language == "pol") ui->languageComboBox->setCurrentIndex(4);
    else ui->languageComboBox->setCurrentIndex(0);

    ui->defaultfolderLineEdit->setText(defaultfolder);
    ui->mouseFullscreenCheckBox->setChecked(mouseFullscreen);
    ui->mouseZoomCheckBox->setChecked(mouseZoom);
    ui->slideshowIntervalSpinBox->setValue(slideshowInterval);
    ui->slideshowTransitionCheckBox->setChecked(slideshowSmoothTransition);
    ui->iconpackBox->setCurrentIndex(currenticonpack);

    QString style = "background-color: rgb(%1, %2, %3);";
    ui->fullscreenColorLabel->setStyleSheet(style.arg(fullscreencolor.red()).
                                                  arg(fullscreencolor.green()).
                                                  arg(fullscreencolor.blue()));

    ui->toolpanelComboBox->setCurrentIndex(panelalignment);

    setHotkeys();
    setPanelButtons();
}

void Settings::setPanelButtons()
{
    ui->panelbutRotateLeftCheckBox->setChecked(old_isneedBut.rotateLeft);
    ui->panelbutRotateRightCheckBox->setChecked(old_isneedBut.rotateRight);
    ui->panelbutFlipHorizontalCheckBox->setChecked(old_isneedBut.flipHorizontal);
    ui->panelbutFlipVerticalCheckBox->setChecked(old_isneedBut.flipVertical);
    ui->panelbutZoomInCheckBox->setChecked(old_isneedBut.zoomIn);
    ui->panelbutZoomOutCheckBox->setChecked(old_isneedBut.zoomOut);
    ui->panelbutZoomWindowCheckBox->setChecked(old_isneedBut.zoomWindow);
    ui->panelbutZoomOriginalCheckBox->setChecked(old_isneedBut.zoomOriginal);
    ui->panelbutFullscreenCheckBox->setChecked(old_isneedBut.fullscreen);
    ui->panelbutSlideshowCheckBox->setChecked(old_isneedBut.slideshow);
    ui->panelbutPropertiesCheckBox->setChecked(old_isneedBut.properties);
}

void Settings::savePanelButtons()
{
    old_isneedBut.rotateLeft = ui->panelbutRotateLeftCheckBox->isChecked();
    old_isneedBut.rotateRight = ui->panelbutRotateRightCheckBox->isChecked();
    old_isneedBut.flipHorizontal = ui->panelbutFlipHorizontalCheckBox->isChecked();
    old_isneedBut.flipVertical = ui->panelbutFlipVerticalCheckBox->isChecked();
    old_isneedBut.zoomIn = ui->panelbutZoomInCheckBox->isChecked();
    old_isneedBut.zoomOut = ui->panelbutZoomOutCheckBox->isChecked();
    old_isneedBut.zoomWindow = ui->panelbutZoomWindowCheckBox->isChecked();
    old_isneedBut.zoomOriginal = ui->panelbutZoomOriginalCheckBox->isChecked();
    old_isneedBut.fullscreen = ui->panelbutFullscreenCheckBox->isChecked();
    old_isneedBut.slideshow = ui->panelbutSlideshowCheckBox->isChecked();
    old_isneedBut.properties = ui->panelbutPropertiesCheckBox->isChecked();
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
    case 4:lang = "pol";break;
    }

    if (old_lang != lang)
        QMessageBox::question(this, tr("Warning!"),
                              tr("Language will be switched after application restart"),
                              QMessageBox::Ok | QMessageBox::Default);

    old_lang = lang;
    old_defaultfolder = ui->defaultfolderLineEdit->text();
    old_mouseZoom = ui->mouseZoomCheckBox->isChecked();
    old_mouseFullscreen = ui->mouseFullscreenCheckBox->isChecked();
    old_slideshowInterval = ui->slideshowIntervalSpinBox->value();
    old_slideshowSmoothTransition = ui->slideshowTransitionCheckBox->isChecked();
    old_panelalignment = ui->toolpanelComboBox->currentIndex();
    old_fullscreencolor = fullscreencolor;
    old_currenticonpack = ui->iconpackBox->currentIndex();

    saveHotkeys();
    savePanelButtons();

    close();
}

void Settings::closeEvent(QCloseEvent *event)
{
    if (isActiveHotkeyWidget) QHotkeyWidget->close();
    emit acceptsettings(old_lang,
                        old_defaultfolder,
                        old_mouseZoom,
                        old_mouseFullscreen,
                        old_slideshowSmoothTransition,
                        old_slideshowInterval,
                        old_panelalignment,
                        old_hotkeys, old_isneedBut,
                        old_fullscreencolor,
                        old_currenticonpack);
    qDebug() << "old_isneed = " << old_isneedBut.rotateLeft << old_isneedBut.rotateRight;
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

void Settings::on_defaultfolderBrowseButton_clicked()
{
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setFileMode(QFileDialog::Directory);
    QString path = dialog.getExistingDirectory(this,tr("Open directory"),
                                               ui->defaultfolderLineEdit->text(),
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
    if( !path.isNull() )
        ui->defaultfolderLineEdit->setText(path);
}

void Settings::on_slideshowIntervalButton_clicked()
{
    ui->slideshowIntervalSpinBox->setValue(1);
}

void Settings::on_cancelButton_clicked()
{
    close();
}


void Settings::on_panelReset_clicked()
{
    ui->panelbutRotateLeftCheckBox->setChecked(true);
    ui->panelbutRotateRightCheckBox->setChecked(true);
    ui->panelbutFlipHorizontalCheckBox->setChecked(true);
    ui->panelbutFlipVerticalCheckBox->setChecked(true);
    ui->panelbutZoomInCheckBox->setChecked(true);
    ui->panelbutZoomOutCheckBox->setChecked(true);
    ui->panelbutZoomWindowCheckBox->setChecked(true);
    ui->panelbutZoomOriginalCheckBox->setChecked(true);
    ui->panelbutFullscreenCheckBox->setChecked(true);
    ui->panelbutSlideshowCheckBox->setChecked(true);
    ui->panelbutPropertiesCheckBox->setChecked(false);
}

void Settings::on_fullscreenColorButton_clicked()
{
    fullscreencolor = colordialog.getColor(fullscreencolor);
    QString style = "background-color: rgb(%1, %2, %3);";
    ui->fullscreenColorLabel->setStyleSheet(style.arg(fullscreencolor.red()).
                                                  arg(fullscreencolor.green()).
                                                  arg(fullscreencolor.blue()));
}
