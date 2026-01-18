/***************************************************************************
  qgsquick3dmaptexturegenerator.h - QgsQuick3DMapTextureGenerator

 ---------------------
 begin                : 6.1.2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSQUICK3DMAPTEXTUREGENERATOR_H
#define QGSQUICK3DMAPTEXTUREGENERATOR_H

#include <QImage>
#include <QObject>
#include <QRectF>
#include <QStandardPaths>

class QgsProject;
class QgsMapRendererSequentialJob;

class QgsQuick3DMapTextureGenerator : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QRectF extent READ extent WRITE setExtent NOTIFY extentChanged )
    Q_PROPERTY( QString textureFilePath READ textureFilePath NOTIFY textureFilePathChanged )

  public:
    explicit QgsQuick3DMapTextureGenerator( QObject *parent = nullptr );
    ~QgsQuick3DMapTextureGenerator() override;

    QgsProject *project() const;
    void setProject( QgsProject *project );

    QRectF extent() const;
    void setExtent( const QRectF &extent );

    QString textureFilePath() const;

    Q_INVOKABLE void render();

  signals:
    void projectChanged();
    void extentChanged();
    void textureSizeChanged();
    void ready();
    void textureFilePathChanged();

  private slots:
    void onRenderFinished();

  private:
    QgsProject *mProject = nullptr;
    QRectF mExtent;
    int mTextureSize = 1024;
    QImage mRenderedImage;
    QString mTextureFilePath;
    std::unique_ptr<QgsMapRendererSequentialJob> mRenderJob;

    static int sInstanceCounter;
};

#endif // QGSQUICK3DMAPTEXTUREGENERATOR_H
