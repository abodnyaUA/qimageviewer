#ifndef UPDATEINFORMER_H
#define UPDATEINFORMER_H

#include <QWidget>

namespace Ui {
class UpdateInformer;
}

class UpdateInformer : public QWidget
{
    Q_OBJECT
    
public:
    explicit UpdateInformer(QWidget *parent = 0);
    ~UpdateInformer();
    void setProgress(double procent, QString file);
    
private:
    Ui::UpdateInformer *ui;
};

#endif // UPDATEINFORMER_H
