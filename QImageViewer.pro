
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QImageViewer
TEMPLATE = app
unix {
    target.path = /usr/local/bin
}
# Input

INSTALLS += target

HEADERS += editformcrop.h \
           editformcropimage.h \
           editformresize.h \
           fullscreen.h \
           image.h \
           qimageviewer.h \
           settings.h \
    preview.h \
    previewlist.h
           
FORMS += editformcrop.ui \
         editformresize.ui \
         fullscreen.ui \
         properties.ui \
         qimageviewer.ui \
         settings.ui
         
SOURCES += editformcrop.cpp \
           editformcropimage.cpp \
           editformresize.cpp \
           fullscreen.cpp \
           main.cpp \
           qimageviewer.cpp \
           settings.cpp \
           image.cpp \
           image_edit.cpp \
    image_events.cpp \
    preview.cpp \
    previewlist.cpp

RESOURCES += resources.qrc


TRANSLATIONS += lng/qimageviewer_en.ts \
                lng/qimageviewer_ru.ts \
                lng/qimageviewer_uk.ts

win32 {
#OS WIN
RC_FILE = myapp.rc
 }

OTHER_FILES += \
    res/zoom-out.png \
    res/zoom-in.png \
    res/slideshow.png \
    res/rotate-right.png \
    res/rotate-left.png \
    res/resize.png \
    res/prev.png \
    res/next.png \
    res/fullscreen.png \
    res/format-tiff.png \
    res/format-png.png \
    res/format-jpg.png \
    res/format-jpeg.png \
    res/format-gif.png \
    res/format-bmp.png \
    res/file-save.png \
    res/file-properties.png \
    res/file-open.png \
    res/delete.png \
    res/crop.png \
    lng/qimageviewer_ua.ts \
    lng/qimageviewer_ua.qm \
    lng/qimageviewer_ru.ts \
    lng/qimageviewer_ru.qm \
    lng/qimageviewer_en.ts \
    lng/qimageviewer_en.qm \
    myapp.rc
