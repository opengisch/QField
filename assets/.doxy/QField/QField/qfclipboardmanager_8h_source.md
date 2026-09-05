

# File qfclipboardmanager.h

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfclipboardmanager.h**](qfclipboardmanager_8h.md)

[Go to the documentation of this file](qfclipboardmanager_8h.md)


```C++
/***************************************************************************
  qfclipboardmanager.h - QfClipboardManager

 ---------------------
 begin                : 27.5.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QFCLIPBOARDMANAGER_H
#define QFCLIPBOARDMANAGER_H

#include <QClipboard>
#include <QMimeData>
#include <QObject>
#include <qgsfeature.h>

class QgsVectorLayer;

class QfClipboardManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool holdsFeature READ holdsFeature NOTIFY holdsFeatureChanged )

  public:
    explicit QfClipboardManager( QObject *parent = nullptr );
    ~QfClipboardManager() = default;

    bool holdsFeature() const { return mHoldsFeature; }

    Q_INVOKABLE void copyFeatureToClipboard( const QgsFeature &feature, bool includeGeometry = false );

    Q_INVOKABLE void copyFeatureToClipboard( QgsVectorLayer *layer, QgsFeatureId fid, bool includeGeometry = false, bool isCutOperation = false );

    Q_INVOKABLE QgsFeature pasteFeatureFromClipboard();

    Q_INVOKABLE bool pasteFeatureFromClipboardIntoLayer( QgsVectorLayer *layer );

  signals:

    void holdsFeatureChanged();

  private slots:

    void dataChanged();

  private:
    QClipboard *mClipboard = nullptr;
    bool mSkipDataChanged = false;
    bool mHoldsFeature = false;
    bool mHasNativeFeature = false;
    QgsVectorLayer *mSourceLayer = nullptr;
    bool mIsCutOperation = false;
    QgsFeature mNativeFeature;
    QString mHtmlFeature;
};

#endif // QFCLIPBOARDMANAGER_H
```


