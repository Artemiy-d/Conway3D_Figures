TEMPLATE = app
DESTDIR = dist/Release/
TARGET = Conwey3D
VERSION = 1.0.0
#CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui opengl
SOURCES += MyCollection.cpp MainWindow.cpp otherGuiClasses.cpp modelTemplates.cpp main.cpp Figure.cpp DialogSettings.cpp Languages.cpp FigureClasses.cpp DialogAbout.cpp Modeles.cpp BaseSurface.cpp Scene3D.cpp IncludingSurface.cpp \
    DialogOfCreation.cpp
HEADERS += otherGuiClasses.h Languages.h Scene3D.h Modeles.h modelTemplates.h MainWindow.h FigureClasses.h MyCollection.h DialogAbout.h Figure.h complect_headers.h \
    DialogSettings.h \
    DialogOfCreation.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
#QMAKE_CC = gcc
#QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 


