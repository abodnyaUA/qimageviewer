#ifndef QEXTERNPROGRAMADDFORM_H
#define QEXTERNPROGRAMADDFORM_H

#include <QWidget>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include "qexternprogram.h"

namespace Ui {
class QExternProgramAddForm;
}

class QExternProgramAddForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit QExternProgramAddForm(QList<QExternProgram *> installedSoft);
    ~QExternProgramAddForm();
    
private:
    Ui::QExternProgramAddForm *ui;
    QString name,command;
    QIcon icon;

signals:
    void accept(QString name, QIcon icon, QString command);
    void cancel();

private slots:
    void on_iconBrowseButton_clicked();
    void on_commandBrowseButton_clicked();
    void on_cancelButton_clicked();
    void on_addButton_clicked();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // QEXTERNPROGRAMADDFORM_H
