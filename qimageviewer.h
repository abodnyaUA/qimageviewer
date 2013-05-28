#ifndef QImageViewer_H
#define QImageViewer_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPixmap>
#include <QSlider>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QDebug>
#include <QCloseEvent>
#include <QDropEvent>
#include <QScrollArea>
#include <QSpacerItem>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QLibraryInfo>
#include <QTranslator>
#include <QSettings>
#include "image.h"
#include "preview.h"
#include "previewlist.h"
#include "fullscreen.h"
#include "editformresize.h"
#include "editformresizeelements.h"
#include "editformcrop.h"
#include "settings.h"
#include "hotkeys.h"
#include "qexternprogram.h"
#include "qexternprogramaddform.h"
#include "qexternprogrammanager.h"
#include "imageshackuploader.h"

namespace Ui {
class QImageViewer;
}

class QImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit QImageViewer(QString path = QString::null, QWidget *parent = 0);
    ~QImageViewer();
    
private slots:
    //FILE
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void filesFind();
    void currentIndexWasChanged(int indx);
    void settingsWindow();
    void updateSettings(QString language,
                        QString defaultfolder,
                        bool mouseZoom, bool mouseFullscreen,
                        bool slideshowSmoothTransition, double slideshowInterval,
                        int panelalignment,
                        hotkeysStruct hotkeys, isneedButStruct isneedBut,
                        QColor fullscreencolor,int);
    //HELP
    void helpAbout();
    //NAVIGATION
    void nextImage();
    void prevImage();
    //WATCH
    void fullScreen();
    void fullScreenOvered();
    void fullScreenFromImage();
    void slideShow();
    //EDIT
    void resizeImage();
    void resizeImageOvered(bool result);
    void cropImage();
    void cropImageOvered(bool result);
    void resizeImageList();
    void resizeImageListOvered(bool result);
    void setUndoEnable(bool);
    void setRedoEnable(bool);
    void setStatusName(bool);
    //EXTERN PROGRAM
    void newExternEditor();
    void addEditor(QString name, QString icon, QString command);
    void abortAddingNewExternEditor();
    void exterEditorsManager();
    void exterEditorsManagerOvered(bool);
    //SHARE
    void imageshackShare();

private:
    /// Settings ///
    QSettings *qsettings;
    bool isSettingsActive;

    //Default Folder//
    QString lastdirectory;

    //Buttons iconpacks//
    QString iconpacksfolder;
    int currenticonpack;
    QString currenticonpackString;
    QStringList iconpacks;
    QMap<QString,QString> icon;

    //Language//
    QString language;

    //Fullscreen//
    bool slideshowSmoothTransition;
    double slideshowInterval;

    //Panel//
    int panelalignment;
    QList<QPushButton *> buttonsList;
    QPushButton *butRotateLeft,*butRotateRight,
                *butFlipHorizontal,*butFlipVertical,
                *butZoomIn,*butZoomOut,
                *butZoomOriginal,*butZoomWindow,
                *butFullscreen,*butSlideshow,
                *butProperties;
    struct isneedButStruct isneedBut;
    QSpacerItem *spacerLeft,*spacerRight;

    //Hotkeys//
    hotkeysStruct hotkeys;
    Settings *settings;

    // desktop size //
    QDesktopWidget desk;

    // main form //
    Ui::QImageViewer *ui;
    image *imagewidget;
    QSlider * zoomslider;

    // preview [not released] //
    QScrollArea previewArea;
    previewList * prevList;
    QHBoxLayout prevLayout;

    // image file dialog //
    QFileDialog * dialog;

    // argv parameter //
    QString defaultpath;

    // Fullscreen //
    bool isfullScreenActive;
    fullscreen * fullScreenWidget;
    QColor fullscreencolor;

    // Edit Forms //
    editformResize *editFormResize;
    editformCrop *editFormCrop;
    editformResizeElements *editFormResizeElements;

    // EXTERN EDITORS //
    QList<QExternProgram *> editors;
    QList<QAction*> editorsActions;
    QExternProgramAddForm * editorAddForm;
    bool isEditorAddFormActive;
    QExternProgramManager * editorsManager;
    bool isEditosManagerActive;

    // SHARE //
    QList<ImageShackUploader *> imageshack;


    void createActions();
    void createHotkeys();
    void createDesign();
    void createPanel();
    void loadsettings();
    void savesettings();

protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *event);
};



#endif // QImageViewer_H
