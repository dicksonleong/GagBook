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

#include "gagobject.h"

#include <QtCore/QString>
#include <QtCore/QUrl>

class GagObjectData : public QSharedData
{
public:
    GagObjectData() : imageHeight(0), votesCount(0), commentsCount(0), isVideo(false), isNSFW(false) {}
    ~GagObjectData() {}

    QString id;
    QUrl url;
    QString title;
    QUrl imageUrl;
    int imageHeight;
    int votesCount;
    int commentsCount;
    bool isVideo;
    bool isNSFW;

private:
    Q_DISABLE_COPY(GagObjectData) // Disable copy for the data
};

GagObject::GagObject() :
    d(new GagObjectData)
{
}

GagObject::GagObject(const GagObject &other) :
    d(other.d)
{
}

GagObject &GagObject::operator=(const GagObject &other)
{
    d = other.d;
    return *this;
}

GagObject::~GagObject()
{
}

QString GagObject::id() const
{
    return d->id;
}

void GagObject::setId(const QString &id)
{
    d->id = id;
}

QUrl GagObject::url() const
{
    return d->url;
}

void GagObject::setUrl(const QUrl &url)
{
    d->url = url;
}

QString GagObject::title() const
{
    return d->title;
}

void GagObject::setTitle(const QString &title)
{
    d->title = title;
}

QUrl GagObject::imageUrl() const
{
    return d->imageUrl;
}

void GagObject::setImageUrl(const QUrl &imageUrl)
{
    d->imageUrl = imageUrl;
}

int GagObject::imageHeight() const
{
    return d->imageHeight;
}

void GagObject::setImageHeight(int imageHeight)
{
    d->imageHeight = imageHeight;
}

int GagObject::votesCount() const
{
    return d->votesCount;
}

void GagObject::setVotesCount(int votes)
{
    d->votesCount = votes;
}

int GagObject::commentsCount() const
{
    return d->commentsCount;
}

void GagObject::setCommentsCount(int comments)
{
    d->commentsCount = comments;
}

bool GagObject::isVideo() const
{
    return d->isVideo;
}

void GagObject::setIsVideo(bool isVideo)
{
    d->isVideo = isVideo;
}

bool GagObject::isNSFW() const
{
    return d->isNSFW;
}

void GagObject::setIsNSFW(bool isNSFW)
{
    d->isNSFW = isNSFW;
}

QVariantMap GagObject::toVariantMap() const
{
    QVariantMap gagMap;
    gagMap["id"] = d->id;
    gagMap["url"] = d->url;
    gagMap["title"] = d->title;
    gagMap["imageUrl"] = d->imageUrl;
    gagMap["votesCount"] = d->votesCount;
    gagMap["isVideo"] = d->isVideo;
    gagMap["isNSFW"] = d->isNSFW;
    return gagMap;
}
