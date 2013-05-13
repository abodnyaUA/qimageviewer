#ifndef HOTKEYS_H
#define HOTKEYS_H
#include <QString>

struct hotkeysStruct
{
    QString fileOpen,fileSave,fileSaveAs,
            fileSettings,fileQuit;
    QString editUndo,editRedo,
            editRotateLeft,editRotateRight,
            editFlipHorizontal,editFlipVertical,
            editCrop,editResize,editResizeItems;
    QString watchNext,watchPrevious,
            watchFullscreen,watchSlideshow,
            watchWallpaper,zoomIn,zoomOut,
            zoomWindow,zoomOriginal;
    QString helpAbout;
};

#endif // HOTKEYS_H
