TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    creature.cpp \
    wolf.cpp \
    rabbit.cpp \
    cell.cpp \
    field.cpp \
    coords.cpp

include(deployment.pri)
qtcAddDeployment()

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    creature.hpp \
    wolf.hpp \
    defs.hpp \
    rabbit.hpp \
    cell.hpp \
    field.hpp \
    coords.hpp
