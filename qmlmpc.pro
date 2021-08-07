TEMPLATE = app

QT += qml
QT += quick
QT += quickcontrols2

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050F00

SOURCES += main.cpp \
    mpc/datamodels.cpp \
    mpc/mpdconnector.cpp \
    mpc/mpdparser.cpp \
    mpc/mpdrequest.cpp \
    mpc/musicplayerconnection.cpp \
    mpc/types.cpp \
    settings.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
#include(deployment.pri)

HEADERS += \
    mpc/datamodels.h \
    mpc/mpdconnector.h \
    mpc/mpdparser.h \
    mpc/mpdrequest.h \
    mpc/musicplayerconnection.h \
    mpc/types.h \
    settings.h

DISTFILES += \
    README.md

android {
    ANDROID_PACKAGE_SOURCE_DIR = android

    DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/res/values/libs.xml
}
