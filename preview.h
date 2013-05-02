#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QLabel>

class preview : public QWidget
{
    Q_OBJECT
public:
    explicit preview(QString name, int previewsize);
    
signals:
    
public slots:
    
private:
    QString imagename;
    QGraphicsView graphwidget;
    QPixmap pixmap;
    QGraphicsScene scene;
    QVBoxLayout layout;
    QLabel label;
};

#endif // PREVIEW_H
