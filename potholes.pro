#-------------------------------------------------
#
# Project created by QtCreator 2016-09-07T20:46:04
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = potholes
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
LIBS += `pkg-config opencv --libs`
