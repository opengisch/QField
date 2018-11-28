#ifndef LOCATORMODELSUPERBRIDGE_H
#define LOCATORMODELSUPERBRIDGE_H

#include <QStandardItemModel>
#include <qgslocatormodelbridge.h>

class LocatorActionsModel : public QStandardItemModel
{
    Q_OBJECT
  public:
    enum ActionRoles
    {
      IconRole = Qt::UserRole + 1
    };
    explicit LocatorActionsModel( QObject *parent = nullptr );
    LocatorActionsModel( int rows, int columns, QObject *parent = nullptr );
    QHash<int, QByteArray> roleNames() const override;
};

class LocatorModelSuperBridge : public QgsLocatorModelBridge
{
    Q_OBJECT
  public:
    explicit LocatorModelSuperBridge( QObject *parent = nullptr );
    ~LocatorModelSuperBridge() = default;

    Q_INVOKABLE LocatorActionsModel *contextMenuActionsModel( const int row );

  public slots:
    Q_INVOKABLE void triggerResultAtRow( const int row );
};

#endif // LOCATORMODELSUPERBRIDGE_H
