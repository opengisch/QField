#ifndef LAYERLOGINHANDLER_H
#define LAYERLOGINHANDLER_H

#include <QStandardItemModel>

class QgsProject;

class LayerLoginHandler : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

  public:
    enum Roles
    {
      DataSourceRole = Qt::UserRole,
      LayerNameRole,
      LayerIdRole
    };

    LayerLoginHandler( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    QgsProject *project() const;
    void setProject( QgsProject *project );

    Q_INVOKABLE void enterCredentials( const QString realm, const QString username, const QString password );
    Q_INVOKABLE void reloadLayers();
    Q_INVOKABLE void handleLayerLogins();

  signals:
    void projectChanged();
    void showLoginDialog( const QString realm );
    void loginDialogClosed( const QString realm );
    void reloadEverything();

  private:
    QgsProject *mProject;
    QStringList mRealmList;
};


#endif // LAYERLOGINHANDLER_H
