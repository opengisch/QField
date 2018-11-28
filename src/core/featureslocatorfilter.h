#ifndef FEATURESLOCATORFILTER_H
#define FEATURESLOCATORFILTER_H

#include <QObject>

#include "qgslocatorfilter.h"
#include "qgsexpressioncontext.h"
#include "qgsfeatureiterator.h"

class QgsQuickMapSettings;
class LocatorHighlight;

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

    FeaturesLocatorFilter( QgsQuickMapSettings *mapSettings, LocatorHighlight *locatorHighlight, QObject *parent = nullptr );
    FeaturesLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "allfeatures" ); }
    QString displayName() const override { return tr( "Features In All Layers" ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "af" ); }

    void prepare( const QString &string, const QgsLocatorContext &context ) override;
    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromContextMenu( const QgsLocatorResult &result, const int id ) override;

  private:
    int mMaxResultsPerLayer = 6;
    int mMaxTotalResults = 12;
    QList<PreparedLayer> mPreparedLayers;
    QgsQuickMapSettings *mMapSettings = nullptr;
    LocatorHighlight *mLocatorHighlight = nullptr;
};

#endif // FEATURESLOCATORFILTER_H
