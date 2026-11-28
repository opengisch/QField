

# File qfappexpressioncontextscopesgenerator.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfappexpressioncontextscopesgenerator.h**](qfappexpressioncontextscopesgenerator_8h.md)

[Go to the documentation of this file](qfappexpressioncontextscopesgenerator_8h.md)


```C++
/***************************************************************************
 qfappexpressioncontextscopesgenerator.h - QfAppExpressionContextScopesGenerator

 ---------------------
 begin                : 26.03.2025
 copyright            : (C) 2025 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFAPPEXPRESSIONCONTEXTSCOPESGENERATOR_H
#define QFAPPEXPRESSIONCONTEXTSCOPESGENERATOR_H

#include "qfcloudutils.h"
#include "qfgnsspositioninformation.h"

#include <QObject>

class QgsExpressionContextScope;

class QfAppExpressionContextScopesGenerator : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QfGnssPositionInformation positionInformation READ positionInformation WRITE setPositionInformation NOTIFY positionInformationChanged )
    Q_PROPERTY( bool positionLocked READ positionLocked WRITE setPositionLocked NOTIFY positionLockedChanged )
    Q_PROPERTY( QfCloudUserInformation cloudUserInformation READ cloudUserInformation WRITE setCloudUserInformation NOTIFY cloudUserInformationChanged )

  public:
    explicit QfAppExpressionContextScopesGenerator( QObject *parent = nullptr );

    QfGnssPositionInformation positionInformation() const;

    void setPositionInformation( const QfGnssPositionInformation &positionInformation );

    bool positionLocked() const;

    void setPositionLocked( bool positionLocked );

    QfCloudUserInformation cloudUserInformation() const;

    void setCloudUserInformation( const QfCloudUserInformation &cloudUserInformation );

    QList<QgsExpressionContextScope *> generate();

  signals:
    void positionInformationChanged();
    void positionLockedChanged();
    void cloudUserInformationChanged();

  private:
    QfGnssPositionInformation mPositionInformation;
    bool mPositionLocked = false;
    QfCloudUserInformation mCloudUserInformation;
};

#endif // QFAPPEXPRESSIONCONTEXTSCOPESGENERATOR_H
```


