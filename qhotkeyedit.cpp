#include "qhotkeyedit.h"

QHotkeyEdit::QHotkeyEdit(QWidget *parent)
{
    this->installEventFilter(this);
}

QHotkeyEdit::QHotkeyEdit(QString text,QWidget *parent) :
    QLineEdit(parent)
{
    setText(text);
    this->installEventFilter(this);
}

bool QHotkeyEdit::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int keyInt = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(keyInt);

        if (key == Qt::Key_Escape) emit needDecline();
        else if (key == Qt::Key_Return) emit needAccept();
        else
        if (key != Qt::Key_Control && key != Qt::Key_Shift && key!= Qt::Key_Backspace &&
                key != Qt::Key_Alt && key != Qt::Key_Meta && key != Qt::Key_unknown)
        {
            // check for a combination of user clicks
            Qt::KeyboardModifiers modifiers = keyEvent->modifiers();

            if(modifiers & Qt::ShiftModifier)
                keyInt += Qt::SHIFT;
            if(modifiers & Qt::ControlModifier)
                keyInt += Qt::CTRL;
            if(modifiers & Qt::AltModifier)
                keyInt += Qt::ALT;
            if(modifiers & Qt::MetaModifier)
                keyInt += Qt::META;

            QString textHotkey = QKeySequence(keyInt).toString(QKeySequence::PortableText);

            setMaxLength(20);
            setText( textHotkey );
            setMaxLength(textHotkey.size());
        }
    }
}
