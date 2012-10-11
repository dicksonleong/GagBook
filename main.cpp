/*
    Copyright (C) 2012  Dickson Leong
    This file is part of GagBook.

    GagBook is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see http://www.gnu.org/licenses/.
*/

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
    app->setApplicationVersion("0.2.0");

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
    viewer.setMainQmlFile(QLatin1String("qml/gagbook-harmattan/main.qml"));
#elif defined(Q_OS_SYMBIAN)
    viewer.setMainQmlFile(QLatin1String("qml/gagbook-symbian/main.qml"));
#else // Simulator. Change this value to run Symbian or Harmattan version on simulator
    viewer.setMainQmlFile(QLatin1String("qml/gagbook-harmattan/main.qml"));
#endif
    viewer.showExpanded();

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    splash->finish(&viewer);
    splash->deleteLater();
#endif

    return app->exec();
}
