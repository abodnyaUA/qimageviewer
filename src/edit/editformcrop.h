#ifndef EDITFORMCROP_H
#define EDITFORMCROP_H


#include <QWidget>
#include <QGraphicsView>
#include "editformcropimage.h"


namespace Ui {
class editformCrop;
}

class editformCrop : public QWidget
{
    Q_OBJECT

public:
    explicit editformCrop(int width, int height);

    void loadImage(QPixmap pixmap_old);
    QPixmap *getpixmap();

    ~editformCrop();

private slots:
    void resizeprewiew();
    void on_widthSpinBox_valueChanged(int arg1);
    void on_heightSpinBox_valueChanged(int arg1);
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();
    void on_widthSpinBox_editingFinished();
    void on_heightSpinBox_editingFinished();

    void setnewLeftTop(int left, int top);
    void setnewRightDown(int right, int down);

signals:
    void editFinished(bool);

private:
    QGraphicsScene* scene;
    QPixmap* pixmap;
    editformcropimage *imagepreview;
    double zoom;
    int im_width,im_height;
    double originalAspectratio;
    Ui::editformCrop *ui;

    int left,right,top,down;
    bool isStartedCrop;
    int wd,hg;

protected:
    bool mouseisGrabbing;
};
#endif // EDITFORMCROP_H
