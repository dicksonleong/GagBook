#ifndef GAGMANAGER_H
#define GAGMANAGER_H

#include <QtCore/QObject>

#include "gagobject.h"

class QNetworkAccessManager;
class GagRequest;
class GagModel;

class GagManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(RefreshType)

    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(GagModel* model READ model WRITE setModel NOTIFY modelChanged)
public:
    explicit GagManager(QObject *parent = 0);

    enum RefreshType {
        RefreshAll,
        RefreshOlder
    };

    Q_INVOKABLE void refresh(RefreshType refreshType);

    bool isBusy() const;
    void setBusy(bool busy);

    GagModel *model() const;
    void setModel(GagModel *model);

signals:
    void refreshFailure(const QString &errorMessage);
    void modelChanged();
    void busyChanged();

private slots:
    void onSuccess(const QList<GagObject> &gagList);
    void onFailure(const QString &errorMessage);

private:
    QNetworkAccessManager *m_netManager;
    GagRequest *m_request;

    bool m_busy;
    GagModel *m_model;
};

#endif // GAGMANAGER_H
