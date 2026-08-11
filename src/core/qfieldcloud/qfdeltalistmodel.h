/***************************************************************************
    qfdeltalistmodel.h
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

#ifndef QFDELTALISTMODEL_H
#define QFDELTALISTMODEL_H

#include "qfcloudconnection.h"
#include "qfcloudutils.h"

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QUuid>

/**
 * \ingroup core
 */
class QfDeltaListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( bool isValid READ isValid NOTIFY isValidChanged )
    Q_PROPERTY( bool isRefreshing READ isRefreshing NOTIFY isRefreshingChanged )

    Q_PROPERTY( QfCloudConnection *cloudConnection READ cloudConnection WRITE setCloudConnection NOTIFY cloudConnectionChanged )
    Q_PROPERTY( QString cloudProjectId READ cloudProjectId WRITE setCloudProjectId NOTIFY cloudProjectIdChanged )

    Q_PROPERTY( QString errorString READ errorString NOTIFY errorStringChanged )

  public:
    enum ColumnRole
    {
      IdRole,
      DeltafileIdRole,
      CreatedByRole,
      CreatedAtRole,
      UpdatedAtRole,
      StatusRole,
      SummaryRole,
      OutputRole,
    };
    Q_ENUM( ColumnRole )

    explicit QfDeltaListModel();

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

    QfCloudConnection *cloudConnection() const { return mCloudConnection; }

    void setCloudConnection( QfCloudConnection *cloudConnection );

    QString cloudProjectId() const { return mCloudProjectId; }

    void setCloudProjectId( const QString &cloudProjectId );

    //! Whether the model is valid and can be used.
    bool isValid() const;

    //! Whether the model is refreshing.
    bool isRefreshing() const;

    //! Refreshes the delta list model
    Q_INVOKABLE void refresh();

  signals:
    void isValidChanged();
    void isRefreshingChanged();

    void errorStringChanged();

    void cloudConnectionChanged();
    void cloudProjectIdChanged();

  private:
    void setIsRefreshing( bool isRefreshing );

    bool mIsValid = false;
    bool mIsRefreshing = false;

    QJsonDocument mJson;
    QString mErrorString;

    QList<QfCloudDelta> mDeltas;

    QfCloudConnection *mCloudConnection = nullptr;
    QString mCloudProjectId;
};

#endif // QFDELTALISTMODEL_H
