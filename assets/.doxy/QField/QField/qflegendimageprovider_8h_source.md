

# File qflegendimageprovider.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qflegendimageprovider.h**](qflegendimageprovider_8h.md)

[Go to the documentation of this file](qflegendimageprovider_8h.md)


```C++
/***************************************************************************
  qflegendimageprovider.h - QfLegendImageProvider

 ---------------------
 begin                : 7.12.2016
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
#ifndef QFLEGENDIMAGEPROVIDER_H
#define QFLEGENDIMAGEPROVIDER_H

#include "qgsquickmapsettings.h"

#include <QQuickAsyncImageProvider>
#include <QQuickImageProvider>
#include <QQuickImageResponse>
#include <qgsrasterdataprovider.h>
#include <qobjectuniqueptr.h>

class QgsLayerTreeModel;
class QgsLayerTree;

class QfLegendImageProvider : public QQuickImageProvider
{
  public:
    explicit QfLegendImageProvider( QgsLayerTreeModel *layerTreeModel );

    QPixmap requestPixmap( const QString &id, QSize *size, const QSize &requestedSize ) override;

  private:
    QPointer<QgsLayerTreeModel> mLayerTreeModel;
    QPointer<QgsLayerTree> mRootNode;
};


class QfAsyncLegendImageResponse : public QQuickImageResponse
{
  public:
    QfAsyncLegendImageResponse( QgsRasterDataProvider *dataProvider = nullptr, const QgsMapSettings *mapSettings = nullptr );
    ~QfAsyncLegendImageResponse();

    QQuickTextureFactory *textureFactory() const override;

  private slots:
    void handleFinish( const QImage &image );
    void handleError( const QString &error );

  private:
    std::unique_ptr<QgsRasterDataProvider> mDataProvider;
    QPointer<QgsImageFetcher> mFetcher;

    QImage mImage;
};


class QfAsyncLegendImageProvider : public QQuickAsyncImageProvider
{
  public:
    explicit QfAsyncLegendImageProvider( QgsLayerTreeModel *layerTreeModel );

    QQuickImageResponse *requestImageResponse( const QString &id, const QSize &requestedSize ) override;

    void setMapSettings( QgsQuickMapSettings *mapSettings );

  private:
    QPointer<QgsLayerTreeModel> mLayerTreeModel;
    QPointer<QgsLayerTree> mRootNode;

    QPointer<QgsQuickMapSettings> mMapSettings;
};

#endif // QFLEGENDIMAGEPROVIDER_H
```


