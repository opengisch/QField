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

    //! Returns number of rows.
    int rowCount( const QModelIndex &parent ) const override;

    //! Returns the data at given \a index with given \a role.
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Returns the model role names.
    QHash<int, QByteArray> roleNames() const override;

    //! Returns the json document used to initialize the model.
    QJsonDocument json() const;

    //! Whether the model is valid and can be used.
    bool isValid() const;

    //! Holds the reason why it is invalid. Null string if not invalid.
    QString errorString() const;

    //! Whether all the deltas are in final status.
    bool allHaveFinalStatus() const;

    //! Returns a combined output for all deltas, separated by a new line.
    QString combinedOutput() const;

  private:
    QJsonDocument mJson;
    bool mIsValid = false;
    QString mErrorString;
    QList<Delta> mDeltas;
};

#endif // DELTALISTMODEL_H
