

# File valuemapmodelbase.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**valuemapmodelbase.h**](valuemapmodelbase_8h.md)

[Go to the documentation of this file](valuemapmodelbase_8h.md)


```C++
/***************************************************************************
                            valuemapmodelbase.h

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

#ifndef VALUEMAPMODELBASE_H
#define VALUEMAPMODELBASE_H

#include <QAbstractListModel>
#include <QVariant>

class ValueMapModelBase : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit ValueMapModelBase( QObject *parent = nullptr );

    QVariant map() const;

    void setMap( const QVariant &map );

    int keyToIndex( const QVariant &key ) const;

    QVariant keyForValue( const QString &value ) const;

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

  signals:
    void mapChanged();

  private:
    QList<QPair<QVariant, QString>> mMap;
    QVariant mConfiguredMap;
};

#endif // VALUEMAPMODELBASE_H
```


