

# File legendimageprovider.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**legendimageprovider.h**](legendimageprovider_8h.md)

[Go to the documentation of this file](legendimageprovider_8h.md)


```C++
/***************************************************************************
  legendimageprovider.h - LegendImageProvider

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
#ifndef LEGENDIMAGEPROVIDER_H
#define LEGENDIMAGEPROVIDER_H

#include "qgsquickmapsettings.h"

#include <QQuickAsyncImageProvider>
#include <QQuickImageProvider>
#include <QQuickImageResponse>
#include <qgsrasterdataprovider.h>

class QgsLayerTreeModel;
class QgsLayerTree;

class LegendImageProvider : public QQuickImageProvider
{
  public:
    explicit LegendImageProvider( QgsLayerTreeModel *layerTreeModel );

    QPixmap requestPixmap( const QString &id, QSize *size, const QSize &requestedSize ) override;

  private:
    QPointer<QgsLayerTreeModel> mLayerTreeModel;
    QPointer<QgsLayerTree> mRootNode;
};


class AsyncLegendImageResponse : public QQuickImageResponse
{
  public:
    AsyncLegendImageResponse( QgsRasterDataProvider *dataProvider = nullptr, const QgsMapSettings *mapSettings = nullptr );

    QQuickTextureFactory *textureFactory() const override;

  private slots:
    void handleFinish( const QImage &image );
    void handleError( const QString &error );

  private:
    std::unique_ptr<QgsRasterDataProvider> mDataProvider;
    std::unique_ptr<QgsImageFetcher> mFetcher;

    QImage mImage;
};


class AsyncLegendImageProvider : public QQuickAsyncImageProvider
{
  public:
    explicit AsyncLegendImageProvider( QgsLayerTreeModel *layerTreeModel );

    QQuickImageResponse *requestImageResponse( const QString &id, const QSize &requestedSize ) override;

    void setMapSettings( QgsQuickMapSettings *mapSettings );

  private:
    QPointer<QgsLayerTreeModel> mLayerTreeModel;
    QPointer<QgsLayerTree> mRootNode;

    QPointer<QgsQuickMapSettings> mMapSettings;
};

#endif // LEGENDIMAGEPROVIDER_H
```


