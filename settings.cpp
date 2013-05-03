#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) : ui(new Ui::Settings)
{
	ui->setupUi(this);
}

void Settings::setDefaultSettings(QString defaultfolder,
                        bool mouseZoom, bool mouseFullscreen,
                        bool slideshowSmoothTransition, int slideshowInterval)
{
    ui->defaultfolderLineEdit->setText(defaultfolder);
    ui->mouseFullscreenCheckBox->setChecked(mouseZoom);
    ui->mouseZoomCheckBox->setChecked(mouseFullscreen);
    ui->slideshowIntervalSpinBox->setValue(slideshowInterval);
    ui->slideshowTransitionCheckBox->setChecked(slideshowSmoothTransition);
}

Settings::~Settings()
{
}

void Settings::on_acceptButton_clicked()
{
    emit acceptsettings(ui->defaultfolderLineEdit->text(),
                        ui->mouseZoomCheckBox->isChecked(),
                        ui->mouseFullscreenCheckBox->isChecked(),
                        ui->slideshowTransitionCheckBox->isChecked(),
                        ui->slideshowIntervalSpinBox->value());
    close();
}

void Settings::closeEvent(QCloseEvent *event)
{
    on_acceptButton_clicked();
    event->accept();
}

void Settings::on_resetButton_clicked()
{
    ui->defaultfolderLineEdit->setText("/home/abodnya/");
    ui->mouseFullscreenCheckBox->setChecked(true);
    ui->mouseZoomCheckBox->setChecked(true);
    ui->slideshowIntervalSpinBox->setValue(1000);
    ui->slideshowTransitionCheckBox->setChecked(true);
}

void Settings::on_cropResetButton_clicked()
{

}

void Settings::on_resizeResetButton_clicked()
{

}

void Settings::on_fullscreenResetButton_clicked()
{

}

void Settings::on_slideshowResetButton_clicked()
{

}

void Settings::on_undoResetButton_clicked()
{

}

void Settings::on_redoResetButton_clicked()
{

}

void Settings::on_defaultfolderBrowseButton_clicked()
{

}

void Settings::on_slideshowIntervalButton_clicked()
{
    ui->slideshowIntervalSpinBox->setValue(1000);
}
