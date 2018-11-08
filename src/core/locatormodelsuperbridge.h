#ifndef LOCATORMODELSUPERBRIDGE_H
#define LOCATORMODELSUPERBRIDGE_H


#include <qgslocatormodelbridge.h>


class LocatorModelSuperBridge : public QgsLocatorModelBridge
{
Q_OBJECT
public:
  explicit LocatorModelSuperBridge(QObject *parent = nullptr);
  ~LocatorModelSuperBridge() = default;

public slots:
  Q_INVOKABLE void triggerResultAtRow( int row );
};

#endif // LOCATORMODELSUPERBRIDGE_H
