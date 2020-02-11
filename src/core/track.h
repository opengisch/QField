#ifndef TRACK_H
#define TRACK_H

#include <QQuickItem>
#include <QTimer>

class RubberbandModel;

class Track : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( RubberbandModel *model READ model WRITE setModel NOTIFY modelChanged )
    //! track interval in seconds
    Q_PROPERTY( int timeInterval READ timeInterval WRITE setTimeInterval NOTIFY timeIntervalChanged )
    //! track minimum distance in map units
    Q_PROPERTY( int minimumDistance READ minimumDistance WRITE setMinimumDistance NOTIFY minimumDistanceChanged )
    //! track conjunction if track interval and minimum distance needs to be reached for tracking
    Q_PROPERTY( bool conjunction READ conjunction WRITE setConjunction NOTIFY conjunctionChanged )

  public:
    Track( QQuickItem *parent = nullptr );

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

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

  signals:
    void modelChanged();
    //! \copydoc timeInterval
    void timeIntervalChanged();
    //! \copydoc minimumDistance
    void minimumDistanceChanged();
    //! \copydoc conjunction
    void conjunctionChanged();


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

    void trackPosition();
};

#endif // TRACK_H

