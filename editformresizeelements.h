#ifndef EDITFORMRESIZEELEMENTS_H
#define EDITFORMRESIZEELEMENTS_H

#include <QWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>
#include <QFileDialog>

namespace Ui {
class editformResizeElements;
}

class editformResizeElements : public QWidget
{
    Q_OBJECT
    
public:
    explicit editformResizeElements(QWidget *parent = 0);
    ~editformResizeElements();
    void loadlist(QStringList list,QString folder, int current);
    int getLastIndx();
    bool isSameFolder();

signals:
    void editFinished(bool);

private slots:
    void on_typeSizeRadioButton_clicked();
    void on_typePercentRadioButton_clicked();
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::editformResizeElements *ui;
    QStringList list;
    QString folder;
    int last_element_indx;
    QGraphicsScene * scene;
    QPixmap pixmap;
    bool samefolder;
};

#endif // EDITFORMRESIZEELEMENTS_H
