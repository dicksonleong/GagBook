#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

class QNetworkAccessManager;
class QNetworkReply;
class QUrl;

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    static QNetworkReply *createGetRequest(const QUrl &url);

private slots:
    void trackDownloadSize(QNetworkReply *reply);

private:
    static QScopedPointer<NetworkManager> m_instance;

    explicit NetworkManager(QObject *parent = 0);
    Q_DISABLE_COPY(NetworkManager)

    QNetworkAccessManager *m_networkAccessManager;
};

#endif // NETWORKMANAGER_H
