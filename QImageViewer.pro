
QT       += core gui network webkitwidgets

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
    editformresizeelements.h \
    qhotkeywidget.h \
    hotkeys.h \
    qexternprogram.h \
    qexternprogramaddform.h \
    qexternprogrammanager.h \
    imageshackuploader.h \
    imageshacklistupload.h \
    imageshacklistview.h \
    qvk/reply.h \
    qvk/qvk.h \
    qvk/json.h \
    qvk/vkuploadimageform.h \
    qvk/vkuploadimagesform.h
           
FORMS += editformcrop.ui \
         editformresize.ui \
         fullscreen.ui \
         properties.ui \
         qimageviewer.ui \
         settings.ui \
    editformresizeelements.ui \
    qexternprogramaddform.ui \
    qexternprogrammanager.ui \
    imageshackuploader.ui \
    imageshacklistupload.ui \
    imageshacklistview.ui \
    qvk/vkuploadimageform.ui \
    qvk/vkuploadimagesform.ui
         
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
    editformresizeelements.cpp \
    qhotkeywidget.cpp \
    qimageviewer_design.cpp \
    qexternprogram.cpp \
    qexternprogramaddform.cpp \
    qexternprogrammanager.cpp \
    imageshackuploader.cpp \
    settings_hotkeys.cpp \
    imageshacklistupload.cpp \
    imageshacklistview.cpp \
    qvk/reply.cpp \
    qvk/qvk.cpp \
    qvk/json.cpp \
    qimageviewer_vk.cpp \
    qvk/vkuploadimageform.cpp \
    qvk/vkuploadimagesform.cpp

RESOURCES += resources.qrc


TRANSLATIONS += lng/qimageviewer_en.ts \
                lng/qimageviewer_ru.ts \
                lng/qimageviewer_uk.ts \
                lng/qimageviewer_pl.ts

win32 {
#OS WIN
RC_FILE = myapp.rc
 }

OTHER_FILES += \
    lng/qimageviewer_uk.ts \
    lng/qimageviewer_uk.qm \
    lng/qimageviewer_ru.ts \
    lng/qimageviewer_ru.qm \
    lng/qimageviewer_en.ts \
    lng/qimageviewer_en.qm \
    lng/qimageviewer_pl.ts \
    lng/qimageviewer_pl.qm \
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
    style.qss

QMAKE_CXXFLAGS += -std=c++11
