

# File featurehistory.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featurehistory.h**](featurehistory_8h.md)

[Go to the documentation of this file](featurehistory_8h.md)


```C++
/***************************************************************************
                        featurehistory.h
                        ----------------
  begin                : Dec 2023
  copyright            : (C) 2023 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FEATUREHISTORY_H
#define FEATUREHISTORY_H

#include <QObject>
#include <QTimer>
#include <qgsproject.h>

class TrackingModel;

typedef QPair<QgsFeature, QgsFeature> OldNewFeaturePair;

class FeatureHistory : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool isUndoAvailable READ isUndoAvailable NOTIFY isUndoAvailableChanged )
    Q_PROPERTY( bool isRedoAvailable READ isRedoAvailable NOTIFY isRedoAvailableChanged )

  public:
    struct FeatureModifications
    {
        FeatureModifications()
        {}

        QList<OldNewFeaturePair> createdFeatures;
        QList<OldNewFeaturePair> updatedFeatures;
        QList<OldNewFeaturePair> deletedFeatures;
    };

    explicit FeatureHistory( const QgsProject *project, TrackingModel *trackingModel = nullptr );

    Q_INVOKABLE bool undo();

    Q_INVOKABLE bool redo();

    Q_INVOKABLE const QString undoMessage();

    Q_INVOKABLE const QString redoMessage();

    bool isUndoAvailable();
    bool isRedoAvailable();

  signals:
    void isUndoAvailableChanged();
    void isRedoAvailableChanged();

  private slots:
    void onLayersAdded( const QList<QgsMapLayer *> &layers );

    void onHomePathChanged();

    void onCommittedFeaturesAdded( const QString &localLayerId, const QgsFeatureList &addedFeatures );

    void onCommittedFeaturesRemoved( const QString &layerId, const QgsFeatureIds &deletedFeatureIds );

    void onBeforeCommitChanges();

    void onAfterCommitChanges();

    void onTimerTimeout();

    void onLayerInTrackingChanged( QgsVectorLayer *vl, bool isTracking );

  private:
    static const int sTimeoutMs = 50;

    void addLayerListeners();

    bool applyModifications( QMap<QString, FeatureModifications> &modificationsByLayerId );

    QMap<QString, FeatureModifications> reverseModifications( QMap<QString, FeatureModifications> &modificationsByLayerId );

    const QgsProject *mProject = nullptr;

    TrackingModel *mTrackingModel = nullptr;

    bool mIsApplyingModifications = false;

    QTimer mTimer;

    QMap<QString, FeatureModifications> mTempHistoryStep;

    QMap<QString, QgsFeatureIds> mTempModifiedFeatureIdsByLayerId;

    QMap<QString, QMap<QgsFeatureId, QgsFeature>> mTempModifiedFeaturesByLayerId;

    QMap<QString, QgsFeatureIds> mTempDeletedFeatureIdsByLayerId;

    QList<QMap<QString, FeatureModifications>> mUndoHistory;

    QList<QMap<QString, FeatureModifications>> mRedoHistory;

    QSet<QString> mObservedLayerIds;
};

#endif // FEATUREHISTORY_H
```


