

# File qfvaluemapmodel.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfvaluemapmodel.h**](qfvaluemapmodel_8h.md)

[Go to the documentation of this file](qfvaluemapmodel_8h.md)


```C++
/***************************************************************************
                            qfvaluemapmodel.h

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

#ifndef QFVALUEMAPMODEL_H
#define QFVALUEMAPMODEL_H

#include "qfvaluemapmodelbase.h"

#include <QSortFilterProxyModel>


class QfValueMapModel : public QSortFilterProxyModel
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

    
    explicit QfValueMapModel( QObject *parent = nullptr );

    QVariant map() const;

    void setMap( const QVariant &map );

    Q_INVOKABLE int keyToIndex( const QVariant &key ) const;

    Q_INVOKABLE QVariant keyForValue( const QString &value ) const;

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  signals:
    void mapChanged();

  private:
    QfValueMapModelBase *mSourceModel = nullptr;
};

#endif // QFVALUEMAPMODEL_H
```


