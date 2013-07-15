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
#include <QDragEnterEvent>
#include <QScrollArea>
#include <QSpacerItem>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QLibraryInfo>
#include <QTranslator>
#include <QSettings>
#include <QStyleFactory>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QEventLoop>
#include "image.h"
#include "previewmode.h"
#include "previewlist.h"
#include "fullscreen.h"
#include "edit/editformresize.h"
#include "edit/editformresizeelements.h"
#include "edit/editformcrop.h"
#include "edit/editformfilters.h"
#include "edit/imagefilter.h"
#include "settings/settings.h"
#include "settings/hotkeys.h"
//EXTERN APPLICATION
#include <QFileIconProvider>
#include "externeditor/qexternprogram.h"
#include "externeditor/qexternprogramaddform.h"
#include "externeditor/qexternprogrammanager.h"
//IMAGESHACK
#include "imageshack/imageshackuploader.h"
#include "imageshack/imageshacklistupload.h"
#include "imageshack/imageshacklistview.h"
//VK
#include "qvk/qvk.h"
#include "qvk/vkuploadimageform.h"
#include "qvk/vkuploadimagesform.h"
#include "qvk/vkdownloadalbumform.h"
//Help
#include "aboutform.h"
//UPDATE
#include "update/updateinformer.h"
#include "update/updatedialog.h"

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
    void listIsEmpty();
    void currentIndexWasChanged(int indx);
    void settingsWindow();
    void updateSettings(QString language,
                        QString defaultfolder,
                        bool autoUpdate,
                        bool mouseZoom, bool mouseFullscreen,
                        bool slideshowSmoothTransition, double slideshowInterval,
                        int panelalignment,
                        hotkeysStruct hotkeys, isneedButStruct isneedBut,
                        QColor fullscreencolor,int);
    //HELP
    void helpAbout();
    void helpAboutOvered();
    //NAVIGATION
    void nextImage();
    void prevImage();
    //WATCH
    void fullScreen();
    void fullScreenOvered();
    void fullScreenFromImage();
    void slideShow();
    //EDIT
    void acceptFilterGrayScale();
    void acceptFilterBrightness();
    void acceptFilterTemperature();
    void acceptFilterSaturate();
    void acceptFilterBlur();
    void acceptFilterSharpen();
    void acceptFilterSepia();
    void filterImage(Filter::FilterType filter);
    void filterImageOvered(bool result);
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
    void makeInstalledSoftList();
    void newExternEditor();
    void addEditor(QString name, QIcon icon, QString command);
    void abortAddingNewExternEditor();
    void exterEditorsManager();
    void exterEditorsManagerOvered(bool);
    //SHARE
    void imageshackShare();
    void imageshackShareList();
    void imageshackShareListOvered(bool arg);
    void imageshackShareListAborted(bool arg);
    //VKONTAKTE
    void vkStart();
    void vkLogIn();
    void vkLogOut();
    void vkUploadImage();
    void vkUploadImageOvered(bool);
    void vkUploadImageUpdateAlbums();
    void vkUploadImageList();
    void vkUploadImageListOvered(bool);
    void vkUploadImageListUpdate();
    void vkUploadImageListAbort(bool);
    void vkUploadImageListUpdateAlbums();
    void vkDownloadAlbum();
    void vkDownloadAlbumOvered(bool);
    void vkDownloadAlbumUpdate();
    void vkDownloadAlbumReady(int);
    void vkDownloadAlbumAbort(bool);
    //UPDATE
    void getUpdates(QNetworkReply* reply);
    void updateFile(int number);
    void checkupdates();
    void afterUpdates();
    //MODE
    void changeMode();
    void openImageFromPreview(int);
    //Image operations by Mode//
    void modeRotateLeft();
    void modeRotateRight();
    void modeFlipHorizontal();
    void modeFlipVertical();
    void modeResize();
    void modeUploadToVK();
    void modeUploadToImageshack();
    void modeDelete();
    void modeWallpaper();
    void previewsReady();

private:
    enum Mode {ModeImage, ModePreview} mode;

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
                *butProperties,
                *butMODE;
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
    PreviewMode *previewwiget;
    QSlider * zoomslider;

    // image file dialog //
    QFileDialog * dialog;

    // argv parameter //
    QString defaultpath;

    // Fullscreen //
    bool isfullScreenActive;
    fullscreen * fullScreenWidget;
    QColor fullscreencolor;

    // Edit Forms //
    editformFilters *editFormFilters;
    editformResize *editFormResize;
    editformCrop *editFormCrop;
    editformResizeElements *editFormResizeElements;

    // EXTERN EDITORS //
    QList<QExternProgram *> editors;
    QList<QExternProgram *> installedSoft;
    QList<QAction*> editorsActions;
    QExternProgramAddForm * editorAddForm;
    bool isEditorAddFormActive;
    QExternProgramManager * editorsManager;
    bool isEditosManagerActive;

    // SHARE //
    QList<ImageShackUploader *> imageshack;
    ImageShackListUpload * imageshackuploader;
    bool isImageShackListUploaderOpened;
    QList<ImageShackListView *> imageshacklist;

    // Vkontakte //
    QVk *vkApi;
    QString vkToken;
    int vkUserId;
    vkUploadImageForm *vkuploadimageform;
    bool isVkUploadImageFormActive;
    vkUploadImagesForm *vkuploadimagesform;
    bool isVkUploadImagesFormActive;
    int vkUploadImagesCount, vkUploadImagesAmount;
    vkDownloadAlbumForm *vkdownloadalbumform;
    bool isVkDownloadAlbumFormActive;
    int vkDownloadImagesCount, vkDownloadImagesAmount;

    // Help //
    aboutForm *aboutform;
    bool isAboutFormOpened;

    // Preview //
    bool isReadyPreviews;

    // Update //
    QNetworkAccessManager * updater;
    UpdateInformer * updateInformer;
    QString existDir ;
    QStringList needToUpdate;
    bool automateUpdate;
    bool checkAutoUpdates;
    int timesToUpdate;
    bool isUpdateDialogRunning;

    // Edit //
    bool isResizeListActive;

    void createActions();
    void createHotkeys();
    void createDesign();
    void createPanel();
    void loadsettings();
    void savesettings();

signals:
    void updateFinished();

protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
    void run();
};



#endif // QImageViewer_H
