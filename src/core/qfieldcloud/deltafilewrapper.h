/***************************************************************************
                        deltafilewrapper.h
                        ------------------
  begin                : Apr 2020
  copyright            : (C) 2020 by Ivan Ivanov
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

#ifndef FEATUREDELTAS_H
#define FEATUREDELTAS_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <qgsfeature.h>
#include <qgslogger.h>
#include <qgsvectorlayer.h>

const QString DeltaFormatVersion = QStringLiteral( "1.0" );

/**
 * A class that wraps the operations with a delta file. All read and write operations to a delta file should go through this class.
 * \ingroup core
 */
class DeltaFileWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int count READ count NOTIFY countChanged )
    Q_PROPERTY( bool isPushing READ isPushing NOTIFY isPushingChanged )
    Q_PROPERTY( bool hasError READ hasError NOTIFY errorChanged )
    Q_PROPERTY( ErrorType errorType READ errorType NOTIFY errorChanged )
    Q_PROPERTY( QString errorString READ errorString NOTIFY errorChanged )

  public:
    /**
     * Error types
     */
    enum class ErrorType
    {
      NoError,
      LockError,
      NotCloudProjectError,
      IOError,
      JsonParseError,
      JsonFormatIdError,
      JsonFormatProjectIdError,
      JsonFormatVersionError,
      JsonFormatDeltasError,
      JsonFormatDeltaItemError,
      JsonIncompatibleVersionError
    };
    Q_ENUM( ErrorType )


    /**
     * Construct a new Feature Deltas object.
     *
     * @param project the current project
     * @param fileName complete file name with path where the object should be stored
     */
    DeltaFileWrapper( const QString &projectId, const QString &fileName );

    /**
     * Destroy the Delta File Wrapper object
     */
    ~DeltaFileWrapper();


    /**
     * Returns a list of field names that have edit form as attachments
     *
     * @param project current project instance
     * @param layerId layer ID
     * @return QStringList list of field names
     */
    static QStringList attachmentFieldNames( const QgsProject *project, const QString &layerId );

    /**
     * Returns the CRS as WKT for given \a layerId in the \a project
     */
    static QString crsByLayerId( const QgsProject *project, const QString &layerId );

    /**
     * Returns the name for given \a layerId in the \a project
     */
    static QString nameByLayerId( const QgsProject *project, const QString &layerId );

    /**
     * @brief Get a pair of the index and the name of the primary key column of given \a vl layer. We assume there is no multi-column primary key.
     */
    static QPair<int, QString> getLocalPkAttribute( const QgsVectorLayer *vl );


    /**
     * @brief Get a pair of the index and the name of the primary key column of given \a vl layer, as they are in the original source.
     */
    static QPair<int, QString> getSourcePkAttribute( const QgsVectorLayer *vl );


    /**
     * Get the layer id as it is in the original source.
     */
    static QString getSourceLayerId( const QgsVectorLayer *vl );


    /**
     * Clears the deltas from memory as there are no deltas at all. Does not affect the permanent storage until `toFile()` is called.
     */
    Q_INVOKABLE void reset();


    /**
     * Assigns a new unique delta file id.
     */
    Q_INVOKABLE void resetId();


    /**
     * Returns deltas file id.
     *
     * @return QString id
     */
    QString id() const;


    /**
     * Returns deltas file name.
     *
     * @return QString file name
     */
    QString fileName() const;


    /**
     * Returns deltas file project id.
     *
     * @return QString project id
     */
    QString projectId() const;


    /**
     * Returns whether the class has encountered I/O error regarding the delta file. If true is returned, the behaviour of the class instance is no more defined.
     *
     * @return bool whether an error has been encountered
     */
    bool hasError() const;


    /**
     * Returns whether the instance contents differs from the data saved on the disk.
     *
     * @return bool whether there is a difference with the data saved on the disk
     */
    bool isDirty() const;


    /**
     * Returns the number of delta elements
     *
     * @return int number of delta elements
     */
    Q_INVOKABLE int count() const;


    /**
     * Returns the deltas as a JSON array of delta elements
     *
     * @return QJsonArray deltas JSON array
     */
    QJsonArray deltas() const;


    /**
     * Error type why the class has an error.
     *
     * @return ErrorType error type
     */
    ErrorType errorType() const;

    /**
     * Human readable error description why the class has an error.
     *
     * @return QString human readable error reason
     */
    QString errorString() const;


    /**
     * Returns deltas as JSON QByteArray, ready for I/O operations.
     *
     * @param jsonFormat formatting of the output JSON. Default: QJsonDocument::Indented
     * @return QByteArray JSON representation
     */
    QByteArray toJson( QJsonDocument::JsonFormat jsonFormat = QJsonDocument::Indented ) const;


    /**
     * Returns deltas as JSON string.
     *
     * @return QString JSON representation
     */
    QString toString() const;


    /**
     * Writes deltas file to the permanent storage.
     *
     * @return bool whether write has been successful
     */
    Q_INVOKABLE bool toFile();


    /**
     * Writes deltas file to the permanent storage with replaced layerIds, ready for upload.
     *
     * @return QString an empty string
     */
    Q_INVOKABLE QString toFileForPush( const QString &outFileName = QString() ) const;


    /**
     * Returns TRUE if a feature from a given vector layer is recorded as being
     * created in the deltas file.
     *
     * @param vl the vector layer
     * @param feature the feature
     */
    Q_INVOKABLE bool isCreatedFeature( QgsVectorLayer *vl, QgsFeature feature );


    /**
     * Appends the provided deltas JSON array at the end of the current file.
     */
    bool append( const DeltaFileWrapper *deltaFileWrapper );


    /**
     * Returns a set of file names to be uploaded
     *
     * @return QMap<QString, QString> unique file names
     */
    QMap<QString, QString> attachmentFileNames() const;


    /**
     * Adds create delta.
     *
     * @param localLayerId layer ID where the old feature belongs to
     * @param sourceLayerId layer ID where the old feature belongs to
     * @param localPkAttrName attribute name of the primary key
     * @param sourcePkAttrName attribute name of the primary key
     * @param newFeature the feature that has been created
     */
    void addCreate( const QgsProject *project, const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &newFeature );


    /**
     * Adds delete delta.
     *
     * @param localLayerId layer ID where the old feature belongs to
     * @param sourceLayerId layer ID where the old feature belongs to
     * @param localPkAttrName attribute name of the primary key
     * @param sourcePkAttrName attribute name of the primary key
     * @param oldFeature the feature that has been deleted
     */
    void addDelete( const QgsProject *project, const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &oldFeature );


    /**
     * Adds patch delta.
     *
     * @param localLayerId layer ID where the old feature belongs to
     * @param sourceLayerId layer ID where the old feature belongs to
     * @param localPkAttrName attribute name of the primary key
     * @param sourcePkAttrName attribute name of the primary key
     * @param oldFeature the old version of the feature that has been modified
     * @param newFeature the new version of the feature that has been modified
     * @param storeSnapshot store a complete snapshot of the feature
     */
    void addPatch( const QgsProject *project, const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &oldFeature, const QgsFeature &newFeature, bool storeSnapshot = true );


    /**
     * Returns the list of layers that have applied deltas on them.
     * @return list of layers ids
     * @todo TEST
     **/
    QStringList deltaLayerIds() const;


    /**
     * Whether or not the current delta file is being applied.
     *
     * @todo TEST
     */
    bool isDeltaBeingApplied() const;


    /**
     * Attempts to apply a delta file.
     * The list of deltas is not being reset after successfull application and should be handled by the caller.
     *
     * @note it is not guaranteed that the project layers have not changed in case of failure
     * @return whether the attempt was successful
     * @todo TEST
     */
    Q_INVOKABLE bool apply( const QgsProject *project );


    /**
     * Attempts to apply a delta file in reverse order (resulting in local changes being discarded).
     * The list of deltas is not being reset after successfull application and should be handled by the caller.
     *
     * @note it is not guaranteed that the project layers have not changed in case of failure
     * @return whether the attempt was successful.
     * @todo TEST
     */
    Q_INVOKABLE bool applyReversed( const QgsProject *project );

    /**
     * Returns TRUE if the pushing state is active.
     */
    bool isPushing() const { return mIsPushing; }


    /**
     * Sets the pushing state.
     *
     * @param isPushing set to TRUE to reflect an ongoing pushing state.
     */
    void setIsPushing( bool isPushing );


    /**
     * Retuns the index position of a delta with given \a uuid in the deltas list or -1 if missing.
     *
     * @param uuid the uuid we are looking for
     */
    int getDeltaIndexByUuid( const QString &uuid ) const;


  signals:
    /**
     * Emitted when the `deltas` list has changed.
     */
    void countChanged();


    /**
     * Emmitted when the pushing state has changed.
     */
    void isPushingChanged();

    /**
     * Emmitted when the deltas have been written to a file
     */
    void savedToFile();

    /**
     * Emmitted when the latest error has changed
     */
    void errorChanged();

  private:
    /**
     * Set the error type and details string.
     */
    void setError( const ErrorType &type, const QString &details = QString() );

    /**
     * Converts geometry to QJsonValue string in WKT format.
     * Returns null if the geometry is null, or WKT string of the geometry
     *
     */
    QJsonValue geometryToJsonValue( const QgsGeometry &geom ) const;

    /**
     * Applies the current delta file on the current project. A wrapper method arround \a _applyDeltasOnLayers.
     * If \a shouldApplyInReverse is passed, the deltas are applied in reverse order (e.g. discarding the changes).
     */
    bool applyInternal( const QgsProject *project, bool shouldApplyInReverse );


    /**
     * Applies the current delta file on a list of given layers \a vectorLayers.
     * If \a shouldApplyInReverse is passed, the deltas are applied in reverse order (e.g. discarding the changes).
     */
    bool applyDeltasOnLayers( QHash<QString, QgsVectorLayer *> &vectorLayers, bool shouldApplyInReverse );

    /**
     * Add file checksums from relevant changed attributes.
     * \returns A std::tuple<QJsonObject, QJsonObject> where the first object reflects new file checksums and the second reflects old file checkums.
     */
    std::tuple<QJsonObject, QJsonObject> addAttachments( const QgsProject *project, const QString &localLayerId, const QJsonObject &newAttrs, const QJsonObject &oldAttrs = QJsonObject() );

    /**
     * Converts QVariant value to QJsonValue
     */
    QJsonValue attributeToJsonValue( const QVariant &value );


    /**
     * Append generated \a delta.
     */
    void appendDelta( const QJsonObject &delta );


    /**
     * Merge the generated \a delta into stored deltas.
     */
    void mergeDelta( const QJsonObject &delta );


    /**
     * Merge the generated create \a delta into stored deltas. Should only be called from `mergeDelta` method.
     */
    void mergeCreateDelta( const QJsonObject &delta );


    /**
     * Merge the generated delete \a delta into stored deltas. Should only be called from `mergeDelta` method.
     */
    void mergeDeleteDelta( const QJsonObject &delta );


    /**
     * Merge the generated patch \a delta into stored deltas. Should only be called from `mergeDelta` method.
     */
    void mergePatchDelta( const QJsonObject &delta );

    /**
     * Checks whether the delta really has a change between the `old` and `new` attributes and geometry.
     *
     * It may not have any change in case undo/redo and delta merging is applied.
     */
    bool deltaContainsActualChange( const QJsonObject &delta ) const;

    /**
     * A mapping between the local primary key and the uuid of the delta.
     */
    QMap<QString, QMap<QString, QString>> mLocalPkToDeltaUuid;

    /**
     * The list of JSON deltas.
     */
    QJsonArray mDeltas;

    /**
     * The list of pending JSON deltas.
     */
    QList<QJsonObject> mPendingDeltas;

    /**
     * The root deltas JSON object.
     */
    QJsonObject mJsonRoot;


    /**
     * The delta file name.
     */
    QString mFileName;


    /**
     * The delta file project id.
     */
    QString mCloudProjectId;


    /**
     * Type of error that the constructor has encountered.
     */
    ErrorType mErrorType = ErrorType::NoError;


    /**
     * Additional details describing the error.
     */
    QString mErrorDetails;


    /**
     * Holds whether the deltas in the memory differ from the deltas in the file.
     */
    bool mIsDirty = false;


    /**
     * Holds whether the pushing state has been activated.
     */
    bool mIsPushing = false;


    /**
     * Whether the delta file is currently being applied.
     */
    bool mIsDeltaFileBeingApplied = false;
};

#endif // FEATUREDELTAS_H
