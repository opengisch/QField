#ifndef VALUEMAPMODEL_H
#define VALUEMAPMODEL_H

#include <QVariant>
#include <QAbstractListModel>

class ValueMapModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QVariant valueMap READ map WRITE setMap NOTIFY mapChanged )

    Q_ENUMS( ValueMapRoles )


  public:
    enum ValueMapRoles
    {
      KeyRole = Qt::UserRole + 1,
      ValueRole
    };


    ValueMapModel( QObject *parent  = nullptr );

    QVariant map() const;
    void setMap( const QVariant &map );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int keyToIndex( const QVariant &value ) const;
    Q_INVOKABLE QVariant keyForValue( const QString &value ) const;

  private:
    QList<QPair<QVariant, QString>> mMap;
    QVariant mConfiguredMap;

  signals:
    void mapChanged();

};

#endif // VALUEMAPMODEL_H
