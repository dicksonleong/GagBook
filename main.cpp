/*
 * Copyright (c) 2012-2013 Dickson Leong.
 * All rights reserved.
 *
 * This file is part of GagBook.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of GagBook nor the names of its contributors may be
 *       used to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/qdeclarative.h>
#include "qmlapplicationviewer.h"

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
#include <QtGui/QSplashScreen>
#include <QtGui/QPixmap>
#endif

#include "src/gagmodel.h"
#include "src/qmlutils.h"

#if defined(Q_OS_HARMATTAN) || defined(Q_WS_SIMULATOR)
#include "src/shareui.h"
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    app->setApplicationName("GagBook");
    app->setOrganizationName("GagBook");
    app->setApplicationVersion(APP_VERSION);

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/splash/gagbook-splash-symbian.jpg"));
    splash->show();
    splash->showMessage("Loading...", Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
#endif

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("APP_VERSION", APP_VERSION);

#if defined(Q_OS_HARMATTAN) || defined(Q_WS_SIMULATOR)
    ShareUI shareUI;
    viewer.rootContext()->setContextProperty("shareUI", &shareUI);
#endif

    QMLUtils qmlUtils;
    viewer.rootContext()->setContextProperty("QMLUtils", &qmlUtils);

    qmlRegisterType<GagModel>("GagModel", 1, 0, "GagModel");

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
