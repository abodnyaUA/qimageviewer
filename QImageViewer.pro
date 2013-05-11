
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
    previewlist.h \
    qhotkeyedit.h \
    editformresizeelements.h
           
FORMS += editformcrop.ui \
         editformresize.ui \
         fullscreen.ui \
         properties.ui \
         qimageviewer.ui \
         settings.ui \
    editformresizeelements.ui
         
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
    previewlist.cpp \
    qhotkeyedit.cpp \
    editformresizeelements.cpp

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
    myapp.rc \
    android/version.xml \
    android/res/values-ru/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-et/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/values-es/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/values-ja/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-rs/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    res/help.png \
    lng/qimageviewer_uk.ts \
    lng/qimageviewer_uk.qm \
    res/settings.png \
    res/flag-usa.png \
    res/flag-ukr.png \
    res/flag-rus.png \
    res/flag-sys.png \
    res/zoom-window.png \
    res/zoom-original.png \
    res/resize-items.png \
    res/flip-vertical.png \
    res/flip-horizontal.png \
    res/wallpaper.png
