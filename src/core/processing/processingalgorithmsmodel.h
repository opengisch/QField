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

/**
 * \ingroup core
 */
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


/**
 * \brief A sort/filter proxy model for providers and algorithms available within QField
 * which automatically sorts the toolbox in a logical fashion and supports filtering
 * the results.
 * \ingroup core
 */
class ProcessingAlgorithmsModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( ProcessingAlgorithmsModel::Filters filters READ filters WRITE setFilters NOTIFY filtersChanged )
    Q_PROPERTY( QgsVectorLayer *inPlaceLayer READ inPlaceLayer WRITE setInPlaceLayer NOTIFY inPlaceLayerChanged )

  public:
    //! Available filter flags for filtering the model
    enum Filter
    {
      InPlaceFilter = 1 << 1,  //!< Only show algorithms which support in-place edits
      FavoriteFilter = 1 << 2, //!< Only show algorithms that been added to favorites
    };
    Q_DECLARE_FLAGS( Filters, Filter )
    Q_FLAGS( Filters )

    explicit ProcessingAlgorithmsModel( QObject *parent = nullptr );

    //! Rebuilds the algorithms model.
    Q_INVOKABLE void rebuild();

    /**
     * Returns any filters that affect how toolbox content is filtered.
     * \see setFilters()
     */
    ProcessingAlgorithmsModel::Filters filters() const { return mFilters; }

    /**
     * Set \a filters that affect how toolbox content is filtered.
     * \see filters()
     */
    void setFilters( ProcessingAlgorithmsModel::Filters filters );

    /**
     * Returns the vector \a layer for in-place algorithm filter
     */
    QgsVectorLayer *inPlaceLayer() const { return mInPlaceLayer.data(); }

    /**
     * Sets the vector \a layer for in-place algorithm filter
     */
    void setInPlaceLayer( QgsVectorLayer *layer );

    bool lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const override;
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  signals:
    /**
     * Emitted when the active filters have changed
     */
    void filtersChanged();

    /**
     * Emitted when the in place vector layer has changed
     */
    void inPlaceLayerChanged();

  private:
    ProcessingAlgorithmsModelBase *mModel = nullptr;
    ProcessingAlgorithmsModel::Filters mFilters;
    QPointer<QgsVectorLayer> mInPlaceLayer;
};

/**
 * \brief A model for providers and algorithms available within QField
 *
 * See ProcessingAlgorithmsProxyModel for a sorted, filterable version
 * of this model.
 */
class ProcessingAlgorithmsModelBase : public QAbstractListModel
{
    Q_OBJECT

  public:
    //!Roles to get the data of the model.
    enum Role
    {
      AlgorithmIdRole = Qt::UserRole, //! the algorithm unique ID
      AlgorithmGroupRole,             //! the algorithm group
      AlgorithmNameRole,              //! the algorithm name
      AlgorithmSvgIconRole,           //! the algorithm SVG icon path
      AlgorithmFlagsRole,             //! the algorithm flags
      AlgorithmFavoriteRole,          //! the algorithm favorite status
    };
    Q_ENUM( Role )

    explicit ProcessingAlgorithmsModelBase( QObject *parent = nullptr );

    //! Rebuilds the algorithms model.
    Q_INVOKABLE void rebuild();

    //! Returns the processing algorithm for the given index.
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
