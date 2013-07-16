#ifndef EDITFORMFILTERS_H
#define EDITFORMFILTERS_H

#include <QWidget>
#include <QGraphicsView>
#include <QState>
#include <QVariant>
#include <QPropertyAnimation>
#include <QStateMachine>
#include "edit/imagefilter.h"

namespace Ui {
class editformFilters;
}

class editformFilters : public QWidget
{
    Q_OBJECT
    
public:
    explicit editformFilters(int width, int height, Filter::FilterType filter);
    void loadImage(QPixmap pixmap_old);
    QPixmap *getpixmap();

    ~editformFilters();
    
private slots:
    void filtersPreview();
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();

signals:
    void editFinished(bool);

private:
    Filter::FilterType filter;
    ImageFilter Filter;
    QGraphicsScene* scene;
    QPixmap* pixmap;
    int im_width,im_height;
    double zoom;
    Ui::editformFilters *ui;
    int wd,hg;
};

#endif // EDITFORMFILTERS_H
