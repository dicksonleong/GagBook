#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"

#include "shareui.h"
#include "qmlsettings.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    app->setApplicationName("GagBook");
    app->setOrganizationName("GagBook");
    app->setApplicationVersion("0.0.4");

    QmlApplicationViewer viewer;

    ShareUI shareUI;
    viewer.rootContext()->setContextProperty("shareUI", &shareUI);

    QMLSettings settingsStorage;
    viewer.rootContext()->setContextProperty("settingsStorage", &settingsStorage);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/gagbook/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
