#ifndef SHAREUI_H
#define SHAREUI_H

#include <QObject>

class ShareUI : public QObject
{
    Q_OBJECT
public:
    explicit ShareUI(QObject *parent = 0);
    
    Q_INVOKABLE void share(const QString &title, const QString &link);
signals:
    
public slots:
    
};

#endif // SHAREUI_H
