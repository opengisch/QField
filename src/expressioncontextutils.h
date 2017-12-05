#ifndef EXPRESSIONCONTEXTUTILS_H
#define EXPRESSIONCONTEXTUTILS_H

#include <qgsexpressioncontext.h>

class QGeoPositionInfoSource;

class ExpressionContextUtils
{
  public:
    static QgsExpressionContextScope* positionScope(QGeoPositionInfoSource* source);

  private:
    ExpressionContextUtils() = default;
};

#endif // EXPRESSIONCONTEXTUTILS_H
