#ifndef QEXTERNPROGRAMADDFORM_H
#define QEXTERNPROGRAMADDFORM_H

#include <QWidget>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class QExternProgramAddForm;
}

class QExternProgramAddForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit QExternProgramAddForm(QWidget *parent = 0);
    ~QExternProgramAddForm();
    
private:
    Ui::QExternProgramAddForm *ui;

signals:
    void accept(QString name, QString icon, QString command);
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
