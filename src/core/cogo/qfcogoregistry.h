/***************************************************************************
                              qfcogoregistry.h
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

#ifndef QFCOGOREGISTRY_H
#define QFCOGOREGISTRY_H

#include "qfcogooperation.h"

#include <QMap>
#include <QObject>

/**
 * \brief A registry managing available COGO operations.
 * \ingroup core
 */
class QfCogoRegistry : public QObject
{
    Q_OBJECT

  public:
    /**
     * COGO registry constructor.
     */
    explicit QfCogoRegistry( QObject *parent = nullptr );
    ~QfCogoRegistry();

    /**
     * Registers a COGO \a operation.
     * \note the ownership of the operation pointer is transferred to the registry.
     */
    bool registerOperation( QfCogoOperation *operation );

    /**
     * Returns a COGO operation matching a given \a name.
     */
    QfCogoOperation *operation( const QString &name );

    /**
     * Returns the available COGO operations within the registry.
     */
    QStringList availableOperations() const { return mOperations.keys(); }

    /**
     * Set the current COGO registry singleton \a instance.
     */
    static void setInstance( QfCogoRegistry *instance ) { sCogoRegistryInterface = instance; }

    /**
     * Returns the current COGO registry singleton instance.
     */
    static QfCogoRegistry *instance() { return sCogoRegistryInterface; }

  signals:
    /**
     * Emitted when a COGO has been added into the registry.
     */
    void operationAdded();

  private:
    QMap<QString, QfCogoOperation *> mOperations;

    static QfCogoRegistry *sCogoRegistryInterface;
};

#endif // QFCOGOREGISTRY_H
