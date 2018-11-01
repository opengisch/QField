
#include <qgslocator.h>
#include <qgslocatormodel.h>
#include <qgslocatorcontext.h>

#include "qgsquickmapsettings.h"
#include "qgsquicklocatorwrapper.h"

QgsQuickLocatorWrapper::QgsQuickLocatorWrapper( QQuickItem *parent )
  : QQuickItem( parent )
  , mLocator( new QgsLocator( this ) )
  , mLocatorModel( new QgsLocatorModel( this ) )
{
  mProxyModel = new QgsLocatorProxyModel( mLocatorModel );
  mProxyModel->setSourceModel( mLocatorModel );
}

QgsLocator *QgsQuickLocatorWrapper::locator() const
{
  return mLocator;
}

void QgsQuickLocatorWrapper::setLocator( QgsLocator *locator )
{
  mLocator = locator;
}

void QgsQuickLocatorWrapper::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  mMapSettings = mapSettings;
}


void QgsQuickLocatorWrapper::setSearchText( const QString &text )
{
  mPopupTimer.stop();
  updateResults( text );
  showList();
}

void QgsQuickLocatorWrapper::clearSearch()
{
  setSearchText( QString() );
}

void QgsQuickLocatorWrapper::updateResults( const QString &text )
{
  //mLineEdit->setShowSpinner( true ); TODO
  if ( mLocator->isRunning() )
  {
    // can't do anything while a query is running, and can't block
    // here waiting for the current query to cancel
    // so we queue up this string until cancel has happened
    mLocator->cancelWithoutBlocking();
    mNextRequestedString = text;
    mHasQueuedRequest = true;
    return;
  }
  else
  {
    mLocatorModel->deferredClear();
    mLocator->fetchResults( text, createContext() );
  }
}

QgsLocatorContext QgsQuickLocatorWrapper::createContext()
{
  QgsLocatorContext context;
  if ( mMapSettings )
  {
    context.targetExtent = mMapSettings->mapSettings().visibleExtent();
    context.targetExtentCrs = mMapSettings->mapSettings().destinationCrs();
  }
  return context;
}

void QgsQuickLocatorWrapper::scheduleDelayedPopup()
{
  mPopupTimer.start();
}

void QgsQuickLocatorWrapper::showList()
{
  //mResultsContainer->show();
  //mResultsContainer->raise();
}

void QgsQuickLocatorWrapper::triggerSearchAndShowList()
{
//  if ( mProxyModel->rowCount() == 0 )
//    performSearch();
//  else
//    showList();
}

void QgsQuickLocatorWrapper::searchFinished()
{
  if ( mHasQueuedRequest )
  {
    // a queued request was waiting for this - run the queued search now
    QString nextSearch = mNextRequestedString;
    mNextRequestedString.clear();
    mHasQueuedRequest = false;
    updateResults( nextSearch );
  }
  else
  {
    if ( !mLocator->isRunning() )
    {
      //mLineEdit->setShowSpinner( false ); TODO
    }
  }
}

void QgsQuickLocatorWrapper::addResult( const QgsLocatorResult &result )
{
  bool selectFirst = mProxyModel->rowCount() == 0;
  mLocatorModel->addResult( result );
  if ( selectFirst )
  {
    int row = -1;
    bool selectable = false;
    while ( !selectable && row < mProxyModel->rowCount() )
    {
      row++;
      selectable = mProxyModel->flags( mProxyModel->index( row, 0 ) ).testFlag( Qt::ItemIsSelectable );
    }
    if ( selectable )
    {
      //mResultsView->setCurrentIndex( mProxyModel->index( row, 0 ) ); TODO
    }
  }
}

void QgsQuickLocatorWrapper::acceptCurrentEntry()
{
  if ( mHasQueuedRequest )
  {
    return;
  }
  else
  {
//    if ( !mResultsView->isVisible() )
//      return;

//    QModelIndex index = mResultsView->currentIndex();
//    if ( !index.isValid() )
//      return;

//    QgsLocatorResult result = mProxyModel->data( index, QgsLocatorModel::ResultDataRole ).value< QgsLocatorResult >();
//    mResultsContainer->hide();
//    mLineEdit->clearFocus();
//    mLocator->clearPreviousResults();
//    result.filter->triggerResult( result );
  }
}
