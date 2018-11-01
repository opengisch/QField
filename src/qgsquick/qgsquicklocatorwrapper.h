#ifndef QGSQUICKLOCATORWRAPPER_H
#define QGSQUICKLOCATORWRAPPER_H

#include <QQuickItem>

#include <QTimer>

class QgsLocator;
class QgsLocatorModel;
class QgsLocatorContext;
class QgsLocatorProxyModel;
class QgsLocatorResult;
class QgsQuickMapSettings;

class QgsQuickLocatorWrapper : public QQuickItem
{
    Q_OBJECT
    //Q_PROPERTY( bool isRunning READ isRunning NOTIFY isRunningChanged ) TODO
  public:
    QgsQuickLocatorWrapper( QQuickItem *parent = nullptr );

    void setLocator( QgsLocator *locator );
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QgsLocator *locator() const;

  public slots:
    Q_INVOKABLE void setSearchText( const QString &text );
    void clearSearch();

  private slots:
    void scheduleDelayedPopup();
    void showList();
    void triggerSearchAndShowList();
    void searchFinished();
    void addResult( const QgsLocatorResult &result );

  private:
    void updateResults( const QString &text );
    QgsLocatorContext createContext();
    void acceptCurrentEntry();

    QgsLocator *mLocator = nullptr;
    QgsLocatorModel *mLocatorModel = nullptr;
    QgsLocatorProxyModel *mProxyModel = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;

    QTimer mPopupTimer;
    QTimer mFocusTimer;

    QString mNextRequestedString;
    bool mHasQueuedRequest = false;
};

#endif // QGSQUICKLOCATORWRAPPER_H
