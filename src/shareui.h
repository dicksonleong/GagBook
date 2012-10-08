#ifndef SHAREUI_H
#define SHAREUI_H

#include <QObject>

class ShareUI : public QObject
{
    Q_OBJECT
public:
    explicit ShareUI(QObject *parent = 0);

    Q_INVOKABLE void shareLink(const QString &link, const QString &title = QString());
};

#endif // SHAREUI_H
