ROOT_DIR    = ../../

include (../ADAK/INCLUDE/adak.pri)

QT -= gui
QT += sql


TARGET = MUH_ONAR

CONFIG += console

CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../ADAK/INCLUDE . ..

SOURCES += e9_onar_main.cpp \
           e9_onar_utils.cpp \
           ../MUHASEBE/muh_console_utils.cpp


LIBS           += ../ADAK/$$OBJDIR/libADAK.a
PRE_TARGETDEPS += ../ADAK/$$OBJDIR/libADAK.a

win32: {LIBS+= -liphlpapi}
