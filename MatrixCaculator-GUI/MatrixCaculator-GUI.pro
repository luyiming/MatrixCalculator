#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T22:13:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MatrixCaculator-GUI
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    AddMatrixDialog.cpp \
    Matrix.cpp

HEADERS  += MainWindow.h \
    AddMatrixDialog.h \
    Matrix.h

FORMS    += MainWindow.ui \
    AddMatrixDialog.ui

RESOURCES += \
    resource.qrc
