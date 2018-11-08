#ifndef QGSALLLAYERSFEATURESLOCATORFILTER_H
#define QGSALLLAYERSFEATURESLOCATORFILTER_H

#include <QObject>

#include "qgslocatorfilter.h"
#include "qgsexpressioncontext.h"
#include "qgsfeatureiterator.h"

class QgsAllLayersFeaturesLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

      public:
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

        QgsAllLayersFeaturesLocatorFilter( QObject *parent = nullptr );
        QgsAllLayersFeaturesLocatorFilter *clone() const override;
        QString name() const override { return QStringLiteral( "allfeatures" ); }
        QString displayName() const override { return tr( "Features In All Layers" ); }
        Priority priority() const override { return Medium; }
        QString prefix() const override { return QStringLiteral( "af" ); }

        void prepare( const QString &string, const QgsLocatorContext &context ) override;
        void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
        Q_INVOKABLE void triggerResult( const QgsLocatorResult &result ) override;

      private:
        int mMaxResultsPerLayer = 6;
        int mMaxTotalResults = 12;
        QList<PreparedLayer> mPreparedLayers;
};

#endif // QGSALLLAYERSFEATURESLOCATORFILTER_H
