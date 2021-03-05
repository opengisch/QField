/***************************************************************************
                          qgsgpkgflusher.h
                             -------------------
  begin                : Oct 2019
  copyright            : (C) 2019 by Matthias Kuhn
  email                : matthias@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSGPKGFLUSHER_H
#define QGSGPKGFLUSHER_H

#include <QObject>
#include <QThread>
#include <qgsmaplayer.h>

class QgsProject;
class Flusher;

/**
 * The QgsGpkgFlusher class is attached to a QgsProject.
 * It will make sure that all changes are regularly flushed from the wal file
 * to the gpkg itself on all added layers.
 * It will start a background thread and post an event to it whenever the gpkg has been changed.
 * After a delay of 500ms without any changes the wal file will be flushed.
 * The flusher does not need to be started after initialization.
 */
class QgsGpkgFlusher : public QObject
{
    Q_OBJECT

  public:
    explicit QgsGpkgFlusher( QgsProject *project );
    ~QgsGpkgFlusher();

    /**
     * Immediately performs a flush for a given \a fileName and returns. If the flusher is stopped, flush for that \a fileName would be ignored.
     */
    void stop( const QString &fileName );

    /**
     * Reenables scheduling a flush for a given \a fileName.
     */
    void start( const QString &fileName );

    /**
     * Returns whether the flusher is stopped for a given \a fileName
     */
    bool isStopped( const QString &fileName ) const;

  signals:

    /**
     * Emitted when a file has changed and a flush should be scheduled.
     */
    void requestFlush( const QString &filename );

  private slots:
    void onLayersAdded( const QList<QgsMapLayer *> &layers );

  private:
    QgsProject *mProject = nullptr;
    QThread mFlusherThread;
    Flusher *mFlusher = nullptr;
};

#endif // QGSGPKGFLUSHER_H
