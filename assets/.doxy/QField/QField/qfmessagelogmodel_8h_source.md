

# File qfmessagelogmodel.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfmessagelogmodel.h**](qfmessagelogmodel_8h.md)

[Go to the documentation of this file](qfmessagelogmodel_8h.md)


```C++
/***************************************************************************
  qfmessagelogmodel.h - QfMessageLogModel

 ---------------------
 begin                : 13.7.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFMESSAGELOGMODEL_H
#define QFMESSAGELOGMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <qgsmessagelog.h>

class QfMessageLogModel : public QAbstractListModel
{
    Q_OBJECT

    struct LogMessage
    {
        LogMessage()
        {}

        LogMessage( const QString &tag, const QString &message, Qgis::MessageLevel level )
          : tag( tag )
          , message( message )
          , level( level )
          , datetime( QDateTime::currentDateTime().toString( QStringLiteral( "yyyy-MM-dd hh:mm:ss:zzz" ) ) )
        {
        }

        QString tag;
        QString message;
        Qgis::MessageLevel level;
        QString datetime;
    };

    enum Roles
    {
      MessageRole = Qt::UserRole,
      MessageTagRole,
      MessageLevelRole,
      MessageDateTimeRole
    };

  public:
    explicit QfMessageLogModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void suppress( const QVariantMap &filters );
    Q_INVOKABLE void unsuppress( const QVariantMap &filters );

    Q_INVOKABLE void clear();

  private slots:
    void onMessageReceived( const QString &message, const QString &tag, Qgis::MessageLevel level, Qgis::StringFormat format );

  private:
    QgsMessageLog *mMessageLog = nullptr;
    QVector<LogMessage> mMessages;
    QMap<QString, QStringList> mSuppressedFilters;
};

#endif // QFMESSAGELOGMODEL_H
```


