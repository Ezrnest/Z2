#-------------------------------------------------
#
# Project created by QtCreator 2019-07-22T12:41:39
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Z2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += *.h \



SOURCES += *.cpp \

#CONFIG += resources_big

RESOURCES += \
    resources.qrc

FORMS += *.ui

TRANSLATIONS += lang_cn.ts \
               lang_en.ts

RC_ICONS = z2.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -L$$PWD/../../../api/z2Core/ -lZ2Core
win32: LIBS += -lws2_32
win32: LIBS += -lwsock32

INCLUDEPATH += $$PWD/../../../api/z2Core/include
DEPENDPATH += $$PWD/../../../api/z2Core/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../api/z2Core/Z2Core.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../api/z2Core/libZ2Core.a




