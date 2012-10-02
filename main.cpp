#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"

#if defined(Q_OS_HARMATTAN) || defined(Q_WS_SIMULATOR)
#include "shareui.h"
#endif

#include "qmlsettings.h"
#include "qmlimagesaver.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    app->setApplicationName("GagBook");
    app->setOrganizationName("GagBook");
    app->setApplicationVersion("0.1");

    QmlApplicationViewer viewer;

#if defined(Q_OS_HARMATTAN) || defined(Q_WS_SIMULATOR)
    ShareUI shareUI;
    viewer.rootContext()->setContextProperty("shareUI", &shareUI);
#endif

    QMLSettings settingsStorage;
    viewer.rootContext()->setContextProperty("settingsStorage", &settingsStorage);

    QMLImageSaver imageSaver;
    viewer.rootContext()->setContextProperty("imageSaver", &imageSaver);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
#if defined(Q_OS_HARMATTAN)
    viewer.setMainQmlFile(QLatin1String("qml-meego/gagbook/main.qml"));
#elif defined(Q_OS_SYMBIAN)
    viewer.setMainQmlFile(QLatin1String("qml-symbian/gagbook/main.qml"));
#else // Simulator. Change this value to run symbian or meego version on simulator
    viewer.setMainQmlFile(QLatin1String("qml-symbian/gagbook/main.qml"));
#endif
    viewer.showExpanded();

    return app->exec();
}
