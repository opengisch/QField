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

/**
 * \ingroup core
 */
class FeatureHistory : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool isUndoAvailable READ isUndoAvailable NOTIFY isUndoAvailableChanged )
    Q_PROPERTY( bool isRedoAvailable READ isRedoAvailable NOTIFY isRedoAvailableChanged )

  public:
    /**
     * Stores the created, updated and deleted features on each undo/redo step.
     */
    struct FeatureModifications
    {
        FeatureModifications()
        {}

        QList<OldNewFeaturePair> createdFeatures;
        QList<OldNewFeaturePair> updatedFeatures;
        QList<OldNewFeaturePair> deletedFeatures;
    };

    /**
     * Construct a new Feature history object
     *
     * @param project the current project instance
     * @param trackingModel the tracking model
     */
    explicit FeatureHistory( const QgsProject *project, TrackingModel *trackingModel = nullptr );

    //! Perform undo of the most recent modification step
    Q_INVOKABLE bool undo();

    //! Perform redo of the most recent modification step
    Q_INVOKABLE bool redo();

    //! Get the undo message to be show in the UI. NOTE should be called before calling \a undo.
    Q_INVOKABLE const QString undoMessage();

    //! Get the redo message to be show in the UI. NOTE should be called before calling \a redo.
    Q_INVOKABLE const QString redoMessage();

    bool isUndoAvailable();
    bool isRedoAvailable();

  signals:
    void isUndoAvailableChanged();
    void isRedoAvailableChanged();

  private slots:
    /**
     * Monitors the current project for new layers.
     *
     * @param layers layers added
     */
    void onLayersAdded( const QList<QgsMapLayer *> &layers );

    //! The project file has been changed
    void onHomePathChanged();

    //! Called when features are added on the layer
    void onCommittedFeaturesAdded( const QString &localLayerId, const QgsFeatureList &addedFeatures );

    //! Called after features are committed for deletion.
    void onCommittedFeaturesRemoved( const QString &layerId, const QgsFeatureIds &deletedFeatureIds );

    //! Called before features are committed. Used to prepare the old state of the features and store it in \a mHistry.
    void onBeforeCommitChanges();

    //! Called after features are committed. Used because the added features do not have FID before they are committed.
    void onAfterCommitChanges();

    //! Timer's timeout slot. Used to collect multiple feature changes (calls of \a onBeforeCommitChanges and \a onAfterCommitChanges) into one undo step.
    void onTimerTimeout();

    //! Layer in tracking changed. If layer is in tracking mode, we should not track changes.
    void onLayerInTrackingChanged( QgsVectorLayer *vl, bool isTracking );

  private:
    static const int sTimeoutMs = 50;

    //! Add the needed event listeners to monitor for changes.
    void addLayerListeners();

    //! Apply given modifications on all layers in the current project. Used both by undo and redo operations.
    bool applyModifications( QMap<QString, FeatureModifications> &modificationsByLayerId );

    //! Reverse the modification. Used to make undo modifications into redo modifications.
    QMap<QString, FeatureModifications> reverseModifications( QMap<QString, FeatureModifications> &modificationsByLayerId );

    //! The current project instance.
    const QgsProject *mProject = nullptr;

    //! Tracking model. Used to check if the currently modified layer is in tracking, so all changes should be ignored to prevent cluttering the undo history.
    TrackingModel *mTrackingModel = nullptr;

    //! If currently applying undo or redo feature modifications.
    bool mIsApplyingModifications = false;

    //! Timer to wait for short time before all features (and child features) are saved. When timeouts, all temporary modifications are added as a new undo step.
    QTimer mTimer;

    //! Temporary storage of all modifications before creating a new undo step.
    QMap<QString, FeatureModifications> mTempHistoryStep;

    //! Track committed feature IDs that were added, modified, or removed
    QMap<QString, QgsFeatureIds> mTempModifiedFeatureIdsByLayerId;

    //! Temporary storage of all features that have been modified before creating a new undo step.
    QMap<QString, QMap<QgsFeatureId, QgsFeature>> mTempModifiedFeaturesByLayerId;

    //! Temporary storage of the deleted feature ids before creating a new undo step.
    QMap<QString, QgsFeatureIds> mTempDeletedFeatureIdsByLayerId;

    //! Undo history records
    QList<QMap<QString, FeatureModifications>> mUndoHistory;

    //! Redo history records
    QList<QMap<QString, FeatureModifications>> mRedoHistory;

    //! Layer ids being observed for changes. Should reset when the project is changed. Used to prevent double event listeners.
    QSet<QString> mObservedLayerIds;
};

#endif // FEATUREHISTORY_H
