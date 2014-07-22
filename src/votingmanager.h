#ifndef VOTINGMANAGER_H
#define VOTINGMANAGER_H

#include <QObject>

class QNetworkReply;
class GagBookManager;

class VotingManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(VoteType)
    Q_PROPERTY(GagBookManager *manager READ manager WRITE setManager)
public:
    explicit VotingManager(QObject *parent = 0);

    enum VoteType {
        Like,
        Unlike,
        Dislike
    };

    GagBookManager *manager() const;
    void setManager(GagBookManager *manager);

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
    GagBookManager *m_manager;

    QString enumToString(VoteType aElement);
    void vote(VoteType type, const QString &id);
};

#endif // VOTINGMANAGER_H
