#ifndef QGSEXTENDEDFEATURE_H
#define QGSEXTENDEDFEATURE_H

#include <qgsfeature.h>

class Feature : public QObject, public QgsFeature
{
    Q_OBJECT

  public:
    explicit Feature( const QgsFeature& feature );
};

#endif // QGSEXTENDEDFEATURE_H
