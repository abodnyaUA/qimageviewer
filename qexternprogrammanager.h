#ifndef QEXTERNPROGRAMMANAGER_H
#define QEXTERNPROGRAMMANAGER_H

#include <QWidget>
#include <QEvent>
#include <QList>
#include <QListWidgetItem>
#include <QCloseEvent>
#include "qexternprogram.h"
#include "qexternprogramaddform.h"

namespace Ui {
class QExternProgramManager;
}

class QExternProgramManager : public QWidget
{
    Q_OBJECT
    
public:
    explicit QExternProgramManager(QList<QExternProgram *> editors, image * imagewidget,
                                   QString theme, QMap<QString, QString> icon);
    ~QExternProgramManager();
    QList<QExternProgram *> editors;
    
private slots:
    void on_updateButton_clicked();
    void on_removeButton_clicked();
    void on_addButton_clicked();
    void on_okButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_iconBrowseButton_clicked();
    void on_commandBrowseButton_clicked();

    //Adding new
    void newExternEditor();
    void addEditor(QString name, QString icon, QString command);
    void abortAddingNewExternEditor();

    void on_cancelButton_clicked();

    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::QExternProgramManager *ui;
    QList<QListWidgetItem *> editorsItems;
    QExternProgramAddForm * editorAddForm;
    bool isEditorAddFormActive;
    image * imagewidget;
    bool wasEmit;

signals:
    void overed(bool);

protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *, QEvent *);
};

#endif // QEXTERNPROGRAMMANAGER_H
