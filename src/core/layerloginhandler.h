#ifndef LAYERLOGINHANDLER_H
#define LAYERLOGINHANDLER_H

#include <QObject>

class LayerLoginHandler : public QObject
{
    Q_OBJECT

  public:
    LayerLoginHandler();

    Q_INVOKABLE void enterCredentials( const QString realm, const QString username, const QString password );
    Q_INVOKABLE void handleLayerLogins();

  signals:
    void showLoginDialog( const QString realm );
    void loginDialogClosed( const QString realm );
    void reloadEverything();

  private:
    QStringList mRealmList;
};


#endif // LAYERLOGINHANDLER_H
