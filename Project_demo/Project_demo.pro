QT       += core gui
QT += network
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    keyBoard.cpp \
    login.cpp \
    loginpass.cpp \
    lookpic.cpp \
    main.cpp \
    mainfrist.cpp \
    mainwindow.cpp \
    perproty.cpp \
    perprotylistwidget.cpp \
    readRfidId.cpp \
    registerdialog.cpp \
    thread_rfid.cpp \
    threadpic.cpp \
    v4l2.cpp \
    videodisplay.cpp

HEADERS += \
    keyBoard.h \
    login.h \
    loginpass.h \
    lookpic.h \
    mainfrist.h \
    mainwindow.h \
    perproty.h \
    perprotylistwidget.h \
    readrfidid.h \
    readydatebase.h \
    registerdialog.h \
    thread_rfid.h \
    threadpic.h \
    v4l2.h \
    videodisplay.h

FORMS += \
    login.ui \
    loginpass.ui \
    lookpic.ui \
    mainfrist.ui \
    mainwindow.ui \
    perproty.ui \
    perprotylistwidget.ui \
    registerdialog.ui \
    videodisplay.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc \
    pifu.qrc
