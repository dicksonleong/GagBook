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

#include "qmlutils.h"

#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtDeclarative/QDeclarativeItem>
#include <QtGui/QImage>
#include <QtGui/QDesktopServices>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#ifdef Q_OS_HARMATTAN
#include <MDataUri>
#include <maemo-meegotouch-interfaces/shareuiinterface.h>
#endif

#ifdef Q_OS_SYMBIAN
#include <eikenv.h> // CEikonEnv
#include <apgcli.h> // RApaLsSession
#include <apgtask.h> // TApaTaskList, TApaTask

_LIT(KBrowserPrefix, "4 " );
static const TUid KUidBrowser = { 0x10008D39 };
#endif

static const QString IMAGE_SAVING_FILE_PATH = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);

QMLUtils::QMLUtils(QObject *parent) :
    QObject(parent), m_netManager(0), m_imageDowloadReply(0), m_busy(false)
{
}

void QMLUtils::setNetworkAccessManager(QNetworkAccessManager *manager)
{
    m_netManager = manager;
}

void QMLUtils::copyToClipboard(const QString &text)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text, QClipboard::Clipboard);
    clipboard->setText(text, QClipboard::Selection);
}

void QMLUtils::downloadImage(const QString &imageUrl)
{
    Q_ASSERT_X(m_netManager != 0, Q_FUNC_INFO, "NetworkAccessManager not set");

    if (m_imageDowloadReply != 0) {
        m_imageDowloadReply->disconnect();
        m_imageDowloadReply->deleteLater();
        m_imageDowloadReply = 0;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(imageUrl));
    m_imageDowloadReply = m_netManager->get(request);
    connect(m_imageDowloadReply, SIGNAL(finished()), SLOT(onImageDownloadReplyFinished()));

    m_busy = true;
    emit busyChanged();
}

void QMLUtils::onImageDownloadReplyFinished()
{
    Q_ASSERT(m_imageDowloadReply != 0);

    m_busy = false;
    emit busyChanged();

    if (m_imageDowloadReply->error()) {
        emit imageDownloadFinished(m_imageDowloadReply->errorString());
        m_imageDowloadReply->deleteLater();
        m_imageDowloadReply = 0;
        return;
    }

    QImage gagImage = QImage::fromData(m_imageDowloadReply->readAll());

    if (gagImage.isNull()) {
        emit imageDownloadFinished("Unable to load image from reply data");
        m_imageDowloadReply->deleteLater();
        m_imageDowloadReply = 0;
        return;
    }

    QString fileName = m_imageDowloadReply->url().toString();
    fileName.remove(0, fileName.lastIndexOf("/") + 1);
    fileName.prepend(IMAGE_SAVING_FILE_PATH + "/");
    bool saved = gagImage.save(fileName, 0, 100);

    if (!saved)
        emit imageDownloadFinished("Unable to save image to " + fileName);
    else
        emit imageDownloadFinished("Image saved to " + fileName);

    m_imageDowloadReply->deleteLater();
    m_imageDowloadReply = 0;
}

void QMLUtils::shareLink(const QString &link, const QString &title)
{
#ifdef Q_OS_HARMATTAN
    MDataUri uri;
    uri.setMimeType("text/x-url");
    uri.setTextData(link);

    if (!title.isEmpty())
        uri.setAttribute("title", title);

    if (!uri.isValid()) {
        qCritical("QMLUtils::shareLink(): Invalid URI");
        return;
    }

    ShareUiInterface shareIf("com.nokia.ShareUi");

    if (!shareIf.isValid()) {
        qCritical("QMLUtils::shareLink(): Invalid Share UI interface");
        return;
    }

    shareIf.share(QStringList() << uri.toString());
#else
    qWarning("QMLUtils::shareLink(): This function only available on Harmattan");
    Q_UNUSED(title)
    Q_UNUSED(link)
#endif
}

void QMLUtils::openDefaultBrowser(const QUrl &url)
{
#ifdef Q_OS_SYMBIAN
    // convert url to encoded version of QString
    QString encUrl(QString::fromUtf8(url.toEncoded()));
    // using qt_QString2TPtrC() based on
    // <http://qt.gitorious.org/qt/qt/blobs/4.7/src/corelib/kernel/qcore_symbian_p.h#line102>
    TPtrC tUrl(TPtrC16(static_cast<const TUint16*>(encUrl.utf16()), encUrl.length()));

    // Following code based on
    // <http://www.developer.nokia.com/Community/Wiki/Launch_default_web_browser_using_Symbian_C%2B%2B>

    // create a session with apparc server
    RApaLsSession appArcSession;
    User::LeaveIfError(appArcSession.Connect());
    CleanupClosePushL<RApaLsSession>(appArcSession);

    // get the default application uid for application/x-web-browse
    TDataType mimeDatatype(_L8("application/x-web-browse"));
    TUid handlerUID;
    appArcSession.AppForDataType(mimeDatatype, handlerUID);

    // if UiD not found, use the native browser
    if (handlerUID.iUid == 0 || handlerUID.iUid == -1)
        handlerUID = KUidBrowser;

    // Following code based on
    // <http://qt.gitorious.org/qt/qt/blobs/4.7/src/gui/util/qdesktopservices_s60.cpp#line213>

    HBufC* buf16 = HBufC::NewLC(tUrl.Length() + KBrowserPrefix.iTypeLength);
    buf16->Des().Copy(KBrowserPrefix); // Prefix used to launch correct browser view
    buf16->Des().Append(tUrl);

    TApaTaskList taskList(CEikonEnv::Static()->WsSession());
    TApaTask task = taskList.FindApp(handlerUID);
    if (task.Exists()) {
        // Switch to existing browser instance
        task.BringToForeground();
        HBufC8* param8 = HBufC8::NewLC(buf16->Length());
        param8->Des().Append(buf16->Des());
        task.SendMessage(TUid::Uid( 0 ), *param8); // Uid is not used
        CleanupStack::PopAndDestroy(param8);
    } else {
        // Start a new browser instance
        TThreadId id;
        appArcSession.StartDocument(*buf16, handlerUID, id);
    }

    CleanupStack::PopAndDestroy(buf16);
    CleanupStack::PopAndDestroy(&appArcSession);
#else
    qWarning("QMLUtils::openDefaultBrowser(): This function only available on Symbian");
    Q_UNUSED(url)
#endif
}
