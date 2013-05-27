#include "networkmanager.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "qmlutils.h"

static const QByteArray USER_AGENT = QByteArray("GagBook/") + APP_VERSION;

QScopedPointer<NetworkManager> NetworkManager::m_instance(new NetworkManager);

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent), m_networkAccessManager(new QNetworkAccessManager(this))
{
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(trackDownloadSize(QNetworkReply*)));
}

QNetworkReply *NetworkManager::createGetRequest(const QUrl &url)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", USER_AGENT);

    return m_instance->m_networkAccessManager->get(request);
}

void NetworkManager::trackDownloadSize(QNetworkReply *reply)
{
    QMLUtils::instance()->increaseDataDownloaded(reply->size());
}
