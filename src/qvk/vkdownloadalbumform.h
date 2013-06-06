#ifndef VKDOWNLOADALBUM_H
#define VKDOWNLOADALBUM_H

#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

namespace Ui {
class vkDownloadAlbumForm;
}

class vkDownloadAlbumForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit vkDownloadAlbumForm(QMap<QString,int> albums,int userId, QString defaultfolder);
    QMap<QString,QVariant> getDownloadData();
    ~vkDownloadAlbumForm();
    void setCurrentDownloadedProcent(int procent);

signals:
    void overed(bool);
    void abort(bool);

private slots:
    void on_cancelButton_clicked();
    void on_acceptButton_clicked();
    void on_AnotherAlbumRadioButton_clicked();
    void on_UserAlbumRadioButton_clicked();
    void on_browseButton_clicked();

private:
    Ui::vkDownloadAlbumForm *ui;
    bool exit;
    QMap<QString,int> albums;
    int userId;
    bool startDownload;
    int lastSlashPos;

    QMessageBox exitMessageBox;
    QAbstractButton *butYes,*butNo,*butMinimize;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // VKDOWNLOADALBUM_H
