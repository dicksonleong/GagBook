TARGET = harbour-gagbook

#see gagbook.yaml for version setting
DEFINES += APP_VERSION=\\\"$$VERSION\\\" HAS_LIBRESOURCEQT

QT += network widgets webkitwidgets

CONFIG += link_pkgconfig sailfishapp
PKGCONFIG += libresourceqt5

INCLUDEPATH += ..

HEADERS += \
    ../src/qmlutils.h \
    ../src/appsettings.h \
    ../src/gagbookmanager.h \
    ../src/gagmodel.h \
    ../src/gagobject.h \
    ../src/gagrequest.h \
    ../src/infinigagrequest.h \
    ../src/ninegagrequest.h \
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
    ../src/infinigagrequest.cpp \
    ../src/ninegagrequest.cpp \
    ../src/networkmanager.cpp \
    ../src/gagimagedownloader.cpp \
    ../src/gagcookiejar.cpp \
    ../src/volumekeylistener.cpp

# Qt-Json
HEADERS += ../qt-json/json.h
SOURCES += ../qt-json/json.cpp

OTHER_FILES += \
    rpm/$${TARGET}.spec \
    rpm/$${TARGET}.yaml \
    rpm/$${TARGET}.changes \
    $${TARGET}.desktop \
    $${TARGET}.png \
    ../README.md \
    ../LICENSE \
    qml/*.qml \
    qml/Images/*.png \
    qml/LoginPage.qml
