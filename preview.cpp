#include "preview.h"

preview::preview(QString name,int previewsize)
{
    resize(previewsize,previewsize);
    pixmap.load(name);
    imagename=name;
    if (pixmap.width()>pixmap.height())
         scene.addPixmap(pixmap.scaledToWidth(previewsize));
    else scene.addPixmap(pixmap.scaledToHeight(previewsize));
    graphwidget.resize(previewsize,previewsize);
    graphwidget.setScene(&scene);
    layout.addWidget(&graphwidget);
    label.setText("text");
    layout.addWidget(&label);
    setLayout(&layout);
}
