/***************************************************************************
  activelayerfeatureslocatorfilter.h

 ---------------------
 begin                : 30.08.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef ACTIVELAYERFEATURESLOCATORFILTER_H
#define ACTIVELAYERFEATURESLOCATORFILTER_H

#include <QObject>
#include <qgsexpressioncontext.h>
#include <qgslocatorfilter.h>
#include <qgsvectorlayerfeatureiterator.h>


class LocatorModelSuperBridge;

/**
 * FeaturesLocatorFilter is a locator filter to search
 * for features across layers in the project.
 * Reimplemented from QGIS code (app).
 */
class ActiveLayerFeaturesLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    //! Origin of the action which triggers the result
    enum ActionOrigin
    {
      Normal,
      OpenForm,
      Navigation,
    };

    enum class ResultType
    {
      Feature,
      FieldRestriction,
    };
    Q_ENUM( ResultType )

    explicit ActiveLayerFeaturesLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    ActiveLayerFeaturesLocatorFilter *clone() const override;
    // Note that the name is important here, has to match the QgsLocator::CORE_FILTERS one to give us a 1-letter prefix
    QString name() const override { return QStringLiteral( "features" ); }
    QString displayName() const override { return tr( "Features from active layer" ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "f" ); }

    QStringList prepare( const QString &string, const QgsLocatorContext &locatorContext ) override;
    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    QString fieldRestriction( QString &searchString, bool *isRestricting = nullptr );

    QgsExpression mDispExpression;
    QgsExpressionContext mContext;
    QgsFeatureIterator mDisplayTitleIterator;
    QgsFeatureIterator mFieldIterator;
    QString mLayerId;
    QString mLayerName;
    bool mLayerIsSpatial = false;
    QStringList mAttributeAliases;
    QStringList mFieldsCompletion;

    int mMaxTotalResults = 16;
    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // ACTIVELAYERFEATURESLOCATORFILTER_H
