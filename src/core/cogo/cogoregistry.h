/***************************************************************************
                              cogoregistry.h
                              -------------------
              begin                : 2025.12.21
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef COGOREGISTRY_H
#define COGOREGISTRY_H

#include "cogooperation.h"

#include <QMap>
#include <QObject>

/**
 * \brief A registry managing available COGO operations.
 * \ingroup core
 */
class CogoRegistry : public QObject
{
    Q_OBJECT

  public:
    /**
     * COGO registry constructor.
     */
    explicit CogoRegistry( QObject *parent = nullptr );
    ~CogoRegistry();

    /**
     * Registers a COGO \a operation.
     * \note the ownership of the operation pointer is transferred to the registry.
     */
    bool registerOperation( CogoOperation *operation );

    /**
     * Returns a COGO operation matching a given \a name.
     */
    CogoOperation *operation( const QString &name );

    /**
     * Returns the available COGO operations within the registry.
     */
    QStringList availableOperations() const { return mOperations.keys(); }

    /**
     * Set the current COGO registry singleton \a instance.
     */
    static void setInstance( CogoRegistry *instance ) { sCogoRegistryInterface = instance; }

    /**
     * Returns the current COGO registry singleton instance.
     */
    static CogoRegistry *instance() { return sCogoRegistryInterface; }

  signals:
    /**
     * Emitted when a COGO has been added into the registry.
     */
    void operationAdded();

  private:
    QMap<QString, CogoOperation *> mOperations;

    static CogoRegistry *sCogoRegistryInterface;
};

#endif // COGOREGISTRY_H
