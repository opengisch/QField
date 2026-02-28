

# File deltafilewrapper.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**deltafilewrapper.h**](deltafilewrapper_8h.md)

[Go to the documentation of this file](deltafilewrapper_8h.md)


```C++
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

class DeltaFileWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int count READ count NOTIFY countChanged )
    Q_PROPERTY( bool isPushing READ isPushing NOTIFY isPushingChanged )
    Q_PROPERTY( bool hasError READ hasError NOTIFY errorChanged )
    Q_PROPERTY( ErrorType errorType READ errorType NOTIFY errorChanged )
    Q_PROPERTY( QString errorString READ errorString NOTIFY errorChanged )

  public:
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


    
    DeltaFileWrapper( const QString &projectId, const QString &fileName );

    ~DeltaFileWrapper();


    static QStringList attachmentFieldNames( const QgsProject *project, const QString &layerId );

    static QString crsByLayerId( const QgsProject *project, const QString &layerId );

    static QString nameByLayerId( const QgsProject *project, const QString &layerId );

    static QPair<int, QString> getLocalPkAttribute( const QgsVectorLayer *vl );


    static QPair<int, QString> getSourcePkAttribute( const QgsVectorLayer *vl );


    static QString getSourceLayerId( const QgsVectorLayer *vl );


    Q_INVOKABLE void reset();


    Q_INVOKABLE void resetId();


    QString id() const;


    QString fileName() const;


    QString projectId() const;


    bool hasError() const;


    bool isDirty() const;


    Q_INVOKABLE int count() const;


    QJsonArray deltas() const;


    ErrorType errorType() const;

    QString errorString() const;


    QByteArray toJson( QJsonDocument::JsonFormat jsonFormat = QJsonDocument::Indented ) const;


    QString toString() const;


    Q_INVOKABLE bool toFile();


    Q_INVOKABLE QString toFileForPush( const QString &outFileName = QString() ) const;


    Q_INVOKABLE bool isCreatedFeature( QgsVectorLayer *vl, QgsFeature feature );


    bool append( const DeltaFileWrapper *deltaFileWrapper );


    QMap<QString, QString> attachmentFileNames() const;


    void addCreate( const QgsProject *project, const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &newFeature );


    void addDelete( const QgsProject *project, const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &oldFeature );


    void addPatch( const QgsProject *project, const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &oldFeature, const QgsFeature &newFeature, bool storeSnapshot = true );


    QStringList deltaLayerIds() const;


    bool isDeltaBeingApplied() const;


    Q_INVOKABLE bool apply( const QgsProject *project );


    Q_INVOKABLE bool applyReversed( const QgsProject *project );

    bool isPushing() const { return mIsPushing; }


    void setIsPushing( bool isPushing );


    int getDeltaIndexByUuid( const QString &uuid ) const;


  signals:
    void countChanged();


    void isPushingChanged();

    void savedToFile();

    void errorChanged();

  private:
    void setError( const ErrorType &type, const QString &details = QString() );

    QJsonValue geometryToJsonValue( const QgsGeometry &geom ) const;

    bool applyInternal( const QgsProject *project, bool shouldApplyInReverse );


    bool applyDeltasOnLayers( QHash<QString, QgsVectorLayer *> &vectorLayers, bool shouldApplyInReverse );

    std::tuple<QJsonObject, QJsonObject> addAttachments( const QgsProject *project, const QString &localLayerId, const QJsonObject &newAttrs, const QJsonObject &oldAttrs = QJsonObject() );

    QJsonValue attributeToJsonValue( const QVariant &value );


    void appendDelta( const QJsonObject &delta );


    void mergeDelta( const QJsonObject &delta );


    void mergeCreateDelta( const QJsonObject &delta );


    void mergeDeleteDelta( const QJsonObject &delta );


    void mergePatchDelta( const QJsonObject &delta );

    bool deltaContainsActualChange( const QJsonObject &delta ) const;

    QMap<QString, QMap<QString, QString>> mLocalPkToDeltaUuid;

    QJsonArray mDeltas;

    QList<QJsonObject> mPendingDeltas;

    QJsonObject mJsonRoot;


    QString mFileName;


    QString mCloudProjectId;


    ErrorType mErrorType = ErrorType::NoError;


    QString mErrorDetails;


    bool mIsDirty = false;


    bool mIsPushing = false;


    bool mIsDeltaFileBeingApplied = false;
};

#endif // FEATUREDELTAS_H
```


