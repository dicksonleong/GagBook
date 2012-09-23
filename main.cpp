#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"
#include "shareui.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    app->setApplicationName("GagBook");
    app->setOrganizationName("GagBook");

    QmlApplicationViewer viewer;

    ShareUI shareUI;
    viewer.rootContext()->setContextProperty("shareUI", &shareUI);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/gagbook/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
