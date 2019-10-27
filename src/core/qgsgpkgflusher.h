#ifndef QGSGPKGFLUSHER_H
#define QGSGPKGFLUSHER_H

#include <QObject>
#include <qgsmaplayer.h>

class QgsProject;


class Flusher : public QObject
{
    Q_OBJECT

  public slots:
    void scheduleFlush( const QString &filename );

    void flush( const QString &filename );

  private:
    QMap<QString, QTimer *> mScheduledFlushes;
};

class QgsGpkgFlusher : public QObject
{
    Q_OBJECT

  public:
    QgsGpkgFlusher( QgsProject *project );
    ~QgsGpkgFlusher();

  signals:
    void requestFlush( const QString &filename );

  private slots:
    void onLayersAdded( const QList<QgsMapLayer *> layers );

  private:
    QgsProject *mProject;
    QThread mFlusherThread;
    Flusher *mFlusher;
};

#endif // QGSGPKGFLUSHER_H
