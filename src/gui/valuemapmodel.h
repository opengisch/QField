/***************************************************************************
                            valuemapmodel.h

                              -------------------
              begin                : March 2019
              copyright            : (C) 2019 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VALUEMAPMODEL_H
#define VALUEMAPMODEL_H

#include "valuemapmodelbase.h"

#include <QSortFilterProxyModel>


/**
 * A model that manages the key/value pairs for a ValueMap widget.
 * \ingroup core
 */
class ValueMapModel : public QSortFilterProxyModel
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


  public:
    //! The roles provided by this model
    enum ValueMapRoles
    {
      KeyRole = Qt::UserRole + 1, //!< obtain the key
      ValueRole                   //!< obtain the value
    };

    Q_ENUM( ValueMapRoles )

    /**
     * Create a new value map model
     */
    explicit ValueMapModel( QObject *parent = nullptr );

    /**
     * The map, see the property description
     */
    QVariant map() const;

    /**
     * The map, see the property description
     */
    void setMap( const QVariant &map );

    /**
     * Returns the row (index) of a key or -1 if not found.
     */
    Q_INVOKABLE int keyToIndex( const QVariant &key ) const;

    /**
     * Returns the key for a value or an invalid QVariant if not found.
     */
    Q_INVOKABLE QVariant keyForValue( const QString &value ) const;

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  signals:
    /**
     * Emitted when the map changes.
     */
    void mapChanged();

  private:
    ValueMapModelBase *mSourceModel = nullptr;
};

#endif // VALUEMAPMODEL_H
