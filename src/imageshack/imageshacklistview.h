#ifndef IMAGESHACKLISTVIEW_H
#define IMAGESHACKLISTVIEW_H

#include <QWidget>
#include "imageshacklistupload.h"

namespace Ui {
class ImageShackListView;
}

class ImageShackListView : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageShackListView(QList< QMap<QString,QString> > lst);
    ~ImageShackListView();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::ImageShackListView *ui;
};

#endif // IMAGESHACKLISTVIEW_H
