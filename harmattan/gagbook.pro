TEMPLATE = app
TARGET = gagbook

VERSION = 0.9.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT += network webkit

# Qt-Json
HEADERS += ../qt-json/json.h
SOURCES += ../qt-json/json.cpp

HEADERS += \
    ../src/qmlutils.h \
    ../src/appsettings.h \
    ../src/gagbookmanager.h \
    ../src/gagmodel.h \
    ../src/gagobject.h \
    ../src/gagrequest.h \
    ../src/ninegagrequest.h \
    ../src/infinigagrequest.h \
    ../src/networkmanager.h \
    ../src/gagimagedownloader.h \
    ../src/gagcookiejar.h \
    ../src/volumekeylistener.h

SOURCES += main.cpp \
    ../src/qmlutils.cpp \
    ../src/appsettings.cpp \
    ../src/gagbookmanager.cpp \
    ../src/gagmodel.cpp \
    ../src/gagobject.cpp \
    ../src/gagrequest.cpp \
    ../src/ninegagrequest.cpp \
    ../src/infinigagrequest.cpp \
    ../src/networkmanager.cpp \
    ../src/gagimagedownloader.cpp \
    ../src/gagcookiejar.cpp \
    ../src/volumekeylistener.cpp

# Simulator
simulator{
    folder_01.source = qml/gagbook-harmattan
    folder_01.target = qml
    folder_02.source = qml/gagbook-symbian
    folder_02.target = qml
    DEPLOYMENTFOLDERS = folder_01 folder_02

    RESOURCES += splash/resource.qrc
}

# MeeGo Harmattan
contains(MEEGO_EDITION,harmattan) {
    folder_01.source = qml/gagbook-harmattan
    folder_01.target = qml
    DEPLOYMENTFOLDERS = folder_01

    CONFIG += link_pkgconfig shareuiinterface-maemo-meegotouch mdatauri qdeclarative-boostable
    PKGCONFIG += qdeclarative-boostable libresourceqt1
    DEFINES += Q_OS_HARMATTAN HAS_LIBRESOURCEQT

    splash.files = splash/gagbook-splash-portrait.jpg splash/gagbook-splash-landscape.jpg
    splash.path = /opt/$${TARGET}/splash
    INSTALLS += splash
}

# Symbian^3
symbian{
    folder_01.source = qml/gagbook-symbian
    folder_01.target = qml
    DEPLOYMENTFOLDERS = folder_01

    # This is a self-signed UID
    TARGET.UID3 = 0xA00158E4

    CONFIG += qt-components
    TARGET.CAPABILITY += NetworkServices
    ICON = gagbook-symbian.svg
    RESOURCES += splash/resource.qrc

    vendorinfo += "%{\"Dickson\"}" ":\"Dickson\""
    my_deployment.pkg_prerules = vendorinfo
    DEPLOYMENT += my_deployment
    DEPLOYMENT.display_name += GagBook

    # Symbian have a different syntax
    DEFINES -= APP_VERSION=\\\"$$VERSION\\\"
    DEFINES += APP_VERSION=\"$$VERSION\"

    # For QMLUtils::openDefaultBrowser()
    LIBS += -lapgrfx -leikcore -lcone -lapmime
}

OTHER_FILES += qtc_packaging/debian_harmattan/* \
    gagbook_harmattan.desktop \
    ../README.md \
    ../LICENSE

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
