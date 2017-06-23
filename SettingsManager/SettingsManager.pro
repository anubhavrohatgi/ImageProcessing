QT += core gui

TEMPLATE = app
TARGET = SettingsManager

CONFIG += c++14
CONFIG += console
CONFIG -= app_bundle


QMAKE_CXXFLAGS_RELEASE += -O3 -msse4.1 -mssse3 -msse3 -msse2 -msse

OBJECTS_DIR = .tmplib
MOC_DIR = .tmplib


SOURCES += \
    SettingsManager.cpp\
    main.cpp	

HEADERS += \
    SettingsManager.h

