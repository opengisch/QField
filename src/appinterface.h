#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QObject>
#include <QPointF>

#include <QStandardItemModel>

#include "feature.h"

class QgisMobileapp;

class AppInterface : public QObject
{
    Q_OBJECT

  public:
    AppInterface( QgisMobileapp* app );
    AppInterface()
    {
      // You shouldn't get here, this constructor only exists that we can register it as a QML type
      Q_ASSERT( false );
    }

  public Q_SLOTS:
    /**
     * When called, it will request any features at this position and after doing so emit a
     * featuresIdentified signal to which you may connect.
     *
     * @param point Coordinate at which to identify features
     */
    void identifyFeatures( const QPointF point );

    void showFeatureForm( const Feature& feature );

    void openProjectDialog();

  private:
    QgisMobileapp* mApp;
};

#endif // APPINTERFACE_H
