######################################################################
#QImageViewer - Simple image viewer with edit functions.
#Copyright (C) 2013 Bodnya Alexey
#
#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
######################################################################

QT       += core gui network webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TEMPLATE = app
TARGET = qimageviewer
INCLUDEPATH += . settings edit externeditor imageshack qvk

unix {
    isEmpty(PREFIX)
    {
        PREFIX = /usr
    }
    target.path = $$PREFIX/bin

    desktop.files = qimageviewer.desktop
    desktop.path =  $$PREFIX/share/applications

    icon_32.files = res/icon/32/qimageviewer.png
    icon_64.files = res/icon/64/qimageviewer.png
    icon_32.path =  $$PREFIX/share/icons/hicolor/32x32/apps
    icon_64.path =  $$PREFIX/share/icons/hicolor/64x64/apps

    INSTALLS += target desktop
    INSTALLS += icon_32 icon_64
    QMAKE_CXXFLAGS = -std=c++11
}
win32 {
#OS WIN
RC_FILE = myapp.rc
    QMAKE_CXXFLAGS = -std=c++11
}
macx {
    ICON = res/icon.icns
    RC_FILE = res/icon.icns
    QMAKE_INFO_PLIST = info.plist
    #CONFIG-=app_bundle
    QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+
    CONFIG+=c++11
    HEADERS += os_application.h
    SOURCES += os_application.cpp
}

# Input
HEADERS += \
    aboutform.h \
    fullscreen.h \
    image.h \
    previewlist.h \
    qimageviewer.h \
    edit/editformcrop.h \
    edit/editformcropimage.h \
    edit/editformresize.h \
    edit/editformresizeelements.h \
    externeditor/qexternprogram.h \
    externeditor/qexternprogramaddform.h \
    externeditor/qexternprogrammanager.h \
    imageshack/imageshacklistupload.h \
    imageshack/imageshacklistview.h \
    imageshack/imageshackuploader.h \
    imageshack/uploadtoimageshack.h \
    qvk/json.h \
    qvk/qvk.h \
    qvk/reply.h \
    qvk/vkdownloadalbumform.h \
    qvk/vkuploadimageform.h \
    qvk/vkuploadimagesform.h \
    settings/hotkeys.h \
    settings/qhotkeyedit.h \
    settings/qhotkeywidget.h \
    settings/settings.h \
    update/updatedialog.h \
    update/updateinformer.h \
    previewmode.h \
    algorithms.h \
    edit/imagefilter.h \
    edit/editformfilters.h
FORMS += \
    aboutform.ui \
    fullscreen.ui \
    properties.ui \
    qimageviewer.ui \
    edit/editformcrop.ui \
    edit/editformresize.ui \
    edit/editformresizeelements.ui \
    externeditor/qexternprogramaddform.ui \
    externeditor/qexternprogrammanager.ui \
    imageshack/imageshacklistupload.ui \
    imageshack/imageshacklistview.ui \
    imageshack/imageshackuploader.ui \
    qvk/vkdownloadalbumform.ui \
    qvk/vkuploadimageform.ui \
    qvk/vkuploadimagesform.ui \
    settings/settings.ui \
    update/updatedialog.ui \
    update/updateinformer.ui \
    edit/editformfilters.ui
SOURCES += \
    aboutform.cpp \
    fullscreen.cpp \
    image.cpp \
    image_events.cpp \
    main.cpp \
    previewlist.cpp \
    qimageviewer.cpp \
    edit/editformcrop.cpp \
    edit/editformcropimage.cpp \
    edit/editformresize.cpp \
    edit/editformresizeelements.cpp \
    edit/image_edit.cpp \
    externeditor/qexternprogram.cpp \
    externeditor/qexternprogramaddform.cpp \
    externeditor/qexternprogrammanager.cpp \
    imageshack/imageshacklistupload.cpp \
    imageshack/imageshacklistview.cpp \
    imageshack/imageshackuploader.cpp \
    imageshack/qimageviewer_imageshack.cpp \
    imageshack/uploadtoimageshack.cpp \
    qvk/json.cpp \
    qvk/qimageviewer_vk.cpp \
    qvk/qvk.cpp \
    qvk/reply.cpp \
    qvk/vkdownloadalbumform.cpp \
    qvk/vkuploadimageform.cpp \
    qvk/vkuploadimagesform.cpp \
    settings/qhotkeyedit.cpp \
    settings/qhotkeywidget.cpp \
    settings/qimageviewer_design.cpp \
    settings/settings.cpp \
    settings/settings_hotkeys.cpp \
    update/updatedialog.cpp \
    update/updateinformer.cpp \
    update/qimageviewer_update.cpp \
    previewmode.cpp \
    externeditor/qimageviewer_extern.cpp \
    algorithms.cpp \
    qimageviewer_mode.cpp \
    edit/imagefilter.cpp \
    edit/editformfilters.cpp
RESOURCES += resources.qrc
TRANSLATIONS += lng/qimageviewer_cz.ts \
                lng/qimageviewer_en.ts \
                lng/qimageviewer_pl.ts \
                lng/qimageviewer_ru.ts \
                lng/qimageviewer_uk.ts

OTHER_FILES += \
    lng/qimageviewer_uk.ts \
    lng/qimageviewer_uk.qm \
    lng/qimageviewer_ru.ts \
    lng/qimageviewer_ru.qm \
    lng/qimageviewer_pl.ts \
    lng/qimageviewer_pl.qm \
    lng/qimageviewer_en.ts \
    lng/qimageviewer_en.qm \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    lng/qimageviewer_cs.ts \
    lng/qimageviewer_cs.qm \
    info.plist
