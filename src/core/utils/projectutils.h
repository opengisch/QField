/***************************************************************************
  projectutils.h - ProjectUtils

 ---------------------
 begin                : 19.04.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTUTILS_H
#define PROJECTUTILS_H

#include <QObject>
#include <qgsproject.h>

/**
 * \ingroup core
 */
class ProjectUtils : public QObject
{
    Q_OBJECT

  public:
    explicit ProjectUtils( QObject *parent = nullptr );

    /**
     * Returns a map of all registered layers by layer ID.
     * \note This function mimics QgsProject::mapLayers with a return type
     * that is QML compatible.
     */
    Q_INVOKABLE static QVariantMap mapLayers( QgsProject *project = nullptr );

    /**
     * Returns the transaction mode for a given \a project.
     * \note To be removed when QField updates to QGIS 3.38.
     */
    Q_INVOKABLE Qgis::TransactionMode transactionMode( QgsProject *project = nullptr );
};

#endif // PROJECTUTILS_H
