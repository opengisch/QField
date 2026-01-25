/***************************************************************************
  quick3dmaptexturegenerator.h - Quick3DMapTextureGenerator

 ---------------------
 begin                : 26.1.2026
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
#ifndef QUICK3DMAPTEXTUREGENERATOR_H
#define QUICK3DMAPTEXTUREGENERATOR_H

#include <QImage>
#include <QObject>
#include <QStandardPaths>
#include <qgsproject.h>
#include <qgsrectangle.h>

class QgsMapRendererSequentialJob;

class Quick3DMapTextureGenerator : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )
    Q_PROPERTY( QString textureFilePath READ textureFilePath NOTIFY textureFilePathChanged )

  public:
    explicit Quick3DMapTextureGenerator( QObject *parent = nullptr );
    ~Quick3DMapTextureGenerator() override;

    QgsProject *project() const;
    void setProject( QgsProject *project );

    QgsRectangle extent() const;
    void setExtent( const QgsRectangle &extent );

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
    QgsRectangle mExtent;
    int mTextureSize = 1024;
    QImage mRenderedImage;
    QString mTextureFilePath;
    std::unique_ptr<QgsMapRendererSequentialJob> mRenderJob;

    static int sInstanceCounter;
};

#endif // QUICK3DMAPTEXTUREGENERATOR_H
