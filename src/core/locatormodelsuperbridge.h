#ifndef LOCATORMODELSUPERBRIDGE_H
#define LOCATORMODELSUPERBRIDGE_H

#include <QStandardItemModel>
#include <qgslocatormodelbridge.h>

#include "locatorhighlight.h"
#include "multifeaturelistmodel.h"

class QgsQuickMapSettings;

class LocatorActionsModel : public QStandardItemModel
{
    Q_OBJECT
  public:
    enum ActionRoles
    {
      IdRole = Qt::UserRole + 1,
      IconPathRole
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
    Q_PROPERTY( MultiFeatureListModel* model READ model WRITE setModel NOTIFY modelChanged )

  public:
    explicit LocatorModelSuperBridge( QObject *parent = nullptr );
    ~LocatorModelSuperBridge() = default;

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    LocatorHighlight *locatorHighlight() const;
    void setLocatorHighlight( LocatorHighlight *locatorHighlight );

    MultiFeatureListModel* model() const;
    void setModel( MultiFeatureListModel* model );

    Q_INVOKABLE LocatorActionsModel *contextMenuActionsModel( const int row );

  signals:
    void mapSettingsChanged();
    void locatorHighlightChanged();
    void modelChanged();

  public slots:
    Q_INVOKABLE void triggerResultAtRow( const int row, const int id = -1 );

  private:
    QgsQuickMapSettings *mMapSettings = nullptr;
    LocatorHighlight *mLocatorHighlight = nullptr;
    MultiFeatureListModel* mModel = nullptr;
};

#endif // LOCATORMODELSUPERBRIDGE_H
