#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "hotkeys.h"
#include "qhotkeywidget.h"

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
                            hotkeysStruct hotkeys, isneedButStruct isneedBut);
    ~Settings();
    
private slots:
    void on_acceptButton_clicked();
    void on_resetButton_clicked();
    void on_defaultfolderBrowseButton_clicked();
    void on_slideshowIntervalButton_clicked();
    void on_cancelButton_clicked();
    void on_hotkeyWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void changingEndAccept();
    void changingEndDecline();

    void on_resetHotkeysButton_clicked();

    void on_panelReset_clicked();

signals:
    void acceptsettings(QString language,
                        QString defaultfolder,
                        bool mouseZoom, bool mouseFullscreen,
                        bool slideshowSmoothTransition, double slideshowInterval,
                        int panelalignment,
                        hotkeysStruct hotkeys, isneedButStruct isneedBut);

private:
    Ui::Settings *ui;
    QString old_lang;
    QString old_defaultfolder;
    bool old_mouseZoom;
    bool old_mouseFullscreen;
    bool old_slideshowSmoothTransition;
    double old_slideshowInterval;
    int old_panelalignment;

    ///Panel///
    isneedButStruct old_isneedBut;
    void setPanelButtons();
    void savePanelButtons();

    ///Hotkeys///
    //Change widget//
    qhotkeywidget * QHotkeyWidget;

    //buttons//
    void setHotkeys();
    void saveHotkeys();
    QTreeWidgetItem * addCategory(QString name);
    QTreeWidgetItem * addHotkey(QTreeWidgetItem * category, QString name, QString hotkey, QString icon, QString defaultHotkey);
    QStringList currentHotkeys;
    QTreeWidgetItem *categoryFile,*categoryEdit,*categoryWatch,*categoryHelp;
    QTreeWidgetItem *hotkeyItemFileOpen,*hotkeyItemFileSave,*hotkeyItemFileSaveAs,
                    *hotkeyItemFileSettings,*hotkeyItemFileQuit;
    QTreeWidgetItem *hotkeyItemEditUndo,*hotkeyItemEditRedo,
                    *hotkeyItemEditRotateLeft,*hotkeyItemEditRotateRight,
                    *hotkeyItemEditFlipHorizontal,*hotkeyItemEditFlipVertical,
                    *hotkeyItemEditCrop,*hotkeyItemEditResize,*hotkeyItemEditResizeItems;
    QTreeWidgetItem *hotkeyItemWatchPrevious,*hotkeyItemWatchNext,
                    *hotkeyItemWatchFullscreen,*hotkeyItemWatchSlideshow,
                    *hotkeyItemWatchWallpaper,*hotkeyItemZoomIn,*hotkeyItemZoomOut,
                    *hotkeyItemZoomWindow,*hotkeyItemZoomOriginal;
    QTreeWidgetItem *hotkeyItemHelpAbout;

    //values//
    hotkeysStruct old_hotkeys;

protected:
    void closeEvent(QCloseEvent *);
};

#endif // SETTINGS_H
