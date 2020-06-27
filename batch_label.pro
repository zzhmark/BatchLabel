
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VAA3DPATH = D:/vaa3d/v3d_external
INCLUDEPATH	+= $$VAA3DPATH/v3d_main/basic_c_fun \
    $$VAA3DPATH/v3d_main/common_lib/include

HEADERS	+= batch_label_plugin.h \
    batchlabeldialog.h
SOURCES	+= batch_label_plugin.cpp \
    batchlabeldialog.cpp \
    vaa3dutil.cpp
SOURCES	+= $$VAA3DPATH/v3d_main/basic_c_fun/v3d_message.cpp

TARGET	= $$qtLibraryTarget(batch_label)
DESTDIR	= $$VAA3DPATH/bin/plugins/batch_label/

FORMS += \
    batchlabeldialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/winlib64/ -llibtiff
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/winlib64/ -llibtiff

INCLUDEPATH += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64
DEPENDPATH += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64/liblibtiff.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64/liblibtiff.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64/libtiff.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64/libtiff.lib

win32: LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/winlib64/ -lteem

INCLUDEPATH += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64
DEPENDPATH += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64/teem.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/winlib64/libteem.a

macx: LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/lib_mac64/ -lteem

INCLUDEPATH += $$PWD/../../v3d_external/v3d_main/common_lib
DEPENDPATH += $$PWD/../../v3d_external/v3d_main/common_lib

macx: PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/lib_mac64/libteem.a

macx: LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_mac/ -ltiff

INCLUDEPATH += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_mac
DEPENDPATH += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_mac

macx: PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_mac/libtiff.a

macx: LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_mac/ -ltiffxx

INCLUDEPATH += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_mac
DEPENDPATH += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_mac

macx: PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_mac/libtiffxx.a

unix:!macx: LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_linux/ -ltiff

INCLUDEPATH += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_linux
DEPENDPATH += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_linux

unix:!macx: PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_linux/libtiff.a

unix:!macx: LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_linux/ -ltiffxx

INCLUDEPATH += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_linux
DEPENDPATH += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_linux

unix:!macx: PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/tiff4/lib_linux/libtiffxx.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64/ -lteem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64/ -lteemd
else:unix: LIBS += -L$$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64/ -lteem

INCLUDEPATH += $$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64
DEPENDPATH += $$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64/libteem.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64/libteemd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64/teem.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64/teemd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../v3d_external/v3d_main/common_lib/lib_unix64/libteem.a
