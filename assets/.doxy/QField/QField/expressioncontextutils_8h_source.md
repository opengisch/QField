

# File expressioncontextutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**expressioncontextutils.h**](expressioncontextutils_8h.md)

[Go to the documentation of this file](expressioncontextutils_8h.md)


```C++
/***************************************************************************
                            expressioncontextutils.h
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


#ifndef EXPRESSIONCONTEXTUTILS_H
#define EXPRESSIONCONTEXTUTILS_H

#include "gnsspositioninformation.h"
#include "qfieldcloudconnection.h"
#include "snappingresult.h"

#include <qgsexpressioncontext.h>

class ExpressionContextUtils : public QObject
{
    Q_OBJECT

  public:
    explicit ExpressionContextUtils( QObject *parent = nullptr );

    static QgsExpressionContextScope *positionScope( const GnssPositionInformation &positionInformation, bool positionLocked );
    static QgsExpressionContextScope *mapToolCaptureScope( const SnappingResult &topSnappingResult );
    static QgsExpressionContextScope *cloudUserScope( const CloudUserInformation &cloudUserInformation );

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

#endif // EXPRESSIONCONTEXTUTILS_H
```


