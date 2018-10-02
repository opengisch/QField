#ifndef QGSQUICKLOCATORWIDGET_H
#define QGSQUICKLOCATORWIDGET_H

#include <QQuickItem>

class QgsLocator;

class QgsQuickLocatorWidget : public QQuickItem
{
    Q_OBJECT
public:
    QgsQuickLocatorWidget(QQuickItem *parent = nullptr);

    QgsLocator *locator() const;

signals:

public slots:

private:
    QgsLocator *mLocator;
};

#endif // QGSQUICKLOCATORWIDGET_H
