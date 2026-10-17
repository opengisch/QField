

# File qfactivelayerfeatureslocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qfactivelayerfeatureslocatorfilter.h**](qfactivelayerfeatureslocatorfilter_8h.md)

[Go to the documentation of this file](qfactivelayerfeatureslocatorfilter_8h.md)


```C++
/***************************************************************************
  qfactivelayerfeatureslocatorfilter.h

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


#ifndef QFACTIVELAYERFEATURESLOCATORFILTER_H
#define QFACTIVELAYERFEATURESLOCATORFILTER_H

#include <QObject>
#include <qgsexpressioncontext.h>
#include <qgslocatorfilter.h>
#include <qgsvectorlayerfeatureiterator.h>


class QfLocatorModelSuperBridge;

class QfActiveLayerFeaturesLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    enum ActionOrigin
    {
      Normal,
      OpenForm,
      QfNavigation,
    };

    enum class ResultType
    {
      Feature,
      FieldRestriction,
    };
    Q_ENUM( ResultType )

    explicit QfActiveLayerFeaturesLocatorFilter( QfLocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    QfActiveLayerFeaturesLocatorFilter *clone() const override;
    // Note that the name is important here, has to match the QgsLocator::CORE_FILTERS one to give us a 1-letter prefix
    QString name() const override { return QStringLiteral( "features" ); }
    QString displayName() const override { return tr( "Features from active layer" ); }
    QString description() const override { return tr( "Returns a list of features from the active layer with matching attributes. Restricting matching to a single attribute is done by identifying its name prefixed with an '@'." ); }
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
    QfLocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // QFACTIVELAYERFEATURESLOCATORFILTER_H
```


