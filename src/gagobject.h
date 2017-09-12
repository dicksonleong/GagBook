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

#ifndef GAGOBJECT_H
#define GAGOBJECT_H

#include <QtCore/QExplicitlySharedDataPointer>
#include <QtCore/QVariant>

class GagObjectData;

/*! Data object representing each 9GAG post

    A data object that representing each 9GAG post (aka "gag"). GagObject is
    an explicitly shared class, that means only a shallow copy is done when
    copying this object. This class can not be deep copy, since each gag is
    suppose to be unique.

    \sa [QExplicitlySharedDataPointer](http://qt-project.org/doc/qt-4.8/qexplicitlyshareddatapointer.html)
 */
class GagObject
{
public:
    GagObject();
    GagObject(const GagObject &other);
    GagObject &operator=(const GagObject &other);
    ~GagObject();

    QString id() const;
    void setId(const QString &id);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString title() const;
    void setTitle(const QString &title);

    QUrl imageUrl() const;
    void setImageUrl(const QUrl &imageUrl);

    QUrl fullImageUrl() const;
    void setFullImageUrl(const QUrl &fullImageUrl);

    QUrl gifImageUrl() const;
    void setGifImageUrl(const QUrl &imageUrl);

    QUrl videoUrl() const;
    void setVideoUrl(const QUrl &videoUrl);

    QSize imageSize() const;
    void setImageSize(const QSize &imageSize);

    int votesCount() const;
    void setVotesCount(int votes);

    int commentsCount() const;
    void setCommentsCount(int comments);

    int likes() const;
    void setLikes(int likes);

    bool isNSFW() const;
    void setIsNSFW(bool isNSFW);

    bool isGIF() const;
    void setIsGIF(bool isGIF);

    bool isVideo() const;
    void setIsVideo(bool isVideo);

    bool isPartialImage() const;
    void setIsPartialImage(bool isPartialImage);
    
private:
    QExplicitlySharedDataPointer<GagObjectData> d;
};

#endif // GAGOBJECT_H
