
#include <qgslocator.h>

#include "qgsquicklocatorwidget.h"

QgsQuickLocatorWidget::QgsQuickLocatorWidget(QQuickItem *parent)
    : QQuickItem( parent )
    , mLocator( new QgsLocator() )
{

}

QgsLocator *QgsQuickLocatorWidget::locator() const
{
  return mLocator;
}
