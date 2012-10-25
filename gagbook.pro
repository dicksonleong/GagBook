HEADERS += \
    src/qmlsettings.h \
    src/qmlimagesaver.h

SOURCES += main.cpp \
    src/qmlsettings.cpp \
    src/qmlimagesaver.cpp

# Simulator
simulator{
    folder_01.source = qml/gagbook-harmattan
    folder_01.target = qml
    folder_02.source = qml/gagbook-symbian
    folder_02.target = qml
    DEPLOYMENTFOLDERS = folder_01 folder_02

    HEADERS += src/shareui.h
    SOURCES += src/shareui.cpp
    RESOURCES += resource.qrc
}

# MeeGo Harmattan
contains(MEEGO_EDITION,harmattan) {
    folder_01.source = qml/gagbook-harmattan
    folder_01.target = qml
    DEPLOYMENTFOLDERS = folder_01

    HEADERS += src/shareui.h
    SOURCES += src/shareui.cpp

    CONFIG += shareuiinterface-maemo-meegotouch share-ui-plugin share-ui-common mdatauri qdeclarative-boostable
    DEFINES += Q_OS_HARMATTAN

    splash.files = splash/gagbook-splash-portrait.jpg splash/gagbook-splash-landscape.jpg
    splash.path = /opt/gagbook/splash
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
    RESOURCES += resource.qrc

    vendorinfo += "%{\"Dickson\"}" ":\"Dickson\""
    my_deployment.pkg_prerules = vendorinfo
    DEPLOYMENT += my_deployment
    DEPLOYMENT.display_name += GagBook

    VERSION = 0.2.2
}

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += qtc_packaging/debian_harmattan/* \
    gagbook_harmattan.desktop \
    README.md
