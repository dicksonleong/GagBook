#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"

#include "shareui.h"
#include "qmlsettings.h"
#include "qmlimagesaver.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    app->setApplicationName("GagBook");
    app->setOrganizationName("GagBook");
    app->setApplicationVersion("0.1");

    QmlApplicationViewer viewer;

    ShareUI shareUI;
    viewer.rootContext()->setContextProperty("shareUI", &shareUI);

    QMLSettings settingsStorage;
    viewer.rootContext()->setContextProperty("settingsStorage", &settingsStorage);

    QMLImageSaver imageSaver;
    viewer.rootContext()->setContextProperty("imageSaver", &imageSaver);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/gagbook/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
