#-------------------------------------------------
#
# Project created by QtCreator 2019-05-01T15:41:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH=/usr/local/include

LIBS += -lpcap
LIBS +=  -L/usr/local/lib  -lnet

TARGET = ProtoTool
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    nicmsg.cpp \
    choosedlg.cpp \
    netmodel.cpp \
    showwid.cpp \
    requestdia.cpp

HEADERS += \
        mainwindow.h \
    nicmsg.h \
    singleton.h \
    choosedlg.h \
    netmodel.h \
    pcapstruct.h \
    showwid.h \
    requestdia.h

FORMS += \
        mainwindow.ui \
    choosedlg.ui \
    showwid.ui \
    requestdia.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
