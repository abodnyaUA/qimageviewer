#include "settings.h"
#include "ui_settings.h"

QTreeWidgetItem * Settings::addCategory(QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->hotkeyWidget);
    item->setText(0,name);
    ui->hotkeyWidget->addTopLevelItem(item);
    return item;
}

QTreeWidgetItem * Settings::addHotkey(QTreeWidgetItem * category, QString name, QString hotkey, QString iconkey, QString defaultHotkey)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(category);
    item->setText(0,name);
    item->setText(1,hotkey);
    item->setIcon(0,QIcon(QPixmap(iconpacks[old_currenticonpack] + icon[iconkey])));
    item->setWhatsThis(2,defaultHotkey);
    category->addChild(item);
    return item;
}

void Settings::setHotkeys()
{
    ui->hotkeyWidget->setColumnWidth(0,300);
    QStringList ColumnNames;
    ColumnNames << tr("Command") << tr("Hotkey");
    ui->hotkeyWidget->setHeaderLabels(ColumnNames);

    QHotkeyWidget = new qhotkeywidget;
    connect(QHotkeyWidget,SIGNAL(butOKClicked()),this,SLOT(changingEndAccept()));
    connect(QHotkeyWidget,SIGNAL(butCancelClicked()),this,SLOT(changingEndDecline()));
    QHotkeyWidget->isChanging = false;

    categoryFile = addCategory(tr("File"));
    categoryFile->setExpanded(true);
    hotkeyItemFileOpen = addHotkey(categoryFile,tr("Open image"),old_hotkeys.fileOpen,"FileOpen","Ctrl+O");
    hotkeyItemFileSave = addHotkey(categoryFile,tr("Save"),old_hotkeys.fileSave,"FileSave","Ctrl+S");
    hotkeyItemFileSaveAs = addHotkey(categoryFile,tr("Save as..."),old_hotkeys.fileSaveAs,"","Ctrl+Shift+S");
    hotkeyItemFileSettings = addHotkey(categoryFile,tr("Settings"),old_hotkeys.fileSettings,"Settings","");
    hotkeyItemFileQuit = addHotkey(categoryFile,tr("Quit"),old_hotkeys.fileQuit,"","Ctrl+Q");
    allactions << hotkeyItemFileOpen << hotkeyItemFileSave << hotkeyItemFileSaveAs <<
                      hotkeyItemFileSettings << hotkeyItemFileQuit;

    categoryEdit = addCategory(tr("Edit"));
    categoryEdit->setExpanded(true);
    hotkeyItemEditUndo = addHotkey(categoryEdit,tr("Undo"),old_hotkeys.editUndo,"","Ctrl+Z");
    hotkeyItemEditRedo = addHotkey(categoryEdit,tr("Redo"),old_hotkeys.editRedo,"","Ctrl+Shift+Z");
    hotkeyItemEditRotateRight = addHotkey(categoryEdit,tr("Rotate to the right"),old_hotkeys.editRotateRight,"RotateRight","Ctrl+Shift+L");
    hotkeyItemEditRotateLeft = addHotkey(categoryEdit,tr("Rotate to the left"),old_hotkeys.editRotateLeft,"RotateLeft","Ctrl+Shift+R");
    hotkeyItemEditFlipHorizontal = addHotkey(categoryEdit,tr("Flip horizontal"),old_hotkeys.editFlipHorizontal,"FlipHorizontal","");
    hotkeyItemEditFlipVertical = addHotkey(categoryEdit,tr("Flip vertical"),old_hotkeys.editFlipVertical,"FlipVertical","");
    hotkeyItemEditCrop = addHotkey(categoryEdit,tr("Crop"),old_hotkeys.editCrop,"Crop","Ctrl+Shift+C");
    hotkeyItemEditResize = addHotkey(categoryEdit,tr("Resize"),old_hotkeys.editResize,"Resize","Ctrl+R");
    hotkeyItemEditResizeItems = addHotkey(categoryEdit,tr("Resize items..."),old_hotkeys.editResizeItems,"ResizeItems","Ctrl+Shift+R");
    allactions << hotkeyItemEditUndo << hotkeyItemEditRedo << hotkeyItemEditRotateLeft << hotkeyItemEditRotateRight <<
                      hotkeyItemEditFlipHorizontal << hotkeyItemEditFlipVertical << hotkeyItemEditCrop <<
                      hotkeyItemEditResize << hotkeyItemEditResizeItems;

    categoryWatch = addCategory(tr("Preview"));
    categoryWatch->setExpanded(true);
    hotkeyItemWatchPrevious = addHotkey(categoryWatch,tr("Previous image"),old_hotkeys.watchPrevious,"Previous","Ctrl+Left");
    hotkeyItemWatchNext = addHotkey(categoryWatch,tr("Next image"),old_hotkeys.watchNext,"Next","Ctrl+Right");
    hotkeyItemWatchFullscreen = addHotkey(categoryWatch,tr("Fullscreen"),old_hotkeys.watchFullscreen,"Fullscreen","F11");
    hotkeyItemWatchSlideshow = addHotkey(categoryWatch,tr("Slideshow"),old_hotkeys.watchSlideshow,"Slideshow","F5");
    hotkeyItemWatchWallpaper = addHotkey(categoryWatch,tr("Set as wallpaper"),old_hotkeys.watchWallpaper,"Wallpaper","");
    hotkeyItemZoomIn = addHotkey(categoryWatch,tr("Zoom in"),old_hotkeys.zoomIn,"ZoomIn","+");
    hotkeyItemZoomOut = addHotkey(categoryWatch,tr("Zoom out"),old_hotkeys.zoomOut,"ZoomOut","-");
    hotkeyItemZoomWindow = addHotkey(categoryWatch,tr("Window size"),old_hotkeys.zoomWindow,"ZoomWindow","");
    hotkeyItemZoomOriginal = addHotkey(categoryWatch,tr("Original size"),old_hotkeys.zoomOriginal,"ZoomOriginal","");
    allactions << hotkeyItemWatchPrevious << hotkeyItemWatchNext << hotkeyItemWatchFullscreen <<
                      hotkeyItemWatchSlideshow << hotkeyItemWatchWallpaper << hotkeyItemZoomIn <<
                      hotkeyItemZoomOut << hotkeyItemZoomWindow << hotkeyItemZoomOriginal;

    categoryHelp = addCategory(tr("Help"));
    categoryHelp->setExpanded(true);
    hotkeyItemHelpAbout = addHotkey(categoryHelp,tr("About"),old_hotkeys.helpAbout,"Help","F1");
    allactions << hotkeyItemHelpAbout;
}

void Settings::saveHotkeys()
{
    old_hotkeys.fileOpen = hotkeyItemFileOpen->text(1);
    old_hotkeys.fileSave = hotkeyItemFileSave->text(1);
    old_hotkeys.fileSaveAs = hotkeyItemFileSaveAs->text(1);
    old_hotkeys.fileSettings = hotkeyItemFileSettings->text(1);
    old_hotkeys.fileQuit = hotkeyItemFileQuit->text(1);
    old_hotkeys.editUndo = hotkeyItemEditUndo->text(1);
    old_hotkeys.editRedo = hotkeyItemEditRedo->text(1);
    old_hotkeys.editRotateLeft = hotkeyItemEditRotateLeft->text(1);
    old_hotkeys.editRotateRight = hotkeyItemEditRotateRight->text(1);
    old_hotkeys.editFlipHorizontal = hotkeyItemEditFlipHorizontal->text(1);
    old_hotkeys.editFlipVertical = hotkeyItemEditFlipVertical->text(1);
    old_hotkeys.editCrop = hotkeyItemEditCrop->text(1);
    old_hotkeys.editResize = hotkeyItemEditResize->text(1);
    old_hotkeys.editResizeItems = hotkeyItemEditResizeItems->text(1);
    old_hotkeys.watchPrevious = hotkeyItemWatchPrevious->text(1);
    old_hotkeys.watchNext = hotkeyItemWatchNext->text(1);
    old_hotkeys.watchFullscreen = hotkeyItemWatchFullscreen->text(1);
    old_hotkeys.watchSlideshow = hotkeyItemWatchSlideshow->text(1);
    old_hotkeys.watchWallpaper = hotkeyItemWatchWallpaper->text(1);
    old_hotkeys.zoomIn = hotkeyItemZoomIn->text(1);
    old_hotkeys.zoomOut = hotkeyItemZoomOut->text(1);
    old_hotkeys.zoomWindow = hotkeyItemZoomWindow->text(1);
    old_hotkeys.zoomOriginal = hotkeyItemZoomOriginal->text(1);
    old_hotkeys.helpAbout = hotkeyItemHelpAbout->text(1);
}

void Settings::on_hotkeyWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (QHotkeyWidget->isChanging == false && item != categoryFile &&
            item != categoryEdit && item != categoryWatch && item != categoryHelp &&
            !isActiveHotkeyWidget)
    {
        QHotkeyWidget->loadItem(item);
        QHotkeyWidget->show();
        QHotkeyWidget->isChanging = true;
    }
}

void Settings::changingEndAccept()
{
    QString newhotkey = QHotkeyWidget->hotkeyEdit->text();

    bool ok = true;

    QTreeWidgetItem *bad = 0;
    QTreeWidgetItem *current = QHotkeyWidget->changingItem;
    foreach (QTreeWidgetItem* item, allactions)
        if (item->text(1) == newhotkey && item != current)
        {
            ok = false;
            bad = item;
        }
    if (!ok)
    {
        int r = QMessageBox::warning(QHotkeyWidget,tr("This hotkey is reserved"),
                                     tr("Hotkey's already reserved to ")+
                                     "'" + bad->text(0) +"'\n"+
                                     tr("Do you want to set hotkey anyway?"),
                                     QMessageBox::Yes,
                                     QMessageBox::No | QMessageBox::Default);
        if (r == QMessageBox::Yes)
        {
            ok = true;
            bad->setText(1,"");
        }
    }

    if (ok)
    {
        current->setText(1,newhotkey);
        QHotkeyWidget->isChanging = false;
        QHotkeyWidget->close();
    }
}
void Settings::changingEndDecline()
{
    QHotkeyWidget->isChanging = false;
    QHotkeyWidget->close();
}

void Settings::on_resetHotkeysButton_clicked()
{
    hotkeyItemFileOpen->setText(1,"Ctrl+O");
    hotkeyItemFileSave->setText(1,"Ctrl+S");
    hotkeyItemFileSaveAs->setText(1,"Ctrl+Shift+S");
    hotkeyItemFileSettings->setText(1,"");
    hotkeyItemFileQuit->setText(1,"Ctrl+Q");

    hotkeyItemEditUndo->setText(1,"Ctrl+Z");
    hotkeyItemEditRedo->setText(1,"Ctrl+Shift+Z");
    hotkeyItemEditRotateRight->setText(1,"Ctrl+Shift+L");
    hotkeyItemEditRotateLeft->setText(1,"Ctrl+Shift+R");
    hotkeyItemEditFlipHorizontal->setText(1,"");
    hotkeyItemEditFlipVertical->setText(1,"");
    hotkeyItemEditCrop->setText(1,"Ctrl+Shift+C");
    hotkeyItemEditResize->setText(1,"Ctrl+R");
    hotkeyItemEditResizeItems->setText(1,"");

    hotkeyItemWatchPrevious->setText(1,"Ctrl+Left");
    hotkeyItemWatchNext->setText(1,"Ctrl+Right");
    hotkeyItemWatchFullscreen->setText(1,"F10");
    hotkeyItemWatchSlideshow->setText(1,"F5");
    hotkeyItemWatchWallpaper->setText(1,"");
    hotkeyItemZoomIn->setText(1,"+");
    hotkeyItemZoomOut->setText(1,"-");
    hotkeyItemZoomWindow->setText(1,"");
    hotkeyItemZoomOriginal->setText(1,"");

    hotkeyItemHelpAbout->setText(1,"F1");
}
