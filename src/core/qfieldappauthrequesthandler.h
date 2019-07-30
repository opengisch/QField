#ifndef QFIELDAPPAUTHREQUESTHANDLER_H
#define QFIELDAPPAUTHREQUESTHANDLER_H

// QGIS includes
#include <qgsapplication.h>

#include "qgsnetworkaccessmanager.h"

class QFieldAppAuthRequestHandler : public QObject, public QgsNetworkAuthenticationHandler
{
    Q_OBJECT

  public:
    //not used I guess
    static QFieldAppAuthRequestHandler *instance();

    QFieldAppAuthRequestHandler();
    void handleAuthRequest( QNetworkReply *reply, QAuthenticator *auth );

  signals:
    void loadProjectStarted( const QString &filename );

  private:
    static QFieldAppAuthRequestHandler *sAppAuthRequestHandler;

    //not used I guess
    static void setInstance( QFieldAppAuthRequestHandler *appAuthRequestHandler );
};

#endif // QFIELDAPPAUTHREQUESTHANDLER_H
