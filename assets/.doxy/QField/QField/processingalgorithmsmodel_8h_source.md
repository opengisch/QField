

# File processingalgorithmsmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**processing**](dir_14e4815d6f2d5bd986fc95a1af67243d.md) **>** [**processingalgorithmsmodel.h**](processingalgorithmsmodel_8h.md)

[Go to the documentation of this file](processingalgorithmsmodel_8h.md)


```C++
/***************************************************************************
  processingalgorithmsmodel.h - ProcessingAlgorithmsModel

 ---------------------
 begin                : 18.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PROCESSINGALGORITHMSMODEL
#define PROCESSINGALGORITHMSMODEL

#include <QAbstractListModel>
#include <QPointer>
#include <QSortFilterProxyModel>

class QgsProcessingProvider;
class QgsProcessingAlgorithm;
class QgsVectorLayer;

class ProcessingAlgorithmsModelBase;

class AlgorithmItem
{
  public:
    explicit AlgorithmItem( const QgsProcessingAlgorithm *algorithm = nullptr )
      : mAlgorithm( algorithm )
    {}
    const QgsProcessingAlgorithm *algorithm() const { return mAlgorithm; }

  private:
    const QgsProcessingAlgorithm *mAlgorithm = nullptr;
};


class ProcessingAlgorithmsModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( ProcessingAlgorithmsModel::Filters filters READ filters WRITE setFilters NOTIFY filtersChanged )
    Q_PROPERTY( QgsVectorLayer *inPlaceLayer READ inPlaceLayer WRITE setInPlaceLayer NOTIFY inPlaceLayerChanged )

  public:
    enum Filter
    {
      InPlaceFilter = 1 << 1,  
      FavoriteFilter = 1 << 2, 
    };
    Q_DECLARE_FLAGS( Filters, Filter )
    Q_FLAGS( Filters )

    explicit ProcessingAlgorithmsModel( QObject *parent = nullptr );

    Q_INVOKABLE void rebuild();

    ProcessingAlgorithmsModel::Filters filters() const { return mFilters; }

    void setFilters( ProcessingAlgorithmsModel::Filters filters );

    QgsVectorLayer *inPlaceLayer() const { return mInPlaceLayer.data(); }

    void setInPlaceLayer( QgsVectorLayer *layer );

    bool lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const override;
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  signals:
    void filtersChanged();

    void inPlaceLayerChanged();

  private:
    ProcessingAlgorithmsModelBase *mModel = nullptr;
    ProcessingAlgorithmsModel::Filters mFilters;
    QPointer<QgsVectorLayer> mInPlaceLayer;
};

class ProcessingAlgorithmsModelBase : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Role
    {
      AlgorithmIdRole = Qt::UserRole, 
      AlgorithmGroupRole,             
      AlgorithmNameRole,              
      AlgorithmSvgIconRole,           
      AlgorithmFlagsRole,             
      AlgorithmFavoriteRole,          
    };
    Q_ENUM( Role )

    explicit ProcessingAlgorithmsModelBase( QObject *parent = nullptr );

    Q_INVOKABLE void rebuild();

    const QgsProcessingAlgorithm *algorithmForIndex( const QModelIndex &index ) const;

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;


  private:
    void addProvider( QgsProcessingProvider *provider );

    QList<AlgorithmItem> mAlgorithms;
};


#endif // PROCESSINGALGORITHMSMODEL
```


