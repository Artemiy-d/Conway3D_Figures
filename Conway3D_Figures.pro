TEMPLATE = app
#DESTDIR = dist/Release/
TARGET = Conwey3D
VERSION = 1.0.0
#CONFIG -= debug_and_release app_bundle lib_bundle
#CONFIG += release
QT = core gui opengl
#QMAKE_CXXFLAGS += -std=c++11


INCLUDEPATH += ./includes

SOURCES += \
    src/MainWindow.cpp \
    src/main.cpp \
    src/Figure.cpp \
    src/DialogSettings.cpp \
    src/FigureClasses.cpp \
    src/DialogAbout.cpp \
    src/Scene3D.cpp \
    src/IncludingSurface.cpp \
    src/DialogTemplates.cpp \
    src/DialogNewFigure.cpp \
    src/LanguageManager.cpp \
    src/DialogSaveFigure.cpp \
    src/GroupBoxCustom.cpp \
    src/ModelRedactor.cpp \
    src/Scene3DView.cpp \
    src/FileManager.cpp \
    src/ModelsManager.cpp \
    src/Models.cpp \
    src/FiguresFactory.cpp \
    src/NewFigureOptions.cpp

HEADERS += \
    includes/Scene3D.h \
    includes/MainWindow.h \
    includes/FigureClasses.h \
    includes/DialogAbout.h \
    includes/Figure.h \
    includes/DialogSettings.h \
    includes/DialogTemplates.h \
    includes/StringMap.h \
    includes/DialogNewFigure.h \
    includes/LanguageManager.h \
    includes/RandomLCG.h \
    includes/DialogSaveFigure.h \
    includes/GroupBoxCustom.h \
    includes/ModelRedactor.h \
    includes/Point.h \
    includes/Scene3DView.h \
    includes/IncludingSurface.h \
    includes/FileManager.h \
    includes/ModelsManager.h \
    includes/Models.h \
    includes/FiguresFactory.h \
    includes/NewFigureOptions.h

FORMS +=
RESOURCES +=
TRANSLATIONS +=
#OBJECTS_DIR = build/Release/
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
#QMAKE_CC = gcc
#QMAKE_CXX = g++
DEFINES += 
LIBS += -lglut
unix: LIBS += -lGLU

config.path    = $$OUT_PWD
config.files   = data/*
INSTALLS += config




