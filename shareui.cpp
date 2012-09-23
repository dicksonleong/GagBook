#include "shareui.h"

// Not sure why MEEGO_EDITION_HARMATTAN marco is not working for me,
// so I use Linux marco since I developing in Windows
//#ifdef MEEGO_EDITION_HARMATTAN
#ifdef Q_OS_LINUX
#include <MDataUri>
#include <maemo-meegotouch-interfaces/shareuiinterface.h>
#endif

ShareUI::ShareUI(QObject *parent) :
    QObject(parent)
{
}

void ShareUI::share(const QString &title, const QString &link)
{
//#ifdef MEEGO_EDITION_HARMATTAN
#ifdef Q_OS_LINUX
    MDataUri uri;
    uri.setMimeType("text/x-url");
    uri.setAttribute ("title", title);
    uri.setTextData(link);

    if(!uri.isValid()){
        qCritical() << "Invalid URI";
        return;
    }

    ShareUiInterface shareIf("com.nokia.ShareUi");

    if(!shareIf.isValid()){
        qCritical() << "Invalid Share UI interface";
        return;
    }

    shareIf.share(QStringList() << uri.toString());
#else
    Q_UNUSED(title)
    Q_UNUSED(link)
#endif
}
