#include "gagmanager.h"

#include <QtNetwork/QNetworkAccessManager>

#include "gagmodel.h"
#include "gagrequest.h"
#include "settings.h"

GagManager::GagManager(QObject *parent) :
    QObject(parent), m_netManager(new QNetworkAccessManager(this)), m_request(0),
    m_busy(false), m_model(0)
{
}

void GagManager::refresh(RefreshType refreshType)
{
    Q_ASSERT_X(m_model != 0, Q_FUNC_INFO, "model not set");

    if (m_request != 0) {
        m_request->disconnect();
        m_request->deleteLater();
        m_request = 0;
    }

    if (m_model->isEmpty())
        refreshType = RefreshAll;
    else if (refreshType == RefreshAll)
        m_model->clear();

    m_request = new GagRequest(m_netManager, this);
    m_request->setSection(static_cast<GagRequest::Section>(Settings::instance()->selectedSection()));
    if (refreshType == RefreshOlder)
        m_request->setLastId(m_model->lastGagId());

    connect(m_request, SIGNAL(success(QList<GagObject>)), this, SLOT(onSuccess(QList<GagObject>)));
    connect(m_request, SIGNAL(failure(QString)), this, SLOT(onFailure(QString)));

    m_request->send();

    setBusy(true);
}

bool GagManager::isBusy() const
{
    return m_busy;
}

void GagManager::setBusy(bool busy)
{
    if (m_busy != busy) {
        m_busy = busy;
        emit busyChanged();
    }
}

GagModel *GagManager::model() const
{
    return m_model;
}

void GagManager::setModel(GagModel *model)
{
    if (m_model != model) {
        m_model = model;
        emit modelChanged();
    }
}

void GagManager::onSuccess(const QList<GagObject> &gagList)
{
    m_model->append(gagList);
    m_request->deleteLater();
    m_request = 0;
    setBusy(false);
}

void GagManager::onFailure(const QString &errorMessage)
{
    emit refreshFailure(errorMessage);
    m_request->deleteLater();
    m_request = 0;
    setBusy(false);
}
