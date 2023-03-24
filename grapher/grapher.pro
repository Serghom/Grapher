QT += core gui widgets xml svg
unix {
    INCLUDEPATH += /home/lebedenko/Qt5.5.1/qwt6/include/
    LIBS += /home/lebedenko/Qt5.5.1/qwt6/lib/libqwt.so.6.1.3 -ldl
}

win32 {
    LIBS += -L$$(QWT_ROOT)/lib -lqwt
    INCLUDEPATH += $$(QWT_ROOT)/include
#    DEFINES += QT_NO_DEBUG_OUTPUT
}


TARGET = grapher
#CONFIG += console
CONFIG += app_bundle
CONFIG += qwt

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app



SOURCES += main.cpp \
    painter.cpp \
    parcer.cpp \
    graphergui.cpp \
    mymagnifier.cpp \
    myqframe.cpp

HEADERS += \
    painter.h \
    parcer.h \
    graphergui.h \
    mymagnifier.h \
    qscaledraw.h \
    myqframe.h

FORMS += \
    graphergui.ui

DISTFILES +=

RESOURCES += \
    resource.qrc


