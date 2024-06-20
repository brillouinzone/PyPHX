QT += core gui widgets

TARGET = GrabberLib_Q
TEMPLATE = app

SOURCES += main.cpp \
    grabberinterface.cpp \
    grabberconsolewidget.cpp \
    grabberthread.cpp

HEADERS += grabberinterface.h \
    grabberconsolewidget.h \
    grabberthread.h

INCLUDEPATH += ActiveSDK/include

LIBS += -L../ActiveSDK/Lib/win64 \
    -lphxdlx64 \
    -lphxblx64 \
    -lphxilx64 \
    -lphxlx64

OTHER_FILES +=




