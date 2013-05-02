#include "fullscreen.h"
#include "ui_fullscreen.h"

fullscreen::fullscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fullscreen)
{
    ui->setupUi(this);
    lay = ui->verticalLayout;
    this->installEventFilter(this);
}
#include <QDebug>
/** close fullscreen with ESC or F10 key **/
bool fullscreen::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int k = keyEvent->key();
        //'ESC' KEY
        if ((int)k == (int)Qt::Key_Escape || (int)k == (int)Qt::Key_F10)
        {
            emit fullscreenEnded();
            return true;
        }
        //'Crtl+R' KEY
        if ((int)k == (int)(Qt::CTRL + Qt::Key_R))
        {
            emit fullscreenEnded();
            return true;
        }
        return false;
    }
    return false;
}

fullscreen::~fullscreen()
{
    delete ui;
}
