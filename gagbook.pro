HEADERS += \
    qmlsettings.h \
    qmlimagesaver.h

SOURCES += main.cpp \
    qmlsettings.cpp \
    qmlimagesaver.cpp

# Simulator
simulator{
    folder_01.source = qml-meego/gagbook
    folder_01.target = qml-meego
    folder_02.source = qml-symbian/gagbook
    folder_02.target = qml-symbian
    DEPLOYMENTFOLDERS = folder_01 folder_02

    HEADERS += shareui.h
    SOURCES += shareui.cpp
}

# MeeGo Harmattan
contains(MEEGO_EDITION,harmattan) {
    folder_01.source = qml-meego/gagbook
    folder_01.target = qml-meego
    DEPLOYMENTFOLDERS = folder_01

    HEADERS += shareui.h
    SOURCES += shareui.cpp

    CONFIG += shareuiinterface-maemo-meegotouch share-ui-plugin share-ui-common mdatauri qdeclarative-boostable
    DEFINES += Q_OS_HARMATTAN
}

# Symbian^3
symbian{
    folder_01.source = qml-symbian/gagbook
    folder_01.target = qml-symbian
    DEPLOYMENTFOLDERS = folder_01

    CONFIG += qt-components
    TARGET.UID3 = 0xE1C5FA03 # Development UID which can be self-signed
    TARGET.CAPABILITY += NetworkServices
    ICON = gagbook-symbian.svg

    vendorinfo += "%{\"Dickson\"}" ":\"Dickson\""
    my_deployment.pkg_prerules = vendorinfo
    DEPLOYMENT += my_deployment
    DEPLOYMENT.display_name += GagBook

    VERSION = 0.1.0
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

!symbian:OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

