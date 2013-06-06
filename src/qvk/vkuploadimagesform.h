#ifndef VKUPLOADIMAGESFORM_H
#define VKUPLOADIMAGESFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

namespace Ui {
class vkUploadImagesForm;
}

class vkUploadImagesForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit vkUploadImagesForm(QMap<QString,int> albums,QString addicon);
    void loadlist(QStringList list,QString folder, int current);
    int getalbum();
    void updatealbums(QMap<QString,int> albums);
    QStringList getlinkslist();
    void setCurrentUploadedItem(int item);
    ~vkUploadImagesForm();

signals:
    void overed(bool);
    void abort(bool);
    void addNewAlbum(QString name);

private slots:
    void on_uploadButton_clicked();
    void on_cancelButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_toolButton_clicked();

private:
    Ui::vkUploadImagesForm *ui;
    QStringList list;
    QString folder;

    QGraphicsScene * scene;
    QPixmap pixmap;

    QMap<QString,int> albums;
    QString newalbum;

    QStringList uploadlist;

    bool startUpload;
    bool exit;

    QMessageBox exitMessageBox;
    QAbstractButton *butYes,*butNo,*butMinimize;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // VKUPLOADIMAGESFORM_H
