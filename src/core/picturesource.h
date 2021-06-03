/***************************************************************************
  picturesource.h - PictureSource

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
#ifndef PICTURESOURCE_H
#define PICTURESOURCE_H

#include <QObject>

/**
 * A picture source object will be created when a new picture is requested from the system.
 * It should be subclassed and filled with platform specific code.
 */
class PictureSource : public QObject
{
    Q_OBJECT
  public:
    /**
     * Construct a new Picture Source object.
     *
     * @note Subclasses which implement their own file dialog should provide an empty string for \a pictureFilePath and emit \a pictureReceived when appropriate.
     * @param parent Parent object
     * @param prefix The project folder. Base directory path for all relative paths.
     * @param pictureFilePath Suggested file path to permanently store the file. If the file is not existing yet, it must be an empty string.
     */
    explicit PictureSource( QObject *parent = nullptr, const QString &prefix = QString(), const QString &pictureFilePath = QString() );

    /**
     * Destroy the Picture Source object
     */
    virtual ~PictureSource();

  signals:

    /**
     * Emit this signal when a picture really has been received.
     *
     * @note When the constructor received a non-empty \a pictureFilePath, the signal is emitted in the constructor.
     */
    void pictureReceived( const QString &path );

  private:
    /**
     * Base directory path for all relative paths.
     */
    QString mPrefix;

    /**
     * Suggested file path to permanently store the file. If the file is not existing yet, it must be an empty string.
     */
    QString mPictureFilePath;
};

#endif // PICTURESOURCE_H
