#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T15:07:51
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CGclasses
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    trackball.cpp \
    camera.cpp \
    light.cpp \
    material.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    trackball.h \
    camera.h \
    light.h \
    material.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    vshader1.glsl \
    twoSideRendering.vert \
    twoSideRendering.frag \
    test.vert \
    test.frag
