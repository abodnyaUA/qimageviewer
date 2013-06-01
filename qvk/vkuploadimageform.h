#ifndef VKUPLOADIMAGEFORM_H
#define VKUPLOADIMAGEFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QInputDialog>
#include <QDebug>

namespace Ui {
class vkUploadImageForm;
}

class vkUploadImageForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit vkUploadImageForm(QMap<QString,int> albums,QString addicon);
    int getalbum();
    void updatealbums(QMap<QString,int> albums);
    ~vkUploadImageForm();
    
signals:
    void overed(bool);
    void addNewAlbum(QString name);

private slots:
    void on_userAlbumButton_clicked();
    void on_otherAlbumButton_clicked();
    void on_uploadButton_clicked();
    void on_cancelButton_clicked();
    void on_toolButton_clicked();

private:
    Ui::vkUploadImageForm *ui;
    bool exit;
    QMap<QString,int> albums;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // VKUPLOADIMAGEFORM_H
