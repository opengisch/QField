#ifndef FEATURESLOCATORFILTER_H
#define FEATURESLOCATORFILTER_H

#include <QObject>

#include "qgslocatorfilter.h"
#include "qgsexpressioncontext.h"
#include "qgsfeatureiterator.h"

class LocatorModelSuperBridge;

class FeaturesLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:

    enum ContextMenuEntry
    {
      NoEntry,
      OpenForm
    };

    struct PreparedLayer
    {
      public:
        QgsExpression expression;
        QgsExpressionContext context;
        QgsFeatureIterator iterator;
        QString layerName;
        QString layerId;
        QIcon layerIcon;
    } ;

    FeaturesLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    FeaturesLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "allfeatures" ); }
    QString displayName() const override { return tr( "Features In All Layers" ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "af" ); }

    void prepare( const QString &string, const QgsLocatorContext &context ) override;
    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    int mMaxResultsPerLayer = 6;
    int mMaxTotalResults = 12;
    QList<PreparedLayer> mPreparedLayers;
    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // FEATURESLOCATORFILTER_H
