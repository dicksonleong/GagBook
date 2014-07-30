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

#ifndef VOLUMEKEYLISTENER_H
#define VOLUMEKEYLISTENER_H

#include <QtCore/QObject>

#ifdef HAS_LIBRESOURCEQT
#include <policy/resource-set.h>
#endif

/*! Listen to volume keys press event

    Listen to volume keys press event from QDeclarativeView/QQuickView
    and emit signals so they can be handled (by QML). On platform that support
    [libresourceqt](http://api.devpda.net/meego/html/libresourceqt/main.html)
    (Harmattan & SailfishOS), this class also handle acquiring resource for
    volume keys.
 */
class VolumeKeyListener : public QObject
{
    Q_OBJECT

    /*! If set to true, volume keys press event will be captured and volumeUpClicked()
        and volumeDownClicked() will be emitted. On platforms that use libresourceqt,
        resource for volume keys will be acquired. */
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
public:
    explicit VolumeKeyListener(QObject *parent = 0);

    bool enabled() const;
    void setEnabled(bool enabled);

signals:
    void enabledChanged();

    /*! Emit when volume up keys is pressed. */
    void volumeUpClicked();
    /*! Emit when volume down keys is pressed. */
    void volumeDownClicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    bool m_enabled;
#ifdef HAS_LIBRESOURCEQT
    ResourcePolicy::ResourceSet *set;
#endif
};

#endif // VOLUMEKEYLISTENER_H
