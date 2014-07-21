#ifndef VOTINGMANAGER_H
#define VOTINGMANAGER_H

#include <QObject>

class NetworkManager;
class QNetworkReply;

class VotingManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(VoteType)

public:
    explicit VotingManager(QObject *parent = 0);

    enum VoteType {
        Like,
        Unlike,
        Dislike
    };

    Q_INVOKABLE void setLike(const QString &id, bool liked);
    Q_INVOKABLE void setDislike(const QString &id, bool disliked);

private slots:
    void onReplyFinished();

signals:
    void liked(const QString &id);
    void unliked(const QString &id);
    void disliked(const QString &id);
    void invalidVote();
    void failure(const QString &errorMessage);

private:
    QString enumToString(VoteType aElement);
    NetworkManager *m_netManager;
    void vote(VoteType type, const QString &id);


};

#endif // VOTINGMANAGER_H
