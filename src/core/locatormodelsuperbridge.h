#ifndef LOCATORMODELSUPERBRIDGE_H
#define LOCATORMODELSUPERBRIDGE_H

#include <QStandardItemModel>
#include <qgslocatormodelbridge.h>

#include "locatorhighlight.h"

class QgsQuickMapSettings;

class LocatorActionsModel : public QStandardItemModel
{
    Q_OBJECT
  public:
    enum ActionRoles
    {
      IdRole = Qt::UserRole + 1,
      IconRole
    };
    explicit LocatorActionsModel( QObject *parent = nullptr );
    LocatorActionsModel( int rows, int columns, QObject *parent = nullptr );
    QHash<int, QByteArray> roleNames() const override;
};

class LocatorModelSuperBridge : public QgsLocatorModelBridge
{
    Q_OBJECT
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( LocatorHighlight *locatorHighlight READ locatorHighlight WRITE setLocatorHighlight NOTIFY locatorHighlightChanged )

  public:
    explicit LocatorModelSuperBridge( QObject *parent = nullptr );
    ~LocatorModelSuperBridge() = default;

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    LocatorHighlight *locatorHighlight() const;
    void setLocatorHighlight( LocatorHighlight *locatorHighlight );

    Q_INVOKABLE LocatorActionsModel *contextMenuActionsModel( const int row );

  signals:
    void mapSettingsChanged();
    void locatorHighlightChanged();

  public slots:
    Q_INVOKABLE void triggerResultAtRow( const int row, const int id = -1 );

  private:
    QgsQuickMapSettings *mMapSettings = nullptr;
    LocatorHighlight *mLocatorHighlight = nullptr;
};

#endif // LOCATORMODELSUPERBRIDGE_H
