#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT
    
public:
    explicit Settings(QWidget *parent = 0);
    void setDefaultSettings(QString defaultfolder,
                            bool mouseZoom, bool mouseFullscreen,
                            bool slideshowSmoothTransition, int slideshowInterval);
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

signals:
    void acceptsettings(QString defaultfolder,
                        bool mouseZoom, bool mouseFullscreen,
                        bool slideshowSmoothTransition, int slideshowInterval);

private:
    Ui::Settings *ui;

protected:
    void closeEvent(QCloseEvent *);
};

#endif // SETTINGS_H
