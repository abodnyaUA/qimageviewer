#ifndef PREVIEWLIST_H
#define PREVIEWLIST_H

#include <QWidget>
#include "preview.h"
#include <QHBoxLayout>

class previewList : public QWidget
{
    Q_OBJECT
public:
    explicit previewList(QStringList list);
    
signals:
    
public slots:
    
private:
    QList<preview *> lst;
    QHBoxLayout layout;
};

#endif // PREVIEWLIST_H
