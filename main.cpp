#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"

#if defined(Q_OS_HARMATTAN) || defined(Q_WS_SIMULATOR)
#include "src/shareui.h"
#endif

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
#include <QSplashScreen>
#include <QPixmap>
#endif

#include "src/qmlsettings.h"
#include "src/qmlimagesaver.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    app->setApplicationName("GagBook");
    app->setOrganizationName("GagBook");
    app->setApplicationVersion("0.1.1");

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/splash/gagbook-splash-symbian.jpg"));
    splash->show();
    splash->showMessage("Loading...", Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
#endif

    QmlApplicationViewer viewer;

#if defined(Q_OS_HARMATTAN) || defined(Q_WS_SIMULATOR)
    ShareUI shareUI;
    viewer.rootContext()->setContextProperty("shareUI", &shareUI);
#endif

    QMLSettings settingsStorage;
    viewer.rootContext()->setContextProperty("settingsStorage", &settingsStorage);

    QMLImageSaver imageSaver;
    viewer.rootContext()->setContextProperty("imageSaver", &imageSaver);

    viewer.rootContext()->setContextProperty("appVersion", app->applicationVersion());

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
#if defined(Q_OS_HARMATTAN)
    viewer.setMainQmlFile(QLatin1String("qml/gagbook-meego/main.qml"));
#elif defined(Q_OS_SYMBIAN)
    viewer.setMainQmlFile(QLatin1String("qml/gagbook-symbian/main.qml"));
#else // Simulator. Change this value to run symbian or meego version on simulator
    viewer.setMainQmlFile(QLatin1String("qml/gagbook-symbian/main.qml"));
#endif
    viewer.showExpanded();

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    splash->finish(&viewer);
    splash->deleteLater();
#endif

    return app->exec();
}
