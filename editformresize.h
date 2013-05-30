#ifndef EDITFORMRESIZE_H
#define EDITFORMRESIZE_H

#include <QWidget>
#include <QGraphicsView>
#include <QState>
#include <QVariant>
#include <QPropertyAnimation>
#include <QStateMachine>


namespace Ui {
class editformResize;
}

class editformResize : public QWidget
{
    Q_OBJECT
    
public:
    explicit editformResize(int width, int height);

    void loadImage(QPixmap pixmap_old);
    QPixmap *getpixmap();

    ~editformResize();
    
private slots:
    void resizeprewiew();
    void on_widthSpinBox_valueChanged(int arg1);
    void on_heightSpinBox_valueChanged(int arg1);
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();
    void on_widthSpinBox_editingFinished();
    void on_heightSpinBox_editingFinished();
    void on_aspectratioCheckBox_clicked();

signals:
    void editFinished(bool);

private:
    QGraphicsScene* scene;
    QPixmap* pixmap;
    double zoom;
    int im_width,im_height;
    double originalAspectratio;
    Ui::editformResize *ui;
    int wd,hg;
};

#endif // EDITFORMRESIZE_H
