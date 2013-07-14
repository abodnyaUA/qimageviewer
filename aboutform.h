#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDate>
#if QT_VERSION < 0x050000
#include <QUrl>
#endif

namespace Ui {
class aboutForm;
}

class aboutForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit aboutForm(QWidget *parent = 0);
    ~aboutForm();
    
signals:
    void overed();
private slots:
    void on_pushButton_clicked();
    void on_label_7_linkActivated(const QString &link);
    void on_label_6_linkActivated(const QString &link);

private:
    Ui::aboutForm *ui;
    bool exit;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // ABOUTFORM_H
