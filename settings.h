#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QPixmap>
#include <QRgb>
#include "hotkeys.h"
#include "qhotkeywidget.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT
    
public:
    explicit Settings(QStringList iconpacks, QMap<QString, QString> icon);
    void setDefaultSettings(QString language,
                            QString defaultfolder,
                            bool mouseZoom, bool mouseFullscreen,
                            bool slideshowSmoothTransition, double slideshowInterval,
                            int panelalignment,
                            hotkeysStruct hotkeys, isneedButStruct isneedBut,
                            QColor fullscreencolor,
                            int currenticonpack);
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
    void on_fullscreenColorButton_clicked();

signals:
    void acceptsettings(QString language,
                        QString defaultfolder,
                        bool mouseZoom, bool mouseFullscreen,
                        bool slideshowSmoothTransition, double slideshowInterval,
                        int panelalignment,
                        hotkeysStruct hotkeys, isneedButStruct isneedBut,
                        QColor fullscreencolor,
                        int currenticonpack);

private:
    QStringList iconpacks;
    QMap<QString,QString> icon;
    int old_currenticonpack;

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
    bool isActiveHotkeyWidget;

    //buttons//
    void setHotkeys();
    void saveHotkeys();
    QTreeWidgetItem * addCategory(QString name);
    QTreeWidgetItem * addHotkey(QTreeWidgetItem * category, QString name, QString hotkey, QString icon, QString defaultHotkey);
    QList<QTreeWidgetItem *> allactions;
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

    /// Design ///
    //Fullscreen color//
    QColorDialog colordialog;
    QColor fullscreencolor,old_fullscreencolor;

protected:
    void closeEvent(QCloseEvent *);
};

#endif // SETTINGS_H
