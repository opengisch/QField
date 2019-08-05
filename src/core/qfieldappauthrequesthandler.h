#ifndef QFIELDAPPAUTHREQUESTHANDLER_H
#define QFIELDAPPAUTHREQUESTHANDLER_H

// QGIS includes
#include <qgsapplication.h>

#include "qgsnetworkaccessmanager.h"

class QFieldAppAuthRequestHandler : public QObject, public QgsNetworkAuthenticationHandler
{
    Q_OBJECT

  public:

    QFieldAppAuthRequestHandler();
    void handleAuthRequest( QNetworkReply *reply, QAuthenticator *auth );

  signals:
    void loadProjectStarted( const QString &filename );
    void authNeeded( const QString &realm );
};

#endif // QFIELDAPPAUTHREQUESTHANDLER_H
