

# File featureexpressionvaluesgatherer.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featureexpressionvaluesgatherer.h**](featureexpressionvaluesgatherer_8h.md)

[Go to the documentation of this file](featureexpressionvaluesgatherer_8h.md)


```C++
/***************************************************************************
  featureexpressionvaluesgatherer.h - FeatureExpressionValuesGatherer

 ---------------------
 begin                : 29.1.2021
 copyright            : (C) 2021 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef FEATUREEXPRESSIONVALUESGATHERER_H
#define FEATUREEXPRESSIONVALUESGATHERER_H

#include <QMutex>
#include <QThread>
#include <qgsapplication.h>
#include <qgsfeature.h>
#include <qgslogger.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerfeatureiterator.h>

class FeatureExpressionValuesGatherer : public QThread
{
    Q_OBJECT

  public:
    explicit FeatureExpressionValuesGatherer( QgsVectorLayer *layer,
                                              const QString &displayExpression = QString(),
                                              const QgsFeatureRequest &request = QgsFeatureRequest(),
                                              const QStringList &identifierFields = QStringList() )
      : mSource( new QgsVectorLayerFeatureSource( layer ) )
      , mDisplayExpression( displayExpression.isEmpty() ? layer->displayExpression() : displayExpression )
      , mExpressionContext( layer->createExpressionContext() )
      , mRequest( request )
      , mIdentifierFields( identifierFields )
    {
    }

    struct Entry
    {
        Entry() = default;

        Entry( const QVariantList &_identifierFields, const QString &_value, const QgsFeature &_feature )
          : identifierFields( _identifierFields )
          , featureId( _feature.isValid() ? _feature.id() : FID_NULL )
          , value( _value )
          , feature( _feature )
        {}

        Entry( const QgsFeatureId &_featureId, const QString &_value, const QgsVectorLayer *layer )
          : featureId( _featureId )
          , value( _value )
          , feature( QgsFeature( layer ? layer->fields() : QgsFields() ) )
        {}

        QVariantList identifierFields;
        QgsFeatureId featureId;
        QString value;
        QgsFeature feature;

        bool operator()( const Entry &lhs, const Entry &rhs ) const;
    };

    static Entry nullEntry( QgsVectorLayer *layer )
    {
      return Entry( QVariantList(), QgsApplication::nullRepresentation(), QgsFeature( layer->fields() ) );
    }

    void run() override
    {
      mWasCanceled = false;

      QgsFeatureIterator iterator = mSource->getFeatures( mRequest );

      mDisplayExpression.prepare( &mExpressionContext );

      QgsFeature feature;
      QList<int> attributeIndexes;
      for ( const QString &fieldName : std::as_const( mIdentifierFields ) )
        attributeIndexes << mSource->fields().indexOf( fieldName );

      while ( iterator.nextFeature( feature ) )
      {
        mExpressionContext.setFeature( feature );
        QVariantList attributes;
        for ( const int idx : attributeIndexes )
          attributes << feature.attribute( idx );

        const QString expressionValue = mDisplayExpression.evaluate( &mExpressionContext ).toString();

        mEntries.append( Entry( attributes, expressionValue, feature ) );

        QMutexLocker locker( &mCancelMutex );
        //cppcheck-suppress knownConditionTrueFalse
        if ( mWasCanceled )
          return;
      }
    }

    void stop()
    {
      QMutexLocker locker( &mCancelMutex );
      mWasCanceled = true;
    }

    bool wasCanceled() const
    {
      QMutexLocker locker( &mCancelMutex );
      return mWasCanceled;
    }

    QVector<Entry> entries() const
    {
      return mEntries;
    }

    QgsFeatureRequest request() const
    {
      return mRequest;
    }

    QVariant data() const
    {
      return mData;
    }

    void setData( const QVariant &data )
    {
      mData = data;
    }

  protected:
    QVector<Entry> mEntries;

  private:
    std::unique_ptr<QgsVectorLayerFeatureSource> mSource;
    QgsExpression mDisplayExpression;
    QgsExpressionContext mExpressionContext;
    QgsFeatureRequest mRequest;
    bool mWasCanceled = false;
    mutable QMutex mCancelMutex;
    QStringList mIdentifierFields;
    QVariant mData;
};

#endif // FEATUREEXPRESSIONVALUESGATHERER_H
```


