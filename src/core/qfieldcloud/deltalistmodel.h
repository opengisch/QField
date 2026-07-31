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

#include "qfieldcloudconnection.h"

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QUuid>

/**
 * \ingroup core
 */
class DeltaListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QFieldCloudConnection *cloudConnection READ cloudConnection WRITE setCloudConnection NOTIFY cloudConnectionChanged )
    Q_PROPERTY( QString cloudProjectId READ cloudProjectId WRITE setCloudProjectId NOTIFY cloudProjectIdChanged )

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

    explicit DeltaListModel( QJsonDocument deltasStatusList = QJsonDocument(), QObject *parent = nullptr );

    //! Returns number of rows.
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    //! Returns the data at given \a index with given \a role.
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Returns the model role names.
    QHash<int, QByteArray> roleNames() const override;

    //! Returns the json document used to initialize the model.
    QJsonDocument json() const;

    //! Holds the reason why it is invalid. Null string if not invalid.
    QString errorString() const;

    QFieldCloudConnection *cloudConnection() const { return mCloudConnection; }

    void setCloudConnection( QFieldCloudConnection *cloudConnection );

    QString cloudProjectId() const { return mCloudProjectId; }

    void setCloudProjectId( const QString &cloudProjectId );

    //! Whether the model is valid and can be used.
    Q_INVOKABLE bool isValid() const;

    //! Whether all the deltas are in final status.
    Q_INVOKABLE bool allHaveFinalStatus() const;

    //! Refreshes the delta list model
    Q_INVOKABLE void refresh();

  signals:
    void cloudConnectionChanged();
    void cloudProjectIdChanged();

  private:
    void parseJson();

    QJsonDocument mJson;
    bool mIsValid = false;
    QString mErrorString;
    QList<Delta> mDeltas;

    QFieldCloudConnection *mCloudConnection = nullptr;
    QString mCloudProjectId;
};

#endif // DELTALISTMODEL_H
