/***************************************************************************
  resourcesource.h - ResourceSource

 ---------------------
 begin                : 5.7.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef RESOURCESOURCE_H
#define RESOURCESOURCE_H

#include <QObject>

/**
 * A resource source object will be created when a new resource is requested from the system.
 * It should be subclassed and filled with platform specific code.
 * \ingroup core
 */
class ResourceSource : public QObject
{
    Q_OBJECT
  public:
    /**
     * Construct a new ResourceSource object.
     *
     * @note Subclasses which implement their own file dialog should provide an empty string for \a resourceFilePath and emit \a resourceReceived when appropriate.
     * @param parent Parent object
     * @param prefix The project folder. Base directory path for all relative paths.
     * @param resourceFilePath Suggested file path to permanently store the file. If the file is not existing yet, it must be an empty string.
     */
    explicit ResourceSource( QObject *parent = nullptr, const QString &prefix = QString(), const QString &resourceFilePath = QString() );

    /**
     * Destroy the ResourceSource object
     */
    virtual ~ResourceSource();

  signals:

    /**
     * Emit this signal when a resource really has been received.
     *
     * @note When the constructor received a non-empty \a resourceFilePath, the signal is emitted in the constructor.
     */
    void resourceReceived( const QString &path );

  private:
    /**
     * Base directory path for all relative paths.
     */
    QString mPrefix;

    /**
     * Suggested file path to permanently store the file. If the file is not existing yet, it must be an empty string.
     */
    QString mResourceFilePath;
};

#endif // RESOURCESOURCE_H
