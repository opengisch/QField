

# File qfappexpressioncontextscopesgenerator.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfappexpressioncontextscopesgenerator.cpp**](qfappexpressioncontextscopesgenerator_8cpp.md)

[Go to the documentation of this file](qfappexpressioncontextscopesgenerator_8cpp.md)


```C++
/***************************************************************************
 qfappexpressioncontextscopesgenerator.cpp - QfAppExpressionContextScopesGenerator

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

#include "qfappexpressioncontextscopesgenerator.h"
#include "qfexpressioncontextutils.h"

QfAppExpressionContextScopesGenerator::QfAppExpressionContextScopesGenerator( QObject *parent )
  : QObject( parent )
{
}

QfGnssPositionInformation QfAppExpressionContextScopesGenerator::positionInformation() const
{
  return mPositionInformation;
}

void QfAppExpressionContextScopesGenerator::setPositionInformation( const QfGnssPositionInformation &positionInformation )
{
  if ( mPositionInformation == positionInformation )
    return;

  mPositionInformation = positionInformation;
  emit positionInformationChanged();
}

bool QfAppExpressionContextScopesGenerator::positionLocked() const
{
  return mPositionLocked;
}

void QfAppExpressionContextScopesGenerator::setPositionLocked( bool positionLocked )
{
  if ( mPositionLocked == positionLocked )
    return;

  mPositionLocked = positionLocked;

  emit positionLockedChanged();
}

QfCloudUserInformation QfAppExpressionContextScopesGenerator::cloudUserInformation() const
{
  return mCloudUserInformation;
}

void QfAppExpressionContextScopesGenerator::setCloudUserInformation( const QfCloudUserInformation &cloudUserInformation )
{
  if ( mCloudUserInformation == cloudUserInformation )
    return;

  mCloudUserInformation = cloudUserInformation;
  emit cloudUserInformationChanged();
}

QList<QgsExpressionContextScope *> QfAppExpressionContextScopesGenerator::generate()
{
  QList<QgsExpressionContextScope *> scopes;

  if ( mPositionInformation.isValid() )
  {
    scopes << QfExpressionContextUtils::positionScope( mPositionInformation, mPositionLocked );
  }
  scopes << QfExpressionContextUtils::cloudUserScope( mCloudUserInformation );

  return scopes;
}
```


