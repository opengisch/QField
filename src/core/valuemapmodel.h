#ifndef VALUEMAPMODEL_H
#define VALUEMAPMODEL_H

#include <QVariant>
#include <QAbstractListModel>

/**
 * A model that manages the key/value pairs for a ValueMap widget.
 */
class ValueMapModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * A list of QVariantMap, wrapped in a QVariant.
     *
     * Like this:
     *
     * [{'CH': 'Switzerland'}, {'DE': 'Germany'}, {'FR': 'France'}]
     */
    Q_PROPERTY( QVariant valueMap READ map WRITE setMap NOTIFY mapChanged )

    Q_ENUMS( ValueMapRoles )


  public:
    //! The roles provided by this model
    enum ValueMapRoles
    {
      KeyRole = Qt::UserRole + 1, //!< obtain the key
      ValueRole //!< obtain the value
    };

    /**
     * Create a new value map model
     */
    ValueMapModel( QObject *parent  = nullptr );

    /**
     * The map, see the property description
     */
    QVariant map() const;
    /**
     * The map, see the property description
     */
    void setMap( const QVariant &map );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    /**
     * Returns the row (index) of a key or -1 if not found.
     */
    Q_INVOKABLE int keyToIndex( const QVariant &key ) const;

    /**
     * Returns the key for a value or an invalid QVariant if not found.
     */
    Q_INVOKABLE QVariant keyForValue( const QString &value ) const;

  signals:
    /**
     * Emitted when the map changes.
     */
    void mapChanged();

  private:
    QList<QPair<QVariant, QString>> mMap;
    QVariant mConfiguredMap;
};

#endif // VALUEMAPMODEL_H
