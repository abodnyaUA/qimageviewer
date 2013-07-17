#include "os_application.h"
OS_application::OS_application( int &argc, char **argv )
  : QApplication(argc, argv)
{
        /* QSetting setup to access */
    window = NULL;
    setOrganizationName("QImageViewer");
    setApplicationName("QImageViewer");
}

void OS_application::setWindow(QImageViewer *window)
{
    if (window) this->window = window;
}

bool OS_application::event(QEvent *event)
{
    if (event->type() == QEvent::FileOpen)
    {
        QFileOpenEvent * filevent = static_cast<QFileOpenEvent *>(event);
        osfile = filevent->file();
        if (window) window->openExternFile(osfile);
        return true;
    }
    else return QApplication::event(event);
}
QString OS_application::file()
{
    return osfile;
}
