#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT
    
public:
    explicit Settings(QWidget *parent = 0);
    void setDefaultSettings(QString language,
                            QString defaultfolder,
                            bool mouseZoom, bool mouseFullscreen,
                            bool slideshowSmoothTransition, double slideshowInterval,
                            int panelalignment,
                            QString cropHotkey, QString resizeHotkey,
                            QString fullscreenHotkey, QString slideshowHotkey,
                            QString undoHotkey, QString redoHotkey);
    ~Settings();
    
private slots:
    void on_acceptButton_clicked();
    void on_resetButton_clicked();
    void on_cropResetButton_clicked();
    void on_resizeResetButton_clicked();
    void on_fullscreenResetButton_clicked();
    void on_slideshowResetButton_clicked();
    void on_undoResetButton_clicked();
    void on_redoResetButton_clicked();
    void on_defaultfolderBrowseButton_clicked();
    void on_slideshowIntervalButton_clicked();

    void on_cancelButton_clicked();

signals:
    void acceptsettings(QString language,
                        QString defaultfolder,
                        bool mouseZoom, bool mouseFullscreen,
                        bool slideshowSmoothTransition, double slideshowInterval,
                        int panelalignment,
                        QString cropHotkey, QString resizeHotkey,
                        QString fullscreenHotkey, QString slideshowHotkey,
                        QString undoHotkey, QString redoHotkey);

private:
    Ui::Settings *ui;
    QString old_lang;
    QString old_defaultfolder;
    bool old_mouseZoom;
    bool old_mouseFullscreen;
    bool old_slideshowSmoothTransition;
    double old_slideshowInterval;
    int old_panelalignment;
    //Hotkeys//
    QString old_cropHotkey;
    QString old_resizeHotkey;
    QString old_fullscreenHotkey;
    QString old_slideshowHotkey;
    QString old_undoHotkey;
    QString old_redoHotkey;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // SETTINGS_H
