#pragma once

#include "positioning.h"

#include <QObject>
#include <QStandardItemModel>

class PositioningModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY( Positioning *positioningSource READ positioningSource WRITE setPositioningSource NOTIFY positioningSourceChanged )

  public:
    enum Roles
    {
      VariableNameRole = Qt::UserRole,
      VariableValueRole = Qt::UserRole + 1,
    };

    PositioningModel( QObject *parent = nullptr );

    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setupConnections();
    void refreshData();
    void updateInfo( const QString &name, const QVariant &value );


    Positioning *positioningSource() const;
    void setPositioningSource( Positioning *newPositioningSource );


  signals:
    void positioningSourceChanged();

  private slots:
    void onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );

  private:
    Positioning *mPositioningSource = nullptr;
};
