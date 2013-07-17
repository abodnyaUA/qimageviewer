#ifndef PREVIEWMODE_H
#define PREVIEWMODE_H

#include <QListWidget>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QDir>
#include <QString>
#include <QThread>
#include <QList>
#include <QEvent>
#include <QKeyEvent>
#include "previewlist.h"
#include "algorithms.h"

class PreviewMode : public QListWidget
{
    Q_OBJECT
public:
    explicit PreviewMode();
    void loadImages(QStringList images);
    int size();
    QString folder();
    void reloadPreview(int index);
    void reloadPreviews(QList<int> indexes);
    bool rotateLeft(QString currentImage, bool isSaved);
    bool rotateRight(QString currentImage, bool isSaved);
    bool flipHorizontal(QString currentImage, bool isSaved);
    bool flipVertical(QString currentImage, bool isSaved);
    QStringList selectedImages();
    QStringList getList();
    void removeImage(QString name);
    
private:
    QStringList images;
    QThread *previewThread;
    previewList *previewLoader;

private slots:
    void onItemDoubleClicked(QListWidgetItem *);
    void updateView();
signals:
    void openImage(int);
    void ready();

protected:
    bool eventFilter(QObject *, QEvent *);
//    void keyPressEvent(QKeyEvent *event);
};

#endif // PREVIEWMODE_H
