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

#include <qgsexpressioncontext.h>
#include "snappingresult.h"
#include "qfieldcore_global.h"

class QGeoPositionInfoSource;

class QFIELDCORE_EXPORT ExpressionContextUtils
{
  public:
    static QgsExpressionContextScope *positionScope( QGeoPositionInfoSource *source );
    static QgsExpressionContextScope *mapToolCaptureScope( const SnappingResult &topSnappingResult );

  private:
    ExpressionContextUtils() = default;
};

#endif // EXPRESSIONCONTEXTUTILS_H
