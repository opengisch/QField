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

#include <qgsfeature.h>
#include <qgsvectorlayer.h>
#include <qgslogger.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

/**
 * A class that wraps the operations with a delta file. All read and write operations to a delta file should go through this class.
 *
 */
class DeltaFileWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int count READ count NOTIFY countChanged )

  public:
    /**
     * Error types
     */
    enum ErrorTypes
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


    /**
     * Stores the current version of the format.
     */
    const static QString FormatVersion;


    /**
     * Construct a new Feature Deltas object.
     *
     * @param fileName complete file name with path where the object should be stored
     */
    DeltaFileWrapper( const QgsProject *project, const QString &fileName );

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
     * @return ErrorTypes error type
     */
    ErrorTypes errorType() const;

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
    Q_INVOKABLE QString toFileForUpload( const QString &outFileName = QString() ) const;


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
    void addCreate( const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &newFeature );


    /**
     * Adds delete delta.
     *
     * @param localLayerId layer ID where the old feature belongs to
     * @param sourceLayerId layer ID where the old feature belongs to
     * @param localPkAttrName attribute name of the primary key
     * @param sourcePkAttrName attribute name of the primary key
     * @param oldFeature the feature that has been deleted
     */
    void addDelete( const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &oldFeature );


    /**
     * Adds patch delta.
     *
     * @param localLayerId layer ID where the old feature belongs to
     * @param sourceLayerId layer ID where the old feature belongs to
     * @param localPkAttrName attribute name of the primary key
     * @param sourcePkAttrName attribute name of the primary key
     * @param oldFeature the old version of the feature that has been modified
     * @param newFeature the new version of the feature that has been modified
     */
    void addPatch( const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &oldFeature, const QgsFeature &newFeature );


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
    Q_INVOKABLE bool apply();


    /**
     * Attempts to apply a delta file in reverse order (resulting in local changes being discarded).
     * The list of deltas is not being reset after successfull application and should be handled by the caller.
     *
     * @note it is not guaranteed that the project layers have not changed in case of failure
     * @return whether the attempt was successful.
     * @todo TEST
     */
    Q_INVOKABLE bool applyReversed();


  signals:
    /**
     * Emitted when the `deltas` list has changed.
     *
     * @todo TEST
     */
    void countChanged();


    /**
     *
     * @todo TEST
     */
    void savedToFile();


  private:

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
    bool applyInternal( bool shouldApplyInReverse );


    /**
     * Applies the current delta file on a list of given layers \a vectorLayers.
     * If \a shouldApplyInReverse is passed, the deltas are applied in reverse order (e.g. discarding the changes).
     */
    bool applyDeltasOnLayers( QHash<QString, QgsVectorLayer *> &vectorLayers, bool shouldApplyInReverse );


    /**
     * The current project instance
     */
    const QgsProject *mProject = nullptr;


    /**
     * A mapping between the local primary key and it's index in the delta file.
     */
    QMap<QString, QMap<QString, int>> mLocalPkDeltaIdx;

    /**
     * The list of JSON deltas.
     */
    QJsonArray mDeltas;


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
    ErrorTypes mErrorType = NoError;


    /**
     * Additional details describing the error.
     */
    QString mErrorDetails;


    /**
     * Holds whether the deltas in the memory differ from the deltas in the file.
     */
    bool mIsDirty = false;


    /**
     * Whether the delta file is currently being applied.
     */
    bool mIsDeltaFileBeingApplied = false;
};

#endif // FEATUREDELTAS_H
