

# File qgsgpkgflusher.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsgpkgflusher.h**](qgsgpkgflusher_8h.md)

[Go to the documentation of this file](qgsgpkgflusher_8h.md)


```C++
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

class QgsGpkgFlusher : public QObject
{
    Q_OBJECT

  public:
    explicit QgsGpkgFlusher( QgsProject *project );
    ~QgsGpkgFlusher();

    void stop( const QString &fileName );

    void start( const QString &fileName );

    bool isStopped( const QString &fileName ) const;

  signals:

    void requestFlush( const QString &filename );

  private slots:
    void onLayersAdded( const QList<QgsMapLayer *> &layers );

  private:
    QgsProject *mProject = nullptr;
    QThread mFlusherThread;
    Flusher *mFlusher = nullptr;
};

#endif // QGSGPKGFLUSHER_H
```


