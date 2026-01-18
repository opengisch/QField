

# File messagelogmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**messagelogmodel.h**](messagelogmodel_8h.md)

[Go to the documentation of this file](messagelogmodel_8h.md)


```C++
/***************************************************************************
  messagelogmodel.h - MessageLogModel

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
#ifndef MESSAGELOGMODEL_H
#define MESSAGELOGMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <qgsmessagelog.h>

class MessageLogModel : public QAbstractListModel
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
    explicit MessageLogModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void suppress( const QVariantMap &filters );
    Q_INVOKABLE void unsuppress( const QVariantMap &filters );

    Q_INVOKABLE void clear();

  private slots:
    void onMessageReceived( const QString &message, const QString &tag, Qgis::MessageLevel level );

  private:
    QgsMessageLog *mMessageLog = nullptr;
    QVector<LogMessage> mMessages;
    QMap<QString, QStringList> mSuppressedFilters;
};

#endif // MESSAGELOGMODEL_H
```


