#include "votingmanager.h"

#include <QMetaEnum>
#include <QtNetwork/QNetworkReply>
#include <../qt-json/json.h>
#include <QDebug>

#include "gagbookmanager.h"
#include "networkmanager.h"

VotingManager::VotingManager(QObject *parent) :
    QObject(parent), m_manager(0)
{  
}

GagBookManager *VotingManager::manager() const
{
    return m_manager;
}

void VotingManager::setManager(GagBookManager *manager)
{
    m_manager = manager;
}

void VotingManager::vote(VoteType type, const QString &id)
{
    Q_ASSERT(!id.isEmpty());

    QUrl voteUrl("http://9gag.com/vote/"+ enumToString(type).toLower() +"/id/" + id);
    qDebug() << "vote url: " << voteUrl;
    QNetworkReply *m_reply = m_manager->networkManager()->createPostRequest(voteUrl, QByteArray());
    m_reply->setParent(this);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}

void VotingManager::setLike(const QString &id, bool liked)
{
    liked ? this->vote(Like,id) : this->vote(Unlike,id);
}

void VotingManager::setDislike(const QString &id, bool disliked)
{
    if (disliked)  //don't need to do anything when de-activating a Dislake
        this->vote(Dislike,id);
}

void VotingManager::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT_X(reply != 0, Q_FUNC_INFO, "Unable to cast sender() to QNetworkReply *");

    if (reply->error()) {
        emit failure(reply->errorString());
        reply->deleteLater();
        reply = 0;
        return;
    }

    QByteArray response = reply->readAll();
    reply->deleteLater();
    reply = 0;

    bool ok;
    const QVariantMap result = QtJson::parse(QString::fromUtf8(response), ok).toMap();

    Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");
    Q_ASSERT_X(!result.isEmpty(), Q_FUNC_INFO, "Error parsing JSON or JSON is empty");

    const QString msg = result.value("msg").toString();
    const QString score = result.value("myScore").toString();
    const QString id = result.value("id").toString();

    if(msg == "Loved")
        emit liked(id);

    else if(msg == "Invalid vote.")
        emit invalidVote();

    else if(msg == "Not loved") { //this msg will be in 2 different cases, unliked+disliked, diffrence being the score
        if (score == "-1")
            emit disliked(id);
        else if (score == "0")
            emit unliked(id);
        else
            emit invalidVote();
    }
    else
        emit invalidVote();

}


QString VotingManager::enumToString(VoteType aElement)
{
    int index = metaObject()->indexOfEnumerator("VoteType");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.valueToKey(aElement);
}
