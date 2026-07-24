

# File appexpressioncontextscopesgenerator.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**appexpressioncontextscopesgenerator.h**](appexpressioncontextscopesgenerator_8h.md)

[Go to the documentation of this file](appexpressioncontextscopesgenerator_8h.md)


```C++
/***************************************************************************
 appexpressioncontextscopesgenerator.h - AppExpressionContextScopesGenerator

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

#ifndef APPEXPRESSIONCONTEXTSCOPESGENERATOR_H
#define APPEXPRESSIONCONTEXTSCOPESGENERATOR_H

#include "gnsspositioninformation.h"
#include "qfieldcloudutils.h"

#include <QObject>

class QgsExpressionContextScope;

class AppExpressionContextScopesGenerator : public QObject
{
    Q_OBJECT

    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation WRITE setPositionInformation NOTIFY positionInformationChanged )
    Q_PROPERTY( bool positionLocked READ positionLocked WRITE setPositionLocked NOTIFY positionLockedChanged )
    Q_PROPERTY( CloudUserInformation cloudUserInformation READ cloudUserInformation WRITE setCloudUserInformation NOTIFY cloudUserInformationChanged )

  public:
    explicit AppExpressionContextScopesGenerator( QObject *parent = nullptr );

    GnssPositionInformation positionInformation() const;

    void setPositionInformation( const GnssPositionInformation &positionInformation );

    bool positionLocked() const;

    void setPositionLocked( bool positionLocked );

    CloudUserInformation cloudUserInformation() const;

    void setCloudUserInformation( const CloudUserInformation &cloudUserInformation );

    QList<QgsExpressionContextScope *> generate();

  signals:
    void positionInformationChanged();
    void positionLockedChanged();
    void cloudUserInformationChanged();

  private:
    GnssPositionInformation mPositionInformation;
    bool mPositionLocked = false;
    CloudUserInformation mCloudUserInformation;
};

#endif // APPEXPRESSIONCONTEXTSCOPESGENERATOR_H
```


