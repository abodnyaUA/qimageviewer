#include "qimageviewer.h"
#include "ui_qimageviewer.h"

void QImageViewer::checkupdates()
{
    if (isUpdateDialogRunning) return;
    isUpdateDialogRunning = true;
    updater = new QNetworkAccessManager;
    updater->get(QNetworkRequest(QUrl("http://qiv.p.ht/version.php?notfuckyou=true")));
    updater->connect(updater,SIGNAL(finished(QNetworkReply*)),this,SLOT(getUpdates(QNetworkReply*)));
}

#ifdef Q_OS_WIN32
QByteArray md5 (const QByteArray& data)
{
    return QCryptographicHash::hash(data,QCryptographicHash::Md5).toHex();
}

QStringList getDirFiles(QString dir_name)
{
    QStringList ret_list;
    QDir dir(dir_name);
    QFileInfoList info_list = dir.entryInfoList();
    QList<QFileInfo>::iterator iter=info_list.begin();
    QString path;
    for(iter=info_list.begin();iter != info_list.end();iter++)
    {
        path = iter->absoluteFilePath();
        if(iter->isDir() && !path.endsWith("\\..") && !path.endsWith("\\.")) ret_list += getDirFiles(path);
        else ret_list.append(path);
    }
    return ret_list;
}
#endif

void QImageViewer::getUpdates(QNetworkReply* reply)
{
    needToUpdate.clear();
    // Check request
    QByteArray replyContent = reply->readAll();
    if (replyContent == "Invalid request")
    {
        qDebug() << "Invalid request";
        return;
    }

    // Parse answer
    QVariant response = JSON::parse(replyContent);
    // Last avaiable version
    QString lastVersion(response.toMap()["current_version"].toString());
    qDebug() << lastVersion;

    // If it's last version, show message;
    QString currentVersion = QApplication::applicationVersion();
    if (lastVersion <= currentVersion)
    {
        isUpdateDialogRunning = false;
        if (!automateUpdate) QMessageBox::information(this,tr("Last version"),tr("You have last version"),
                                 QMessageBox::Ok);
        return;
    }

    // Get last version

#ifdef Q_OS_WIN32
    // All files for windows version
    QStringList updateFiles(response.toMap()["files"].toString().split(";"));
    updateFiles.removeLast();
    for (int i=0;i<updateFiles.size();i++) updateFiles[i] = updateFiles[i].right(updateFiles[i].size()-37);
    // Take map file:md5
    QMap<QString,QString> updateFilesMap;
    foreach (QString file, updateFiles)
    {
        QStringList detail = file.split(":");
        detail[0] = detail[0].toLower();
        updateFilesMap[detail[0]] = detail[1];
    }
    updateFiles = updateFilesMap.keys();
    qDebug() << "UPDATED:";
    foreach (QString file, updateFiles)
    {
        qDebug() << "file:"<<file;
        qDebug() << "md5: "<<updateFilesMap[file];
    }

    // Check current files
    existDir = QApplication::applicationDirPath();
    QStringList existFiles = getDirFiles(existDir);
    for (int i=0;i<existFiles.size();i++)
    {
        existFiles[i] = existFiles[i].right(existFiles[i].size()-existDir.size()-1);
        existFiles[i] = existFiles[i].toLower();
    }

    QMap<QString,QString> existFilesMap;
    foreach (QString file,existFiles)
    {
        QFile truefile(existDir+"\\"+file);
        truefile.open(QIODevice::ReadOnly);
        if (!truefile.exists()) qDebug() << "file "<<existDir+"\\"+file<<"is missing!!!!!!!!!!!!!";
        existFilesMap[file] = QString(md5(truefile.readAll()));
    }
    qDebug() << "EXIST:";
    foreach (QString file, existFiles)
    {
        qDebug() << "file:"<<file;
        qDebug() << "md5: "<<existFilesMap[file];
    }

    needToUpdate.clear();
    foreach (QString file, updateFiles)
    {
        if (existFilesMap[file] != updateFilesMap[file]) needToUpdate.append(file);
    }

    qDebug() << "===================================";
    if (needToUpdate.isEmpty())
    {
        qDebug() << "NOTHING TO UPDATE";
        return;
    }
    else foreach (QString file, needToUpdate) qDebug() << "Update: "<< file;
#endif
#ifdef Q_OS_LINUX
    existDir = QDir::homePath();
    if (typeOS == OS::DEBBasedLinux)
    {
        qDebug() << "http://qiv.p.ht/bin/"+response.toMap()["deb-package"].toString();
        needToUpdate.append(response.toMap()["deb-package"].toString());
    }
    if (typeOS == OS::RPMBasedLinux)
    {
        qDebug() << "http://qiv.p.ht/bin/"+response.toMap()["rpm-package"].toString();
        needToUpdate.append(response.toMap()["rpm-package"].toString());
    }
    if (typeOS == OS::ArchLinux)
    {
        qDebug() << "https://aur.archlinux.org/packages/qi/qimageviewer-git/qimageviewer-git.tar.gz";
        needToUpdate.append("qimageviewer-git.tar.gz");
    }
#endif
#ifdef Q_OS_MAC
    qDebug() << "http://qiv.p.ht/bin/"+response.toMap()["mac-app"].toString();
    needToUpdate.append(response.toMap()["mac-app"].toString());
#endif
    // Get changelog
    QNetworkAccessManager * m_NetworkMngr = new QNetworkAccessManager(this);
    QNetworkReply *replyChangelog = m_NetworkMngr->get(
                QNetworkRequest(QUrl("http://qiv.p.ht/changelog.txt")));
    QEventLoop loop;
    connect(replyChangelog, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();
    QString changelog(replyChangelog->readAll());

    // Ask about update
    if (UpdateDialog(this,currentVersion,lastVersion,changelog).exec())
    {
        qDebug() << "Okey, lets start";
        QDir().mkdir(existDir+"/update");
        updateInformer = new UpdateInformer;
        updateInformer->show();
        updateFile(0);
    } else isUpdateDialogRunning = false;

    automateUpdate = false;
    delete updater;
}

void QImageViewer::updateFile(int number)
{
    updateInformer->setProgress((double)number / (double)needToUpdate.size(),needToUpdate[number]);
    QNetworkAccessManager * m_NetworkMngr = new QNetworkAccessManager(this);
#ifdef Q_OS_WIN32
    QNetworkReply *reply = m_NetworkMngr->get(
                QNetworkRequest(QUrl("http://qiv.p.ht/windows/"+needToUpdate[number])));
#endif
#ifdef Q_OS_LINUX
    QNetworkReply *reply;
    if (typeOS == OS::ArchLinux)
    {
        reply = m_NetworkMngr->get(QNetworkRequest(QUrl(
                 "https://aur.archlinux.org/packages/qi/qimageviewer-git/"+needToUpdate[number])));
    }
    else
    {
        reply = m_NetworkMngr->get(
                    QNetworkRequest(QUrl("http://qiv.p.ht/bin/"+needToUpdate[number])));
    }
#endif
#ifdef Q_OS_MAC
    QNetworkReply *reply;
    reply = m_NetworkMngr->get(
                QNetworkRequest(QUrl("http://qiv.p.ht/bin/"+needToUpdate[number])));
#endif
    QEventLoop loop;
    connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();

    if (needToUpdate[number].contains('/'))
    {
        int slashPos;
        for (int i=needToUpdate[number].size()-1;i>0;i--)
            if (needToUpdate[number][i] == '/')
            {
                slashPos = i;
                break;
            }
        QDir().mkdir(existDir+"/update/"+needToUpdate[number].left(slashPos));
    }
    QFile file(existDir+"/update/"+needToUpdate[number]);
    if (!file.open(QIODevice::WriteOnly)) qDebug() <<"Can't open file";
    file.write(reply->readAll());
    file.flush();
    file.close();
    delete reply;
    if (number < needToUpdate.size()-1) updateFile(number+1);
    else afterUpdates();
}

void QImageViewer::afterUpdates()
{
    qDebug() << "Updating finished";
    updateInformer->close();
    delete updateInformer;
    isUpdateDialogRunning = false;
#ifdef Q_OS_WIN32
    QFile updater(existDir+"\\updater.bat");
    updater.open(QIODevice::WriteOnly);
    QTextStream out(&updater);
    // Clear console
    out << "@echo off\n";
    out << "prompt Please wait some seconds ;)\n";
    // Delay for closing application
    out << "ping -n 3 127.0.0.1 > NUL\n";
    // moving all new files
    foreach(QString file, needToUpdate)
    {
        file.replace('/','\\');
        out << "move /y ";
        out << '"' << existDir+"\\update\\"+file<<'"';
        out << " ";
        out << '"' << existDir+"\\"+file<<'"';
        out << "\n";
    }
    // clear temp files
    out << "rd /s /q " << '"' << existDir+"\\update\\" << '"' << "\n";
    // run new version
    out << '"'<<QApplication::applicationDirPath()+"\\QImageViewer.exe"<<'"' << "\n";
    // delete temp 'bat' updater
    out << "del "<< '"' << existDir+"\\updater.bat" << '"';
    updater.flush();
    updater.close();
    QProcess::startDetached("cmd.exe", QStringList() << "/c" << existDir+"\\updater.bat");
    imagewidget->setSaved();
    close();
#endif
#ifdef Q_OS_LINUX
    QFile updater(existDir+"/updater.sh");
    updater.open(QIODevice::WriteOnly);
    QTextStream out(&updater);
    out << "#!/bin/sh\n";
    out << "sleep 3\n";
    if (typeOS == OS::DEBBasedLinux)
    {
        if (QFile("/usr/bin/gksu").exists())
            out << "gksu "<< '"' << "gdebi -n " << existDir+"/update/"+needToUpdate[0]<< '"' << "\n";
        else if (QFile("/usr/bin/kdesu").exists())
            out << "kdesu "<< '"' << "gdebi -n " << existDir+"/update/"+needToUpdate[0]<< '"' << "\n";
    }
    if (typeOS == OS::ArchLinux)
    {
        out << "cd "+existDir+"/update/\n";
        out << "tar -xvf qimageviewer-git.tar.gz\n";
        out << "cd qimageviewer-git\n";
        out << "makepkg\n";
        if (QFile("/usr/bin/gksu").exists())
            out << "gksu "<< '"' << "pacman -U qimageviewer-git.tar.gz.hz"<< '"' << "\n";
        else if (QFile("/usr/bin/kdesu").exists())
            out << "kdesu "<< '"' << "pacman -U qimageviewer-git.tar.gz.hz"<< '"' << "\n";
    }
    if (typeOS == OS::RPMBasedLinux)
        out << "beesu "<< '"' << "rpm -Uvh "<< existDir+"/update/"+needToUpdate[0]<< '"' << "\n";
    out << "qimageviewer\n";
    out << "rm "<< '"' << existDir+"/updater.sh" << '"' << "\n";
    out << "rm -rf "<< '"' << existDir+"/update"<< '"' << "\n";
    updater.flush();
    updater.close();
    QProcess::startDetached("bash", QStringList() << existDir+"/updater.sh");
    imagewidget->setSaved();
    close();
#endif
}
