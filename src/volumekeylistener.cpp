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

#include "volumekeylistener.h"

#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>

#ifdef HAS_LIBRESOURCEQT
#include <policy/resource.h>
#endif

VolumeKeyListener::VolumeKeyListener(QObject *parent) :
    QObject(parent), m_enabled(false)
{
#if HAS_LIBRESOURCEQT
    set = new ResourcePolicy::ResourceSet("player", this);
    set->addResource(ResourcePolicy::ScaleButtonType);
#endif
}

bool VolumeKeyListener::enabled() const
{
    return m_enabled;
}

void VolumeKeyListener::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
#if HAS_LIBRESOURCEQT
        if (m_enabled)
            set->acquire();
        else
            set->release();
#endif
        emit enabledChanged();
    }
}

bool VolumeKeyListener::eventFilter(QObject *obj, QEvent *event)
{
    if (m_enabled && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        switch (keyEvent->key()) {
        case Qt::Key_VolumeUp:
            emit volumeUpClicked();
            return true;
        case Qt::Key_VolumeDown:
            emit volumeDownClicked();
            return true;
        default:
            break;
        }
    }
    return QObject::eventFilter(obj, event);
}
