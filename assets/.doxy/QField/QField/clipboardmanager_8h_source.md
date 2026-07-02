

# File clipboardmanager.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**clipboardmanager.h**](clipboardmanager_8h.md)

[Go to the documentation of this file](clipboardmanager_8h.md)


```C++
/***************************************************************************
  clipboardmanager.h - ClipboardManager

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


#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QClipboard>
#include <QMimeData>
#include <QObject>
#include <qgsfeature.h>

class QgsVectorLayer;

class ClipboardManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool holdsFeature READ holdsFeature NOTIFY holdsFeatureChanged )

  public:
    explicit ClipboardManager( QObject *parent = nullptr );
    ~ClipboardManager() {}

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

#endif // PROJECTINFO_H
```


