
QT       += core gui network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qimageviewer
TEMPLATE = app
unix {
    isEmpty(PREFIX)
    {
        PREFIX = /usr
    }
    target.path = $$PREFIX/bin

    desktop.files = qimageviewer.desktop
    desktop.path =  $$PREFIX/share/applications

    target1.files = res/icon.png
    target1.path =  $$PREFIX/share/pixmaps

    icon_32.files =  res/icon_32-32.png
    icon_64.files =  res/icon.png
    icon_32.path =  $$PREFIX/share/icons/hicolor/32x32/apps
    icon_64.path =  $$PREFIX/share/icons/hicolor/64x64/apps

    INSTALLS += target desktop target1
    INSTALLS += icon_32 icon_64
}
# Input

HEADERS += \
    fullscreen.h \
    image.h \
    qimageviewer.h \
    preview.h \
    previewlist.h \
    edit/editformcrop.h \
    edit/editformcropimage.h \
    edit/editformresize.h \
    edit/editformresizeelements.h \
    settings/settings.h \
    settings/hotkeys.h \
    settings/qhotkeyedit.h \
    settings/qhotkeywidget.h \
    externeditor/qexternprogram.h \
    externeditor/qexternprogramaddform.h \
    externeditor/qexternprogrammanager.h \
    imageshack/imageshackuploader.h \
    imageshack/imageshacklistupload.h \
    imageshack/imageshacklistview.h \
    imageshack/uploadtoimageshack.h \
    qvk/reply.h \
    qvk/qvk.h \
    qvk/json.h \
    qvk/vkuploadimageform.h \
    qvk/vkuploadimagesform.h \
    qvk/vkdownloadalbumform.h \
    aboutform.h
           
FORMS += \
    fullscreen.ui \
    properties.ui \
    qimageviewer.ui \
    settings/settings.ui \
    edit/editformcrop.ui \
    edit/editformresize.ui \
    edit/editformresizeelements.ui \
    externeditor/qexternprogramaddform.ui \
    externeditor/qexternprogrammanager.ui \
    imageshack/imageshackuploader.ui \
    imageshack/imageshacklistupload.ui \
    imageshack/imageshacklistview.ui \
    qvk/vkuploadimageform.ui \
    qvk/vkuploadimagesform.ui \
    qvk/vkdownloadalbumform.ui \
    aboutform.ui
         
SOURCES +=  \
    edit/editformcrop.cpp \
    edit/editformcropimage.cpp \
    edit/editformresize.cpp \
    edit/image_edit.cpp \
    edit/editformresizeelements.cpp \
    fullscreen.cpp \
    main.cpp \
    qimageviewer.cpp \
    image.cpp \
    image_events.cpp \
    preview.cpp \
    previewlist.cpp \
    externeditor/qexternprogram.cpp \
    externeditor/qexternprogramaddform.cpp \
    externeditor/qexternprogrammanager.cpp \
    settings/settings.cpp \
    settings/qimageviewer_design.cpp \
    settings/qhotkeyedit.cpp \
    settings/qhotkeywidget.cpp \
    settings/settings_hotkeys.cpp \
    qvk/reply.cpp \
    qvk/qvk.cpp \
    qvk/json.cpp \
    qvk/vkuploadimageform.cpp \
    qvk/vkuploadimagesform.cpp \
    qvk/vkdownloadalbumform.cpp \
    qvk/qimageviewer_vk.cpp \
    imageshack/uploadtoimageshack.cpp \
    imageshack/imageshacklistupload.cpp \
    imageshack/imageshacklistview.cpp \
    imageshack/imageshackuploader.cpp \
    imageshack/qimageviewer_imageshack.cpp \
    aboutform.cpp


RESOURCES += resources.qrc


TRANSLATIONS += lng/qimageviewer_en.ts \
                lng/qimageviewer_ru.ts \
                lng/qimageviewer_uk.ts \
                lng/qimageviewer_cz.ts

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
    style.qss \
    lng/qimageviewer_cz.ts \
    lng/qimageviewer_cz.qm \
    qimageviewer.desktop

QMAKE_CXXFLAGS += -std=c++11
