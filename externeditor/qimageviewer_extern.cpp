#include "qimageviewer.h"
#include "ui_qimageviewer.h"

#ifdef Q_OS_WIN32
QStringList GetArchivesList(QString dir_name)
{
    QStringList ret_list;
    QDir dir(dir_name);
    QFileInfoList info_list = dir.entryInfoList();
    if(info_list.size() > 2)
    {
        QList<QFileInfo>::iterator iter=info_list.begin();
        QString path;
        for(iter=info_list.begin()+2;iter != info_list.end();iter++)
        {
            path = iter->absoluteFilePath();
            if(iter->isDir()) ret_list += GetArchivesList(path);
            else if (path.endsWith(".lnk",Qt::CaseInsensitive)) ret_list.append(path);
        }
    }
    return ret_list;
}
#endif
void QImageViewer::makeInstalledSoftList()
{
#ifdef Q_OS_LINUX
    QDir dir;
    dir.setPath("/usr/share/applications/");
    QStringList filter("*.desktop");
    QStringList applications = dir.entryList(filter,QDir::NoFilter,QDir::SortByMask);
    for (int i=0;i<applications.size();i++)
    {
        applications[i] = applications[i].left( applications[i].size() - 8 );
        QString iconpath;
        if (QFile("/usr/share/icons/hicolor/32x32/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/32x32/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/36x36/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/36x36/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/48x48/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/48x48/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/64x64/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/64x64/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/72x72/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/72x72/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/96x96/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/96x96/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/24x24/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/24x24/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/22x22/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/22x22/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/icons/hicolor/16x16/apps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/icons/hicolor/16x16/apps/"+applications[i]+".png";
        else if (QFile("/usr/share/pixmaps/"+applications[i]+".png").exists())
            iconpath = "/usr/share/pixmaps/"+applications[i]+".png";

        // Application is GUI and has icon
        if (!iconpath.isEmpty())
        {
            // Read *.desktop file //
            QFile file("/usr/share/applications/"+applications[i]+".desktop");
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            QString desktopFile;
            while (!stream.atEnd()) desktopFile += stream.readLine() + "\n";

            // Application is graphical application //
            if (desktopFile.contains("Graphics"))
            {
                QStringList data = desktopFile.split("\n");

                // Parse name //
                QString name = "";
                foreach (QString str, data)
                {
                    if (str.startsWith("Name="))
                    {
                        name = str.right(str.size()-5);
                        break;
                    }
                }
                // Parse command //
                QString command = "";
                foreach (QString str, data)
                {
                    if (str.startsWith("Exec="))
                    {
                        command = str.right(str.size()-5);
                        break;
                    }
                }
                command = command.left(command.size() - 3);

                // Confirm new application
                installedSoft.append(new QExternProgram(name,QIcon(QPixmap(iconpath)),command,imagewidget));
            }
        }
    }

#endif
#ifdef Q_OS_WIN32
    // CURRENT USER APPLICATIONS //
    // Get links
    QString userApplications = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation)[0];
    QStringList targets = GetArchivesList(userApplications);
    QFileIconProvider iconGetter;

    // Check all links
    foreach (QString app, targets)
    {
        // Parse lnk
        QString str = QFile::symLinkTarget(app);
        if (str.endsWith(".exe",Qt::CaseInsensitive)
                && !str.contains("unins",Qt::CaseInsensitive)
                && !str.contains("cmd.exe",Qt::CaseInsensitive)
                && !str.contains("Example",Qt::CaseInsensitive))
        {
            // Get icon from exe
            QIcon icon = iconGetter.icon(QFileInfo(str));

            // Parse name
            int lastSlash = 0;
            for (int i=0;i<app.size();i++) if (app[i] == '/') lastSlash = i+1;
            app = app.right(app.size()-lastSlash);
            app = app.left(app.size()-4);

            installedSoft.append(new QExternProgram(app,icon,str,imagewidget));
        }
    }
    // ALL USERS APPLICATIONS //
    int secondSlash = 0;
    int thirdSlash = 0;
    int slashAmount = 0;
    //find second slash
    for (int i=0;i<userApplications.size();i++)
    {
        if (userApplications[i] == '/')
        {
            secondSlash = i+1;
            slashAmount++;
        }
        if (slashAmount == 2) break;
    }
    //find third slash
    for (int i=secondSlash+1;i<userApplications.size();i++)
        if (userApplications[i] == '/')
        {
            thirdSlash = i;
            break;
        }
    QString allUsersApplications = userApplications.left(secondSlash) +
            "All Users" + userApplications.right(userApplications.size() - thirdSlash);

    targets = GetArchivesList(allUsersApplications);

    // Check all links
    foreach (QString app, targets)
    {
        // Parse lnk
        QString str = QFile::symLinkTarget(app);
        if (str.endsWith(".exe",Qt::CaseInsensitive)
                && !str.contains("unins",Qt::CaseInsensitive)
                && !str.contains("cmd.exe",Qt::CaseInsensitive)
                && !str.contains("Example",Qt::CaseInsensitive))
        {
            // Get icon from exe
            QIcon icon = iconGetter.icon(QFileInfo(str));

            // Parse name
            int lastSlash = 0;
            for (int i=0;i<app.size();i++) if (app[i] == '/') lastSlash = i+1;
            app = app.right(app.size()-lastSlash);
            app = app.left(app.size()-4);

            installedSoft.append(new QExternProgram(app,icon,str,imagewidget));
        }
    }

#endif

}

void QImageViewer::newExternEditor()
{
    if (!isEditorAddFormActive)
    {
        if (installedSoft.isEmpty()) makeInstalledSoftList();
        editorAddForm = new QExternProgramAddForm(installedSoft);
        editorAddForm->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ExternEditorNew"])));
        connect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
        connect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
        editorAddForm->show();
        isEditorAddFormActive = true;
    }
}

void QImageViewer::addEditor(QString name, QIcon icon, QString command)
{
    QExternProgram *editor = new QExternProgram(name,icon,command,imagewidget);
    editors.append(editor);
    QAction * action = new QAction(icon,name,this);
    editorsActions.append(action);
    connect(action,SIGNAL(triggered()),editor,SLOT(exec()));
    ui->menuExtern_editors->insertAction(ui->editorsNewAction,action);
    if (!imagewidget->isReady()) action->setEnabled(false);

    QString dir;
#ifdef Q_OS_LINUX
    dir = QDir::homePath()+"/.config/QImageViewer/extern/";
#endif
#ifdef Q_OS_WIN32
    dir = QApplication::applicationDirPath()+"\\extern\\";
#endif
    if (!QDir(dir).exists())
    {
        QDir creator;
        creator.mkdir(dir);
    }
    QFile file(dir+name+".png");
    file.open(QIODevice::WriteOnly);
    QPixmap pixmap = icon.pixmap(32,32);
    pixmap.save(&file, QString("PNG").toStdString().c_str());

    qDebug() << name << icon <<command;
    disconnect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));
    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}

void QImageViewer::abortAddingNewExternEditor()
{
    disconnect(editorAddForm,SIGNAL(accept(QString,QIcon,QString)),this,SLOT(addEditor(QString,QIcon,QString)));
    disconnect(editorAddForm,SIGNAL(cancel()),this,SLOT(abortAddingNewExternEditor()));

    editorAddForm->close();
    delete editorAddForm;
    isEditorAddFormActive = false;
}

/** Show Extern editors manager **/
void QImageViewer::exterEditorsManager()
{
    if (!isEditosManagerActive)
    {
        isEditosManagerActive = true;
        if (installedSoft.isEmpty()) makeInstalledSoftList();
        editorsManager = new QExternProgramManager(&installedSoft, editors,imagewidget,iconpacks[currenticonpack],icon);
        editorsManager->setWindowIcon(QIcon(QPixmap(iconpacks[currenticonpack] + icon["ExternEditorsManager"])));
        connect(editorsManager,SIGNAL(overed(bool)),this,SLOT(exterEditorsManagerOvered(bool)));

        editorsManager->show();
    }
}

/** Close Extern editors manager **/
void QImageViewer::exterEditorsManagerOvered(bool result)
{
    isEditosManagerActive = false;

    if (result)
    {
        // Erase old links//
        for (int i=0;i<editorsActions.size();i++)
        {
            ui->menuExtern_editors->removeAction(editorsActions[i]);
            disconnect(editorsActions[i],SIGNAL(triggered()),editors[i],SLOT(exec()));
        }
        editorsActions.clear();

        editors = editorsManager->editors;
        // Add new links //
        for (int i=0;i<editors.size();i++)
        {
            QAction * action = new QAction(editors[i]->icon,editors[i]->name,this);
            editorsActions.append(action);
            connect(action,SIGNAL(triggered()),editors[i],SLOT(exec()));
            ui->menuExtern_editors->insertAction(ui->editorsNewAction,action);
            if (!imagewidget->isReady()) action->setEnabled(false);
        }
    }
    editorsManager->close();
    disconnect(editorsManager,SIGNAL(overed(bool)),this,SLOT(exterEditorsManagerOvered(bool)));
    delete editorsManager;
}
