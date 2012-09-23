#ifndef QMLSETTINGS_H
#define QMLSETTINGS_H

#include <QSettings>

class QMLSettings : public QObject
{
    Q_OBJECT
public:
    explicit QMLSettings(QObject *parent = 0);
    Q_INVOKABLE QVariant read(const QString &key, const QVariant &defaultValue = QVariant());
    Q_INVOKABLE void store(const QString &key, const QVariant &value);

private:
    QSettings *settings;
};

#endif // QMLSETTINGS_H
