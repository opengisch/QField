/***************************************************************************
  appcontroller.h - AppController

---------------------
begin                : 19.7.2026
copyright            : (C) 2026 by Mohsen Dehghanzadeh
email                : info@opengis.ch
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QList>
#include <QString>

/**
 * \defgroup core
 * \brief QField C++ classes
 */

/**
 * \brief Application operations that AppInterface delegates to the app.
 *
 * \ingroup core
 */
class AppController
{
  public:
    virtual ~AppController() = default;

    //! Loads a project file (.qgs/.qgz) or standalone dataset from \a path, labelled \a name.
    virtual bool loadProjectFile( const QString &path, const QString &name = QString() ) = 0;

    //! Reloads the currently opened project.
    virtual void reloadProjectFile() = 0;

    //! Reads the content of the loaded project.
    virtual void readProjectFile() = 0;

    //! Reads a string project entry at \a scope / \a key (\a def if absent).
    virtual QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const = 0;

    //! Reads an int project entry at \a scope / \a key (\a def if absent).
    virtual int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const = 0;

    //! Reads a double project entry at \a scope / \a key (\a def if absent).
    virtual double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const = 0;

    //! Reads a bool project entry at \a scope / \a key (\a def if absent).
    virtual bool readProjectBoolEntry( const QString &scope, const QString &key, bool def = false ) const = 0;

    //! Prints the project layout matching \a layoutName to PDF.
    virtual bool print( const QString &layoutName ) = 0;

    //! Prints the atlas-driven layout \a layoutName for \a featureIds to PDF.
    virtual bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds ) = 0;

    //! Sets the screen dimmer timeout in seconds (0 disables dimming).
    virtual void setScreenDimmerTimeout( int timeoutSeconds ) = 0;

    //! Clears the currently opened project.
    virtual void clearProject() = 0;
};

#endif // APPCONTROLLER_H
