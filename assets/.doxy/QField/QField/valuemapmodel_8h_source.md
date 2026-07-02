

# File valuemapmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**valuemapmodel.h**](valuemapmodel_8h.md)

[Go to the documentation of this file](valuemapmodel_8h.md)


```C++
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


class ValueMapModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QVariant valueMap READ map WRITE setMap NOTIFY mapChanged )


  public:
    enum ValueMapRoles
    {
      KeyRole = Qt::UserRole + 1, 
      ValueRole                   
    };

    Q_ENUM( ValueMapRoles )

    
    explicit ValueMapModel( QObject *parent = nullptr );

    QVariant map() const;

    void setMap( const QVariant &map );

    Q_INVOKABLE int keyToIndex( const QVariant &key ) const;

    Q_INVOKABLE QVariant keyForValue( const QString &value ) const;

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  signals:
    void mapChanged();

  private:
    ValueMapModelBase *mSourceModel = nullptr;
};

#endif // VALUEMAPMODEL_H
```


