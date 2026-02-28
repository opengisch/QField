

# File changelogcontents.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**changelogcontents.h**](changelogcontents_8h.md)

[Go to the documentation of this file](changelogcontents_8h.md)


```C++
/***************************************************************************
  changelogcontents.h - Changelog

 ---------------------
 begin                : Nov 2020
 copyright            : (C) 2020 by Ivan Ivanov
 email                : ivan@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef CHANGELOGCONTENTS_H
#define CHANGELOGCONTENTS_H

#include <QObject>

class ChangelogContents : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString markdown READ markdown NOTIFY markdownChanged )

    
    Q_PROPERTY( Status status READ status NOTIFY statusChanged )

  public:
    explicit ChangelogContents( QObject *parent = nullptr );

    enum Status
    {
      IdleStatus,
      LoadingStatus,
      SuccessStatus,
      ErrorStatus,
    };

    Q_ENUM( Status )

    
    Q_INVOKABLE void request();

    QString markdown();

    Status status();

  signals:

    void markdownChanged();

    void statusChanged();

  private:
    QList<int> parseVersion( const QString &version );

    QString mMarkdown;

    Status mStatus = Status::IdleStatus;
};

#endif // CHANGELOGCONTENTS_H
```


