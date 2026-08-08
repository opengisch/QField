

# File appexpressioncontextscopesgenerator.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**appexpressioncontextscopesgenerator.cpp**](appexpressioncontextscopesgenerator_8cpp.md)

[Go to the documentation of this file](appexpressioncontextscopesgenerator_8cpp.md)


```C++
/***************************************************************************
 appexpressioncontextscopesgenerator.cpp - AppExpressionContextScopesGenerator

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

#include "appexpressioncontextscopesgenerator.h"
#include "expressioncontextutils.h"

AppExpressionContextScopesGenerator::AppExpressionContextScopesGenerator( QObject *parent )
  : QObject( parent )
{
}

GnssPositionInformation AppExpressionContextScopesGenerator::positionInformation() const
{
  return mPositionInformation;
}

void AppExpressionContextScopesGenerator::setPositionInformation( const GnssPositionInformation &positionInformation )
{
  if ( mPositionInformation == positionInformation )
    return;

  mPositionInformation = positionInformation;
  emit positionInformationChanged();
}

bool AppExpressionContextScopesGenerator::positionLocked() const
{
  return mPositionLocked;
}

void AppExpressionContextScopesGenerator::setPositionLocked( bool positionLocked )
{
  if ( mPositionLocked == positionLocked )
    return;

  mPositionLocked = positionLocked;

  emit positionLockedChanged();
}

CloudUserInformation AppExpressionContextScopesGenerator::cloudUserInformation() const
{
  return mCloudUserInformation;
}

void AppExpressionContextScopesGenerator::setCloudUserInformation( const CloudUserInformation &cloudUserInformation )
{
  if ( mCloudUserInformation == cloudUserInformation )
    return;

  mCloudUserInformation = cloudUserInformation;
  emit cloudUserInformationChanged();
}

QList<QgsExpressionContextScope *> AppExpressionContextScopesGenerator::generate()
{
  QList<QgsExpressionContextScope *> scopes;

  if ( mPositionInformation.isValid() )
  {
    scopes << ExpressionContextUtils::positionScope( mPositionInformation, mPositionLocked );
  }
  scopes << ExpressionContextUtils::cloudUserScope( mCloudUserInformation );

  return scopes;
}
```


