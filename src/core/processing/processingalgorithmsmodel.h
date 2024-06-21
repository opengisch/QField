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
#include <QSortFilterProxyModel>

class QgsProcessingProvider;
class QgsProcessingAlgorithm;

class ProcessingAlgorithmsModel;

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
 */
class ProcessingAlgorithmsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

  public:
    explicit ProcessingAlgorithmsProxyModel( QObject *parent = nullptr );

    //! Rebuilds the algorithms model.
    Q_INVOKABLE void rebuild();

    bool lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const override;

  private:
    ProcessingAlgorithmsModel *mModel = nullptr;
};

/**
 * \brief A model for providers and algorithms available within QField
 *
 * See ProcessingAlgorithmsProxyModel for a sorted, filterable version
 * of this model.
 */
class ProcessingAlgorithmsModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    //!Roles to get the data of the model.
    enum Role
    {
      AlgorithmGroupRole = Qt::UserRole, //! the algorithm's group
      AlgorithmNameRole,                 //! the algorithm's name
      AlgorithmSvgIconRole,              //! the algorithm's SVG icon path
      AlgorithmFlagsRole,                //! the algorithm's flags
    };
    Q_ENUM( Role )

    explicit ProcessingAlgorithmsModel( QObject *parent = nullptr );

    //! Rebuilds the algorithms model.
    Q_INVOKABLE void rebuild();

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

  private:
    void addProvider( QgsProcessingProvider *provider );

    QList<AlgorithmItem> mAlgorithms;
};


#endif // PROCESSINGALGORITHMSMODEL
