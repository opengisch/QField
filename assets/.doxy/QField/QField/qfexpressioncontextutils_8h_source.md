

# File qfexpressioncontextutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfexpressioncontextutils.h**](qfexpressioncontextutils_8h.md)

[Go to the documentation of this file](qfexpressioncontextutils_8h.md)


```C++
/***************************************************************************
                            qfexpressioncontextutils.h
                              -------------------
              begin                : 5.12.2017
              copyright            : (C) 2017 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QFEXPRESSIONCONTEXTUTILS_H
#define QFEXPRESSIONCONTEXTUTILS_H

#include "qfcloudutils.h"
#include "qfgnsspositioninformation.h"
#include "qfsnappingresult.h"

#include <qgsexpressioncontext.h>

#define PROTECTED_GLOBAL_VARIABLE_NAMES QStringList( { QStringLiteral( "cloud_username" ), QStringLiteral( "cloud_useremail" ) } )


class QfExpressionContextUtils : public QObject
{
    Q_OBJECT

  public:
    explicit QfExpressionContextUtils( QObject *parent = nullptr );

    static QgsExpressionContextScope *positionScope( const QfGnssPositionInformation &positionInformation, bool positionLocked );
    static QgsExpressionContextScope *mapToolCaptureScope( const QfSnappingResult &topSnappingResult );
    static QgsExpressionContextScope *cloudUserScope( const QfCloudUserInformation &cloudUserInformation );

    Q_INVOKABLE static QVariantMap layerVariables( QgsMapLayer *layer );

    Q_INVOKABLE static void setLayerVariable( QgsMapLayer *layer, const QString &name, const QVariant &value );

    Q_INVOKABLE static void setLayerVariables( QgsMapLayer *layer, const QVariantMap &variables );

    Q_INVOKABLE static void removeLayerVariable( QgsMapLayer *layer, const QString &name );

    Q_INVOKABLE static QVariantMap projectVariables( QgsProject *project );

    Q_INVOKABLE static void setProjectVariable( QgsProject *project, const QString &name, const QVariant &value );

    Q_INVOKABLE static void setProjectVariables( QgsProject *project, const QVariantMap &variables );

    Q_INVOKABLE static void removeProjectVariable( QgsProject *project, const QString &name );

    Q_INVOKABLE static QVariantMap globalVariables();

    Q_INVOKABLE static void setGlobalVariable( const QString &name, const QVariant &value );

    Q_INVOKABLE static void setGlobalVariables( const QVariantMap &variables );

    Q_INVOKABLE static void removeGlobalVariable( const QString &name );
};

#endif // QFEXPRESSIONCONTEXTUTILS_H
```


