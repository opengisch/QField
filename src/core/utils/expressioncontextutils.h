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

/**
 * \ingroup core
 */
class ExpressionContextUtils : public QObject
{
    Q_OBJECT

  public:
    explicit ExpressionContextUtils( QObject *parent = nullptr );

    static QgsExpressionContextScope *positionScope( const GnssPositionInformation &positionInformation, bool positionLocked );
    static QgsExpressionContextScope *mapToolCaptureScope( const SnappingResult &topSnappingResult );
    static QgsExpressionContextScope *cloudUserScope( const CloudUserInformation &cloudUserInformation );

    /**
      * Returns a layer context variables.
      * \param layer map layer
      * \see setLayerVariable()
      * \see setLayerVariables()
      * \see removeLayerVariable()
      */
    Q_INVOKABLE static QVariantMap layerVariables( QgsMapLayer *layer );

    /**
      * Sets a layer context variable.
      * \param layer map layer
      * \param name variable name
      * \param value variable value
      * \see layerVariables()
      * \see setLayerVariables()
      * \see removeLayerVariable()
      */
    Q_INVOKABLE static void setLayerVariable( QgsMapLayer *layer, const QString &name, const QVariant &value );

    /**
      * Sets a layer context variables.
      * \param layer map layer
      * \param variables new set of layer variables
      * \see layerVariables()
      * \see setLayerVariable()
      * \see removeLayerVariable()
      */
    Q_INVOKABLE static void setLayerVariables( QgsMapLayer *layer, const QVariantMap &variables );

    /**
      * Removes a layer context variable.
      * \param layer map layer
      * \param name variable name
      * \see layerVariables()
      * \see setLayerVariable()
      * \see setLayerVariables()
      */
    Q_INVOKABLE static void removeLayerVariable( QgsMapLayer *layer, const QString &name );

    /**
      * Returns a project context variables.
      * \param project project
      * \see setProjectVariable()
      * \see setProjectVariables()
      * \see removeProjectVariable()
      */
    Q_INVOKABLE static QVariantMap projectVariables( QgsProject *project );

    /**
      * Sets a project context variable.
      * \param project project
      * \param name variable name
      * \param value variable value
      * \see projectVariables()
      * \see setProjectVariables()
      * \see removeProjectVariable()
      */
    Q_INVOKABLE static void setProjectVariable( QgsProject *project, const QString &name, const QVariant &value );

    /**
      * Sets a project context variables.
      * \param project project
      * \param variables new set of project variables
      * \see projectVariables()
      * \see setProjectVariable()
      * \see removeProjectVariable()
      */
    Q_INVOKABLE static void setProjectVariables( QgsProject *project, const QVariantMap &variables );

    /**
      * Removes a project context variable.
      * \param project project
      * \param name variable name
      * \see projectVariables()
      * \see setProjectVariable()
      * \see setProjectVariables()
      */
    Q_INVOKABLE static void removeProjectVariable( QgsProject *project, const QString &name );

    /**
      * Returns the global context variables.
      * \see setGlobalVariable()
      * \see setGlobalVariables()
      * \see removeGlobalVariable()
      */
    Q_INVOKABLE static QVariantMap globalVariables();

    /**
      * Sets a global context variable.
      * \param name variable name
      * \param value variable value
      * \see globalVariables()
      * \see setGlobalVariables()
      * \see removeGlobalVariable()
      */
    Q_INVOKABLE static void setGlobalVariable( const QString &name, const QVariant &value );

    /**
      * Sets the global context variables.
      * \param variables new set of global variables
      * \see globalVariables()
      * \see setGlobalVariable()
      * \see removeGlobalVariable()
      */
    Q_INVOKABLE static void setGlobalVariables( const QVariantMap &variables );

    /**
      * Removes a global context variable.
      * \param name variable name
      * \see globalVariables()
      * \see setGlobalVariable()
      * \see setGlobalVariables()
      */
    Q_INVOKABLE static void removeGlobalVariable( const QString &name );
};

#endif // EXPRESSIONCONTEXTUTILS_H
