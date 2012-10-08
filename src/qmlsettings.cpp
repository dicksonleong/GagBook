#include "qmlsettings.h"

QMLSettings::QMLSettings(QObject *parent) :
    QObject(parent), settings(new QSettings(this))
{
}

QVariant QMLSettings::read(const QString &key, const QVariant &defaultValue)
{
    return settings->value(key, defaultValue);
}

void QMLSettings::store(const QString &key, const QVariant &value)
{
    settings->setValue(key, value);
}
