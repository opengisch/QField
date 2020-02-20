#ifndef TRACKER_H
#define TRACKER_H

#include <QTimer>
#include "qgsvectorlayer.h"

class RubberbandModel;

class Tracker : public QObject
{
    Q_OBJECT
  public:
    explicit Tracker( QgsVectorLayer *layer );

    RubberbandModel *model() const;
    void setModel( RubberbandModel *model );

    //! \copydoc timeInterval
    int timeInterval() const;
    //! \copydoc timeInterval
    void setTimeInterval( int timeInterval );

    //! \copydoc minimumDistance
    int minimumDistance() const;
    //! \copydoc timeInterval
    void setMinimumDistance( int minimumDistance );

    //! \copydoc conjunction
    bool conjunction() const;
    //! \copydoc conjunction
    void setConjunction( bool conjunction );

    QgsVectorLayer *layer() const { return mLayer; }
    void setLayer( QgsVectorLayer *layer ) { mLayer = layer; }
    QgsFeature feature() const { return mFeature; }
    void setFeature( QgsFeature feature ) { mFeature = feature; }
    bool visible() const { return mVisible; }
    void setVisible( bool visible ) { mVisible = visible; }

    void start();
    void stop();

  private slots:
    void positionReceived();
    void timeReceived();

  private:
    RubberbandModel *mRubberbandModel = nullptr;

    QTimer mTimer;
    int mTimeInterval = 0;
    int mMinimumDistance = 0;
    bool mConjunction = true;
    bool mTimeIntervalFulfilled = false;
    bool mMinimumDistanceFulfilled = false;

    QgsVectorLayer *mLayer = nullptr;
    QgsFeature mFeature;

    bool mVisible = false;

    void trackPosition();
};

#endif // TRACKER_H

