

# File qfpermissions.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfpermissions.h**](qfpermissions_8h.md)

[Go to the documentation of this file](qfpermissions_8h.md)


```C++
/***************************************************************************
 qfpermissions.h - QfCameraPermission, QfMicrophonePermission

 ---------------------
 begin                : 01.12.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFPERMISSIONS_H
#define QFPERMISSIONS_H

#include <QObject>
#include <QPermission>

class QfCameraPermission : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Qt::PermissionStatus status READ status NOTIFY statusChanged )

  public:
    explicit QfCameraPermission( QObject *parent = nullptr );

    Qt::PermissionStatus status();

    Q_INVOKABLE void request();

  signals:
    void statusChanged();

  private:
    Qt::PermissionStatus mStatus = Qt::PermissionStatus::Undetermined;
    bool mStatusChecked = false;
};

class QfMicrophonePermission : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Qt::PermissionStatus status READ status NOTIFY statusChanged )

  public:
    explicit QfMicrophonePermission( QObject *parent = nullptr );

    Qt::PermissionStatus status();

    Q_INVOKABLE void request();

  signals:
    void statusChanged();

  private:
    Qt::PermissionStatus mStatus = Qt::PermissionStatus::Undetermined;
    bool mStatusChecked = false;
};

#endif // QFPERMISSIONS_H
```


