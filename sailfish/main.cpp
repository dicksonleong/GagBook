/*
 * Copyright (c) 2014 Dickson Leong.
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

#include <QApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QtQml/qqml.h> // qmlRegisterType

#include "sailfishapp.h"

#include "src/gagbookmanager.h"
#include "src/gagmodel.h"
#include "src/qmlutils.h"
#include "src/networkmanager.h"
#include "src/appsettings.h"
#include "src/volumekeylistener.h"
#include "src/votingmanager.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    //return SailfishApp::main(argc, argv);

    QApplication app(argc, argv);

    app.setApplicationName("GagBook");
    app.setOrganizationName("GagBook");
    app.setApplicationVersion(APP_VERSION);

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->rootContext()->setContextProperty("APP_VERSION", APP_VERSION);

    QMLUtils qmlUtils;
    view->rootContext()->setContextProperty("QMLUtils", &qmlUtils);

    VolumeKeyListener volumeKeyListener;
    view->installEventFilter(&volumeKeyListener);
    view->rootContext()->setContextProperty("volumeKeyListener", &volumeKeyListener);

    qmlRegisterType<GagBookManager>("harbour.gagbook.Core", 1, 0, "GagBookManager");
    qmlRegisterType<GagModel>("harbour.gagbook.Core", 1, 0, "GagModel");
    qmlRegisterType<AppSettings>("harbour.gagbook.Core", 1, 0, "AppSettings");
    qmlRegisterType<VotingManager>("harbour.gagbook.Core", 1, 0, "VotingManager");

    view->setSource(SailfishApp::pathTo("qml/main.qml"));
    view->show();

    return app.exec();
}
