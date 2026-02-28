

# File deltalistmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**deltalistmodel.h**](deltalistmodel_8h.md)

[Go to the documentation of this file](deltalistmodel_8h.md)


```C++
/***************************************************************************
    deltalistmodel.h
    ---------------------
    begin                : December 2020
    copyright            : (C) 2020 by Ivan Ivanov
    email                : ivan at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DELTALISTMODEL_H
#define DELTALISTMODEL_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QUuid>

class DeltaListModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Status
    {
      PendingStatus,
      BusyStatus,
      AppliedStatus,
      ConflictStatus,
      NotAppliedStatus,
      ErrorStatus,
      IgnoredStatus,
      UnpermittedStatus,
    };

    Q_ENUM( Status )

    enum ColumnRole
    {
      IdRole,
      DeltafileIdRole,
      CreatedAtRole,
      UpdatedAtRole,
      StatusRole,
      OutputRole,
    };

    Q_ENUM( ColumnRole )

    struct Delta
    {
        QUuid id;
        QUuid deltafileId;
        QString createdAt;
        QString updatedAt;
        Status status;
        QString output;
    };

    DeltaListModel() = default;
    explicit DeltaListModel( QJsonDocument deltasStatusList );

    Q_PROPERTY( int rowCount READ rowCount NOTIFY rowCountChanged )

    
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role ) const override;

    QHash<int, QByteArray> roleNames() const override;

    QJsonDocument json() const;

    bool isValid() const;

    QString errorString() const;

    bool allHaveFinalStatus() const;

    QString combinedOutput() const;

  signals:
    void rowCountChanged();

  private:
    QJsonDocument mJson;
    bool mIsValid = false;
    QString mErrorString;
    QList<Delta> mDeltas;
};

#endif // DELTALISTMODEL_H
```


