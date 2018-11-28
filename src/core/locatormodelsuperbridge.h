#ifndef LOCATORMODELSUPERBRIDGE_H
#define LOCATORMODELSUPERBRIDGE_H

#include <QStandardItemModel>
#include <qgslocatormodelbridge.h>

class LocatorModelSuperBridge : public QgsLocatorModelBridge
{
    Q_OBJECT
  public:
    explicit LocatorModelSuperBridge( QObject *parent = nullptr );
    ~LocatorModelSuperBridge() = default;

    Q_INVOKABLE QStandardItemModel *contextMenuActionsModel( const int row );

  public slots:
    Q_INVOKABLE void triggerResultAtRow( const int row );
};

#endif // LOCATORMODELSUPERBRIDGE_H
