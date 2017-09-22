#-------------------------------------------------
#
# Project created by QtCreator 2017-09-08T11:09:00
#
#-------------------------------------------------

QT       += core gui
QMAKE_MAC_SDK = macosx10.13
QMAKE_MAC_SDK_PATH = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk"
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += webenginewidgets
TARGET = Neutrino
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


SOURCES += \
        main.cpp \
        browserwindow.cpp \
    history.cpp \
    adressbar.cpp \
    predictor.cpp

HEADERS += \
        browserwindow.h \
    history.h \
    adressbar.h \
    predictor.h

FORMS += \
        browserwindow.ui

RESOURCES += \
    resources.qrc
