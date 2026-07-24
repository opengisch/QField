

# File processingalgorithmparametersmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**processing**](dir_14e4815d6f2d5bd986fc95a1af67243d.md) **>** [**processingalgorithmparametersmodel.h**](processingalgorithmparametersmodel_8h.md)

[Go to the documentation of this file](processingalgorithmparametersmodel_8h.md)


```C++
/***************************************************************************
  processingalgorithmparametersmodel.h - ProcessingAlgorithmParametersModel

 ---------------------
 begin                : 21.06.2024
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


#ifndef PROCESSINGALGORITHMPARAMETERSMODEL
#define PROCESSINGALGORITHMPARAMETERSMODEL

#include "qgsvectorlayer.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

class QgsProcessingAlgorithm;
class QgsProcessingParameterDefinition;

class ProcessingAlgorithmParametersModelBase;


class ProcessingAlgorithmParametersModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( ProcessingAlgorithmParametersModel::Filters filters READ filters WRITE setFilters NOTIFY filtersChanged )

    Q_PROPERTY( QString algorithmId READ algorithmId WRITE setAlgorithmId NOTIFY algorithmIdChanged )
    Q_PROPERTY( QgsVectorLayer *inPlaceLayer READ inPlaceLayer WRITE setInPlaceLayer NOTIFY inPlaceLayerChanged )

    Q_PROPERTY( bool isValid READ isValid NOTIFY algorithmIdChanged )
    Q_PROPERTY( bool hasParameters READ hasParameters NOTIFY algorithmIdChanged )
    Q_PROPERTY( bool hasAdvancedParameters READ hasAdvancedParameters NOTIFY algorithmIdChanged )
    Q_PROPERTY( QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged )

    Q_PROPERTY( QString algorithmDisplayName READ algorithmDisplayName NOTIFY algorithmIdChanged )
    Q_PROPERTY( QString algorithmShortHelp READ algorithmShortHelp NOTIFY algorithmIdChanged )

  public:
    enum Filter
    {
      GeneralParameterFilter = 1 << 1,  
      AdvancedParameterFilter = 1 << 2, 
    };
    Q_DECLARE_FLAGS( Filters, Filter )
    Q_FLAGS( Filters )

    explicit ProcessingAlgorithmParametersModel( QObject *parent = nullptr );

    ProcessingAlgorithmParametersModel::Filters filters() const { return mFilters; }

    void setFilters( ProcessingAlgorithmParametersModel::Filters filters );

    QString algorithmId() const;

    void setAlgorithmId( const QString &id );

    QgsVectorLayer *inPlaceLayer() const;

    void setInPlaceLayer( QgsVectorLayer *layer );

    bool isValid() const;

    bool hasParameters() const;

    bool hasAdvancedParameters() const;

    QString algorithmDisplayName() const;

    QString algorithmShortHelp() const;

    QVariantMap parameters();

    void setParameters( const QVariantMap &parameters );

    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  signals:
    void filtersChanged();

    void algorithmIdChanged( const QString &id );

    void inPlaceLayerChanged();

    void parametersChanged();

  private:
    ProcessingAlgorithmParametersModel::Filters mFilters;
    ProcessingAlgorithmParametersModelBase *mModel = nullptr;
};

class ProcessingAlgorithmParametersModelBase : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Role
    {
      ParameterTypeRole = Qt::UserRole, 
      ParameterDescriptionRole,         
      ParameterFlagsRole,               
      ParameterDefaultValueRole,        
      ParameterValueRole,               
      ParameterConfigurationRole,       
    };
    Q_ENUM( Role )

    explicit ProcessingAlgorithmParametersModelBase( QObject *parent = nullptr );

    QString algorithmId() const { return mAlgorithmId; }

    void setAlgorithmId( const QString &id );

    QgsVectorLayer *inPlaceLayer() const { return mInPlaceLayer.data(); }

    void setInPlaceLayer( QgsVectorLayer *layer );

    bool isValid() const { return mAlgorithm; }

    bool hasParameters() const { return !mParameters.isEmpty(); };

    bool hasAdvancedParameters() const { return mHasAdvancedParameters; }

    QString algorithmDisplayName() const;

    QString algorithmShortHelp() const;

    QVariantMap parameters();

    void setParameters( const QVariantMap &parameters );

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

  signals:
    void algorithmIdChanged( const QString &id );

    void inPlaceLayerChanged();

    void parametersChanged();

  private:
    void rebuild();

    QString mAlgorithmId;
    const QgsProcessingAlgorithm *mAlgorithm = nullptr;

    QPointer<QgsVectorLayer> mInPlaceLayer;

    bool mHasAdvancedParameters = false;

    QList<const QgsProcessingParameterDefinition *> mParameters;
    QList<QVariant> mValues;
};


#endif // PROCESSINGALGORITHMPARAMETERSMODEL
```


