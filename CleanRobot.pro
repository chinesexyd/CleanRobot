QT       += core gui network

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
    Source/a_root.cpp \
    Source/b_ros.cpp \
    Source/b_widget.cpp \
    Source/c_centralpage.cpp \
    Source/c_chooseareapage.cpp \
    Source/c_choosemappage.cpp \
    Source/c_mainwindow.cpp \
    Source/c_setpage.cpp \
    Source/c_statusbar.cpp \
    Source/c_statuspage.cpp \
    Source/c_taskpage.cpp \
    Source/d_logic.cpp \
    Source/main.cpp

HEADERS += \
    Source/a_root.h \
    Source/b_ros.h \
    Source/b_widget.h \
    Source/c_centralpage.h \
    Source/c_chooseareapage.h \
    Source/c_choosemappage.h \
    Source/c_mainwindow.h \
    Source/c_setpage.h \
    Source/c_statusbar.h \
    Source/c_statuspage.h \
    Source/c_taskpage.h \
    Source/d_logic.h
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS += -lyaml-cpp

RESOURCES += \
    Image/Image.qrc
