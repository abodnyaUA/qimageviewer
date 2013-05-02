#ifndef fullscreen_H
#define fullscreen_H

#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>

namespace Ui {
class fullscreen;
}

class fullscreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit fullscreen(QWidget *parent = 0);
    ~fullscreen();
    QVBoxLayout *lay;

signals:
    void fullscreenEnded();
    void needResize();
    void needCrop();
    void needSave();
    void needSaveAs();
    void needOpen();
    void needExit();

private:
    Ui::fullscreen *ui;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // fullscreen_H
