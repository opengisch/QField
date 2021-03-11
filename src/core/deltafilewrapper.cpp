/***************************************************************************
                        deltafilewrapper.cpp
                        --------------------
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

#include "deltafilewrapper.h"
#include "qfield.h"
#include "utils/fileutils.h"
#include "utils/qfieldcloudutils.h"

#include <QFileInfo>
#include <QFile>
#include <QUuid>

#include <qgsproject.h>
#include <qgsvectorlayerutils.h>
#include <qgsmessagelog.h>


const QString DeltaFileWrapper::FormatVersion = QStringLiteral( "1.0" );

/**
 * Attachment fields cache.
 */
typedef QMap<QString, QStringList> CacheAttachmentFieldNamesMap;
Q_GLOBAL_STATIC( CacheAttachmentFieldNamesMap, sCacheAttachmentFieldNames );

/**
 * Storage to keep track of the currently opened files. The stored paths are absolute, to ensure they are unique.
 */
Q_GLOBAL_STATIC( QSet<QString>, sFileLocks );


DeltaFileWrapper::DeltaFileWrapper( const QgsProject *project, const QString &fileName )
  : mProject( project )
{
  QFileInfo fileInfo = QFileInfo( fileName );

  // we need to resolve all symbolic links are relative paths, so we produce a unique file path to the file.
  // Because the file may not exist yet, we cannot use QFileInfo::canonicalFilePath() as it returns an empty string if it fails to resolve.
  // However, we assume that the parent directory exists.
  mFileName = fileInfo.canonicalFilePath().isEmpty() ? fileInfo.absoluteFilePath() : fileInfo.canonicalFilePath();
  mErrorType = DeltaFileWrapper::NoError;

#if 0
//  TODO enable this code once we have a single delta pointer stored per project and passed to the layer observer.
//  Now both the qfieldcloudprojects model (Read only) and the layer observer (Read/Write) create their pointers to the deltafilewrapper
  if ( mErrorType == DeltaFileWrapper::NoError && sFileLocks()->contains( mFileName ) )
    mErrorType = DeltaFileWrapper::LockError;
#endif

  if ( mErrorType == DeltaFileWrapper::NoError )
    mCloudProjectId = QFieldCloudUtils::getProjectId( mProject );

  if ( mErrorType == DeltaFileWrapper::NoError && mCloudProjectId.isNull() )
    mErrorType = DeltaFileWrapper::NotCloudProjectError;

  QFile deltaFile( mFileName );

  if ( mErrorType == DeltaFileWrapper::NoError && QFileInfo::exists( mFileName ) )
  {
    QJsonParseError jsonError;

    QgsLogger::debug( QStringLiteral( "Loading deltas from %1" ).arg( mFileName ) );

    if ( mErrorType == DeltaFileWrapper::NoError && ! deltaFile.open( QIODevice::ReadWrite ) )
    {
      mErrorType = DeltaFileWrapper::IOError;
      mErrorDetails = deltaFile.errorString();
    }

    if ( mErrorType == DeltaFileWrapper::NoError )
      mJsonRoot = QJsonDocument::fromJson( deltaFile.readAll(), &jsonError ).object();

    if ( mErrorType == DeltaFileWrapper::NoError && ( jsonError.error != QJsonParseError::NoError ) )
    {
      mErrorType = DeltaFileWrapper::JsonParseError;
      mErrorDetails = jsonError.errorString();
    }

    if ( mErrorType == DeltaFileWrapper::NoError && ( ! mJsonRoot.value( QStringLiteral( "id" ) ).isString() || mJsonRoot.value( QStringLiteral( "id" ) ).toString().isEmpty() ) )
      mErrorType = DeltaFileWrapper::JsonFormatIdError;

    if ( mErrorType == DeltaFileWrapper::NoError && ( ! mJsonRoot.value( QStringLiteral( "project" ) ).isString() || mJsonRoot.value( QStringLiteral( "project" ) ).toString().isEmpty() ) )
      mErrorType = DeltaFileWrapper::JsonFormatProjectIdError;

    if ( mErrorType == DeltaFileWrapper::NoError && ! mJsonRoot.value( QStringLiteral( "deltas" ) ).isArray() )
      mErrorType = DeltaFileWrapper::JsonFormatDeltasError;

    if ( mErrorType == DeltaFileWrapper::NoError && ( ! mJsonRoot.value( QStringLiteral( "version" ) ).isString() || mJsonRoot.value( QStringLiteral( "version" ) ).toString().isEmpty() ) )
      mErrorType = DeltaFileWrapper::JsonFormatVersionError;

    if ( mErrorType == DeltaFileWrapper::NoError && mJsonRoot.value( QStringLiteral( "version" ) ) != DeltaFileWrapper::FormatVersion )
      mErrorType = DeltaFileWrapper::JsonIncompatibleVersionError;

    if ( mErrorType == DeltaFileWrapper::NoError )
    {
      const QJsonArray deltasJsonArray = mJsonRoot.value( QStringLiteral( "deltas" ) ).toArray();

      for ( const QJsonValue &v : deltasJsonArray )
      {
        if ( ! v.isObject() )
        {
          mErrorType = DeltaFileWrapper::JsonFormatDeltaItemError;
          continue;
        }

        // TODO validate delta item properties

        mDeltas.append( v );
      }
    }
  }
  else if ( mErrorType == DeltaFileWrapper::NoError )
  {
    mJsonRoot = QJsonObject( {{"version", DeltaFileWrapper::FormatVersion},
      {"id", QUuid::createUuid().toString( QUuid::WithoutBraces )},
      {"project", mCloudProjectId},
      {"deltas", mDeltas}} );

    if ( ! deltaFile.open( QIODevice::ReadWrite ) )
    {
      mErrorType = DeltaFileWrapper::IOError;
      mErrorDetails = deltaFile.errorString();
    }

    // toFile() modifies mErrorType and mErrorDetails, that's why we ignore the boolean return
    toFile();
  }
  else
  {
    return;
  }

  sFileLocks()->insert( mFileName );
}


DeltaFileWrapper::~DeltaFileWrapper()
{
  sFileLocks()->remove( mFileName );
}


QString DeltaFileWrapper::id() const
{
  return mJsonRoot.value( QStringLiteral( "id" ) ).toString();
}


QString DeltaFileWrapper::fileName() const
{
  return mFileName;
}


QString DeltaFileWrapper::projectId() const
{
  return mCloudProjectId;
}


void DeltaFileWrapper::reset()
{
  if ( ! mIsDirty && mDeltas.size() == 0 )
    return;

  mIsDirty = true;
  mDeltas = QJsonArray();
  mLocalPkDeltaIdx.clear();

  emit countChanged();
}


void DeltaFileWrapper::resetId()
{
  mJsonRoot.insert( QStringLiteral( "id" ), QUuid::createUuid().toString( QUuid::WithoutBraces ) );
}


bool DeltaFileWrapper::hasError() const
{
  return mErrorType != DeltaFileWrapper::NoError;
}


bool DeltaFileWrapper::isDirty() const
{
  return mIsDirty;
}


int DeltaFileWrapper::count() const
{
  return mDeltas.size();
}


QJsonArray DeltaFileWrapper::deltas() const
{
  return mDeltas;
}


DeltaFileWrapper::ErrorTypes DeltaFileWrapper::errorType() const
{
  return mErrorType;
}


QString DeltaFileWrapper::errorString() const
{
  const QHash<DeltaFileWrapper::ErrorTypes, QString> errorMessages(
  {
    {DeltaFileWrapper::NoError, QString()},
    {DeltaFileWrapper::LockError, QStringLiteral( "Delta file is already opened" )},
    {DeltaFileWrapper::NotCloudProjectError, QStringLiteral( "The current project is not a cloud project" ) },
    {DeltaFileWrapper::IOError, QStringLiteral( "Cannot open file for read and write" ) },
    {DeltaFileWrapper::JsonParseError, QStringLiteral( "Unable to parse JSON" ) },
    {DeltaFileWrapper::JsonFormatIdError, QStringLiteral( "Delta file is missing a valid id" ) },
    {DeltaFileWrapper::JsonFormatProjectIdError, QStringLiteral( "Delta file is missing a valid project id" ) },
    {DeltaFileWrapper::JsonFormatVersionError, QStringLiteral( "Delta file is missing a valid version" ) },
    {DeltaFileWrapper::JsonFormatDeltasError, QStringLiteral( "Delta file is missing a valid deltas" ) },
    {DeltaFileWrapper::JsonFormatDeltaItemError, QStringLiteral( "Delta file is missing a valid delta item" ) },
    {DeltaFileWrapper::JsonIncompatibleVersionError, QStringLiteral( "Delta file has incompatible version" ) }
  } );

  Q_ASSERT( errorMessages.contains( mErrorType ) );

  return QStringLiteral( "%1\n%2" ).arg( errorMessages.value( mErrorType ), mErrorDetails );
}


QByteArray DeltaFileWrapper::toJson( QJsonDocument::JsonFormat jsonFormat ) const
{
  QJsonObject jsonRoot( mJsonRoot );
  jsonRoot.insert( QStringLiteral( "deltas" ), mDeltas );
  jsonRoot.insert( QStringLiteral( "files" ), QJsonArray() );

  return QJsonDocument( jsonRoot ).toJson( jsonFormat );
}


QString DeltaFileWrapper::toString() const
{
  return QString::fromStdString( toJson().toStdString() );
}


bool DeltaFileWrapper::toFile()
{
  QFile deltaFile( mFileName );

  if ( ! deltaFile.open( QIODevice::WriteOnly | QIODevice::Unbuffered ) )
  {
    mErrorType = DeltaFileWrapper::IOError;
    mErrorDetails = deltaFile.errorString();
    QgsMessageLog::logMessage( QStringLiteral( "File %1 cannot be open for writing. Reason: %2" ).arg( mFileName ).arg( mErrorDetails ) );

    return false;
  }

  if ( deltaFile.write( toJson() )  == -1 )
  {
    mErrorType = DeltaFileWrapper::IOError;
    mErrorDetails = deltaFile.errorString();
    QgsMessageLog::logMessage( QStringLiteral( "Contents of the file %1 has not been written. Reason %2" ).arg( mFileName ).arg( mErrorDetails ) );
    return false;
  }

  deltaFile.close();
  mIsDirty = false;
  // QgsLogger::debug( "Finished writing deltas JSON" );

  emit savedToFile();

  return true;
}


QString DeltaFileWrapper::toFileForUpload( const QString &outFileName ) const
{
    QString fileName = outFileName;

    if ( fileName.isEmpty() )
    {
        QTemporaryFile tempFile;

        if ( !tempFile.open() )
            return QString();

        fileName = tempFile.fileName();
    }

    QJsonArray resultDeltas;
    QJsonObject jsonRoot( mJsonRoot );

    jsonRoot.insert( QStringLiteral( "deltas" ), deltas() );
    jsonRoot.insert( QStringLiteral( "files" ), QJsonArray() );

    QFile deltaFile( fileName );

    if ( ! deltaFile.open( QIODevice::WriteOnly | QIODevice::Unbuffered ) )
      return QString();

    if ( deltaFile.write( QJsonDocument( jsonRoot ).toJson( QJsonDocument::Indented ) )  == -1 )
      return QString();

    return fileName;
}


bool DeltaFileWrapper::append( const DeltaFileWrapper *deltaFileWrapper )
{
  if ( ! deltaFileWrapper )
    return false;

  if ( deltaFileWrapper->hasError() )
    return false;

  const QJsonArray constDeltas = deltaFileWrapper->deltas();

  for ( const QJsonValue &delta : constDeltas )
    mDeltas.append( delta );

  emit countChanged();

  return true;
}


QStringList DeltaFileWrapper::attachmentFieldNames( const QgsProject *project, const QString &layerId )
{
  QStringList attachmentFieldNames;

  if ( ! project )
    return attachmentFieldNames;

  if ( sCacheAttachmentFieldNames()->contains( layerId ) )
    return sCacheAttachmentFieldNames()->value( layerId );

  const QgsVectorLayer *vl = static_cast<QgsVectorLayer *>( project->mapLayer( layerId ) );

  if ( ! vl )
    return attachmentFieldNames;

  const QgsFields fields = vl->fields();

  for ( const QgsField &field : fields )
  {
    if ( field.editorWidgetSetup().type() == QStringLiteral( "ExternalResource" ) )
      attachmentFieldNames.append( field.name() );
  }

  sCacheAttachmentFieldNames()->insert( layerId, attachmentFieldNames );

  return attachmentFieldNames;
}


QMap<QString, QString> DeltaFileWrapper::attachmentFileNames() const
{
  // NOTE represents { layerId: { featureId: { attributeName: fileName } } }
  // We store all the changes in such mapping that we can return only the last attachment file name that is associated with a feature.
  // E.g. for given feature we start with attachment A.jpg, then we update to B.jpg. Later we change our mind and we apply C.jpg. In this case we only care about C.jpg.
  QMap<QString, QString> fileNames;
  QMap<QString, QString> fileChecksums;

  for ( const QJsonValue &deltaJson : qgis::as_const( mDeltas ) )
  {
    QVariantMap delta = deltaJson.toObject().toVariantMap();
    const QString localLayerId = delta.value( QStringLiteral( "localLayerId" ) ).toString();
    const QString method = delta.value( QStringLiteral( "method" ) ).toString();
    const QString localPk = delta.value( QStringLiteral( "localPk" ) ).toString();
    const QStringList attachmentFieldNamesList = attachmentFieldNames( mProject, localLayerId );

    if ( method == QStringLiteral( "delete" ) || method == QStringLiteral( "patch" ) )
    {
      const QVariantMap oldData = delta.value( QStringLiteral( "old" ) ).toMap();

      Q_ASSERT( ! oldData.isEmpty() );
    }

    if ( method == QStringLiteral( "create" ) || method == QStringLiteral( "patch" ) )
    {
      const QVariantMap newData = delta.value( QStringLiteral( "new" ) ).toMap();

      Q_ASSERT( ! newData.isEmpty() );

      if ( newData.contains( QStringLiteral( "files_sha256" ) ) )
      {
        const QVariantMap filesChecksum = newData.value( QStringLiteral( "files_sha256" ) ).toMap();

        Q_ASSERT( ! filesChecksum.isEmpty() );

        const QVariantMap attributes = newData.value( QStringLiteral( "attributes" ) ).toMap();
        const QStringList attributeNames = attributes.keys();

        for ( const QString &fieldName : attributeNames )
        {
          if ( ! attachmentFieldNamesList.contains( fieldName ) )
            continue;

          const QString fileName = attributes.value( fieldName ).toString();

          Q_ASSERT( filesChecksum.contains( fileName ) );

          const QString key = QStringLiteral( "%1//%2//%3" ).arg( localLayerId, localPk, fieldName );
          const QString fileChecksum = filesChecksum.value( fileName ).toString();

          fileNames.insert( key, fileName );
          fileChecksums.insert( fileName, fileChecksum );
        }
      }
    }

    if ( method != QStringLiteral( "create" ) && method != QStringLiteral( "delete" ) && method != QStringLiteral( "patch" ) )
    {
      QgsLogger::debug( QStringLiteral( "File `%1` contains unknown method `%2`" ).arg( mFileName, method ) );
      Q_ASSERT( 0 );
    }
  }

  QMap<QString, QString> fileNameChecksum;
  const QStringList fileNamesList = fileNames.values();

  for ( const QString &fileName : fileNamesList )
  {
    fileNameChecksum.insert( fileName, fileChecksums.value( fileName ) );
  }

  return fileNameChecksum;
}


void DeltaFileWrapper::addPatch( const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &oldFeature, const QgsFeature &newFeature )
{
  QJsonObject delta(
  {
    {"localPk", oldFeature.attribute( localPkAttrName ).toString()},
    {"localLayerId", localLayerId},
    {"method", "patch"},
    {"sourcePk", oldFeature.attribute( sourcePkAttrName ).toString()},
    {"sourceLayerId", sourceLayerId},
    {"uuid", QUuid::createUuid().toString( QUuid::WithoutBraces )},
  } );

  const QStringList attachmentFieldsList = attachmentFieldNames( mProject, localLayerId );
  const QgsGeometry oldGeom = oldFeature.geometry();
  const QgsGeometry newGeom = newFeature.geometry();
  const QgsAttributes oldAttrs = oldFeature.attributes();
  const QgsAttributes newAttrs = newFeature.attributes();
  QJsonObject oldData;
  QJsonObject newData;
  bool areFeaturesEqual = false;

  if ( ! oldGeom.equals( newGeom ) )
  {
    oldData.insert( QStringLiteral( "geometry" ), geometryToJsonValue( oldGeom ) );
    newData.insert( QStringLiteral( "geometry" ), geometryToJsonValue( newGeom ) );
    areFeaturesEqual = true;
  }

  // TODO types should be checked too, however QgsFields::operator== is checking instances, not values
  // TODO be careful with calculated fields here!!! Needs a fix!
  Q_ASSERT( oldFeature.fields().names() == newFeature.fields().names() );

  QgsFields fields = newFeature.fields();
  QJsonObject tmpOldAttrs;
  QJsonObject tmpNewAttrs;
  QJsonObject tmpOldFileChecksums;
  QJsonObject tmpNewFileChecksums;

  for ( int idx = 0; idx < fields.count(); ++idx )
  {
    const QVariant oldVal = oldAttrs.at( idx );
    const QVariant newVal = newAttrs.at( idx );

    if ( newVal != oldVal )
    {
      const QString name = fields.at( idx ).name();
      tmpOldAttrs.insert( name, oldVal.isNull() ? QJsonValue::Null : QJsonValue::fromVariant( oldVal ) );
      tmpNewAttrs.insert( name, newVal.isNull() ? QJsonValue::Null : QJsonValue::fromVariant( newVal ) );
      areFeaturesEqual = true;

      if ( attachmentFieldsList.contains( name ) )
      {
        const QString homeDir = mProject->homePath();
        const QString oldFileName = oldVal.toString();
        const QString newFileName = newVal.toString();

        // if the file name is an empty or null string, there is not much we can do
        if ( ! oldFileName.isEmpty() )
        {
          const QString oldFullFileName = QFileInfo( oldFileName ).isAbsolute() ? oldFileName : QStringLiteral( "%1/%2" ).arg( homeDir, oldFileName );
          const QByteArray oldFileChecksum = FileUtils::fileChecksum( oldFullFileName, QCryptographicHash::Sha256 );
          const QJsonValue oldFileChecksumJson = oldFileChecksum.isEmpty() ? QJsonValue::Null : QJsonValue( QString( oldFileChecksum.toHex() ) );
          tmpOldFileChecksums.insert( oldFullFileName, oldFileChecksumJson );
        }

        if ( ! newFileName.isEmpty() )
        {
          const QString newFullFileName = QFileInfo( newFileName ).isAbsolute() ? newFileName : QStringLiteral( "%1/%2" ).arg( homeDir, newFileName );
          const QByteArray newFileChecksum = FileUtils::fileChecksum( newFullFileName, QCryptographicHash::Sha256 );
          const QJsonValue newFileChecksumJson = newFileChecksum.isEmpty() ? QJsonValue::Null : QJsonValue( QString( newFileChecksum.toHex() ) );
          tmpNewFileChecksums.insert( newFullFileName, newFileChecksumJson );
        }
      }
    }
  }

  // if features are completely equal, there is no need to change the JSON
  if ( ! areFeaturesEqual )
    return;

  if ( tmpOldAttrs.length() > 0 || tmpNewAttrs.length() > 0 )
  {
    oldData.insert( QStringLiteral( "attributes" ), tmpOldAttrs );
    newData.insert( QStringLiteral( "attributes" ), tmpNewAttrs );

    if ( tmpOldFileChecksums.length() > 0 )
      oldData.insert( QStringLiteral( "files_sha256" ), tmpOldFileChecksums );

    if ( tmpNewFileChecksums.length() > 0 )
      newData.insert( QStringLiteral( "files_sha256" ), tmpNewFileChecksums );
  }
  else
  {
    Q_ASSERT( tmpOldFileChecksums.isEmpty() );
    Q_ASSERT( tmpNewFileChecksums.isEmpty() );
  }

  delta.insert( QStringLiteral( "old" ), oldData );
  delta.insert( QStringLiteral( "new" ), newData );

  mIsDirty = true;

  QMap<QString, int> layerPkDeltaIdx = mLocalPkDeltaIdx.value( localLayerId );
  QString localPk = delta.value( QStringLiteral( "localPk" ) ).toString();

  if ( layerPkDeltaIdx.contains( localPk ) )
  {
    int deltaIdx = layerPkDeltaIdx.take( localPk );
    QJsonObject deltaCreate = mDeltas.at( deltaIdx ).toObject();
    QJsonObject newCreate = deltaCreate.value( QStringLiteral( "new" ) ).toObject();
    QJsonObject attributesCreate = newCreate.value( QStringLiteral( "attributes" ) ).toObject();

    if ( ! newData.value( QStringLiteral( "geometry" ) ).isUndefined() )
    {
      newCreate.insert( QStringLiteral( "geometry" ), newData.value( QStringLiteral( "geometry" ) ) );
    }

    const QStringList attributeNames = tmpNewAttrs.keys();
    for ( const QString &attributeName : attributeNames )
    {
      attributesCreate.insert( attributeName, tmpNewAttrs.value( attributeName ) );
    }

    newCreate.insert( QStringLiteral( "attributes" ), geometryToJsonValue( newGeom ) );
    newCreate.insert( QStringLiteral( "attributes" ), attributesCreate );
    deltaCreate.insert( QStringLiteral( "new" ), newCreate );
    deltaCreate.insert( QStringLiteral( "sourcePk" ), delta.value( QStringLiteral( "sourcePk" ) ) );

    mDeltas.replace( deltaIdx, deltaCreate );

    return;
  }
  else
  {
    mDeltas.append( delta );

    emit countChanged();
  }
}


void DeltaFileWrapper::addDelete( const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &oldFeature )
{
  QJsonObject delta( {
    {"localPk", oldFeature.attribute( localPkAttrName ).toString()},
    {"localLayerId", localLayerId},
    {"method", "delete"},
    {"sourcePk", oldFeature.attribute( sourcePkAttrName ).toString()},
    {"sourceLayerId", sourceLayerId},
    {"uuid", QUuid::createUuid().toString( QUuid::WithoutBraces )},
  } );

  QMap<QString, int> layerPkDeltaIdx = mLocalPkDeltaIdx.value( localLayerId );
  QString localPk = delta.value( QStringLiteral( "localPk" ) ).toString();

  if ( layerPkDeltaIdx.contains( localPk ) )
  {
    mDeltas.removeAt( layerPkDeltaIdx.take( localPk ) );

    emit countChanged();

    return;
  }

  const QStringList attachmentFieldsList = attachmentFieldNames( mProject, localLayerId );
  const QgsAttributes oldAttrs = oldFeature.attributes();
  QJsonObject oldData( {{"geometry", geometryToJsonValue( oldFeature.geometry() )}} );
  QJsonObject tmpOldAttrs;
  QJsonObject tmpOldFileChecksums;

  for ( int idx = 0; idx < oldAttrs.count(); ++idx )
  {
    const QVariant oldVal = oldAttrs.at( idx );
    const QString name = oldFeature.fields().at( idx ).name();
    tmpOldAttrs.insert( name, oldVal.isNull() ? QJsonValue::Null : QJsonValue::fromVariant( oldVal ) );

    if ( attachmentFieldsList.contains( name ) && ! oldVal.isNull() )
    {
      const QString oldFileName = oldVal.toString();
      const QByteArray oldFileChecksum = FileUtils::fileChecksum( oldFileName, QCryptographicHash::Sha256 );
      const QJsonValue oldFileChecksumJson = oldFileChecksum.isEmpty() ? QJsonValue::Null : QJsonValue( QString( oldFileChecksum.toHex() ) );

      tmpOldFileChecksums.insert( oldVal.toString(), oldFileChecksumJson );
    }
  }

  if ( tmpOldAttrs.length() > 0 )
  {
    oldData.insert( QStringLiteral( "attributes" ), tmpOldAttrs );

    if ( tmpOldFileChecksums.length() > 0 )
    {
      oldData.insert( QStringLiteral( "files_sha256" ), tmpOldFileChecksums );
    }
  }
  else
  {
    Q_ASSERT( tmpOldFileChecksums.isEmpty() );
  }

  delta.insert( QStringLiteral( "old" ), oldData );

  mDeltas.append( delta );
  mIsDirty = true;

  emit countChanged();
}


void DeltaFileWrapper::addCreate(  const QString &localLayerId, const QString &sourceLayerId, const QString &localPkAttrName, const QString &sourcePkAttrName, const QgsFeature &newFeature )
{
  QJsonObject delta( {
    {"localPk", newFeature.attribute( localPkAttrName ).toString()},
    {"localLayerId", localLayerId},
    {"method", "create"},
    {"sourcePk", newFeature.attribute( sourcePkAttrName ).toString()},
    {"sourceLayerId", sourceLayerId},
    {"uuid", QUuid::createUuid().toString( QUuid::WithoutBraces )},
  } );
  const QStringList attachmentFieldsList = attachmentFieldNames( mProject, localLayerId );
  const QgsAttributes newAttrs = newFeature.attributes();
  QJsonObject newData( {{"geometry", geometryToJsonValue( newFeature.geometry() )}} );
  QJsonObject tmpNewAttrs;
  QJsonObject tmpNewFileChecksums;

  for ( int idx = 0; idx < newAttrs.count(); ++idx )
  {
    const QVariant newVal = newAttrs.at( idx );
    const QString name = newFeature.fields().at( idx ).name();
    tmpNewAttrs.insert( name, QJsonValue::fromVariant( newVal ) );

    if ( attachmentFieldsList.contains( name ) )
    {
      const QString newFileName = newVal.toString();
      const QByteArray newFileChecksum = FileUtils::fileChecksum( newFileName, QCryptographicHash::Sha256 );
      const QJsonValue newFileChecksumJson = newFileChecksum.isEmpty() ? QJsonValue::Null : QJsonValue( QString( newFileChecksum.toHex() ) );

      tmpNewFileChecksums.insert( newVal.toString(), newFileChecksumJson );
    }
  }

  if ( tmpNewAttrs.length() > 0 )
  {
    newData.insert( QStringLiteral( "attributes" ), tmpNewAttrs );

    if ( tmpNewFileChecksums.length() > 0 )
    {
      newData.insert( QStringLiteral( "files_sha256" ), tmpNewFileChecksums );
    }
  }
  else
  {
    Q_ASSERT( tmpNewFileChecksums.isEmpty() );
  }

  delta.insert( QStringLiteral( "new" ), newData );

  QString localPk = delta.value( QStringLiteral( "localPk" ) ).toString();
  mLocalPkDeltaIdx[localLayerId][localPk] = mDeltas.count();

  mDeltas.append( delta );
  mIsDirty = true;

  emit countChanged();
}


QJsonValue DeltaFileWrapper::geometryToJsonValue( const QgsGeometry &geom ) const
{
  if ( geom.isNull() )
    return QJsonValue::Null;

  return QJsonValue( geom.asWkt() );
}


QStringList DeltaFileWrapper::deltaLayerIds() const
{
  QStringList layerIds;

  for ( const QJsonValue &v : qgis::as_const( mDeltas ) )
  {
    QJsonObject deltaItem = v.toObject();
    const QString layerId = deltaItem.value( QStringLiteral( "layerId" ) ).toString();

    if ( ! layerIds.contains( layerId ) )
      layerIds.append( layerId );
  }

  return layerIds;
}


bool DeltaFileWrapper::isDeltaBeingApplied() const
{
  return mIsDeltaFileBeingApplied;
}


bool DeltaFileWrapper::apply()
{
  return applyInternal( false );
}


bool DeltaFileWrapper::applyReversed()
{
  return applyInternal( true );
}


bool DeltaFileWrapper::applyInternal( bool shouldApplyInReverse )
{
  if ( ! toFile() )
    return false;

  mIsDeltaFileBeingApplied = true;

  bool isSuccess = true;

  // 1) get all vector layers referenced in the delta file and make them editable
  QHash<QString, QgsVectorLayer *> vectorLayers;
  for ( const QJsonValue &deltaJson : qgis::as_const( mDeltas ) )
  {
    const QVariantMap delta = deltaJson.toObject().toVariantMap();
    const QString layerId = delta.value( QStringLiteral( "localLayerId" ) ).toString();

    QgsVectorLayer *vl = static_cast<QgsVectorLayer *>( mProject->mapLayer( layerId ) );

    if ( ! vl || ( ! vl->isEditable() && ! vl->startEditing() ) )
    {
      isSuccess = false;
      break;
    }

    vectorLayers.insert( vl->id(), vl );
  }

  // 2) actual application of the delta contents
  if ( isSuccess )
    isSuccess = applyDeltasOnLayers( vectorLayers, shouldApplyInReverse );

  // 3) commit the changes, if fails, revert the rest of the layers
  if ( isSuccess )
  {
    for ( auto [ layerId, vl ] : qfield::asKeyValueRange( vectorLayers ) )
    {
      // despite the error, try to rollback all the changes so far
      if ( vl->commitChanges() )
        vectorLayers[layerId] = nullptr;
      else
      {
        QgsMessageLog::logMessage( QStringLiteral( "Failed to commit layer with id \"%1\", all the rest layers will be rollbacked" ).arg( layerId ) );
        isSuccess = false;
        break;
      }
    }
  }

  // 4) revert the changes that didn't manage to be applied
  if ( ! isSuccess )
  {
    for ( auto [ layerId, vl ] : qfield::asKeyValueRange( vectorLayers ) )
    {
      // the layer has already been committed
      if ( ! vl )
        continue;

      // despite the error, try to rollback all the changes so far
      if ( ! vl->rollBack() )
        QgsMessageLog::logMessage( QStringLiteral( "Failed to rollback layer with id \"%1\"" ).arg( layerId ) );
    }
  }

  mIsDeltaFileBeingApplied = false;

  return isSuccess;
}


bool DeltaFileWrapper::applyDeltasOnLayers( QHash<QString, QgsVectorLayer *> &vectorLayers, bool shouldApplyInReverse )
{
  QJsonArray deltas;

  if ( shouldApplyInReverse )
    // not the most optimal solution, but at least the QJsonValues are not copied
    for ( int i = mDeltas.size(); i > 0; )
      deltas.append( mDeltas[--i] );
  else
    deltas = QJsonArray( mDeltas );

  for ( const QJsonValue &deltaJson : qgis::as_const( deltas ) )
  {
    const QVariantMap delta = deltaJson.toObject().toVariantMap();
    const QString layerId = delta.value( QStringLiteral( "localLayerId" ) ).toString();
    const QString localPk = delta.value( QStringLiteral( "localPk" ) ).toString();
    // temporary disable attachment checks, enable them when clear how to proceed
    //    const QStringList attachmentFieldNamesList = attachmentFieldNames( mProject, layerId );
    const QgsFields fields = vectorLayers[layerId]->fields();
    const QPair<int, QString> pkAttrPair = getLocalPkAttribute( vectorLayers[layerId] );

    QString method = delta.value( QStringLiteral( "method" ) ).toString();
    QVariantMap oldValues = delta.value( QStringLiteral( "old" ) ).toMap();
    QVariantMap newValues = delta.value( QStringLiteral( "new" ) ).toMap();
    QgsFeature f;

    if ( shouldApplyInReverse )
    {
      if ( method == QStringLiteral( "create" ) )
        method = QStringLiteral( "delete" );
      else if ( method == QStringLiteral( "delete" ) )
        method = QStringLiteral( "create" );

      std::swap( oldValues, newValues );
    }

    Q_ASSERT( vectorLayers[layerId] );

    if ( ! vectorLayers[layerId] )
      return false;

    if ( method != QStringLiteral( "create" ) )
    {
      QgsExpression expr( QStringLiteral( " %1 = %2 " ).arg( QgsExpression::quotedColumnRef( pkAttrPair.second ), QgsExpression::quotedString( localPk ) ) );
      QgsFeatureIterator it = vectorLayers[layerId]->getFeatures( QgsFeatureRequest( expr ) );

      if ( ! it.nextFeature( f ) )
        return false;

      QgsFeature tempFeature;
      if ( it.nextFeature( tempFeature ) )
        return false;

      if ( ! f.isValid() )
        return false;
    }

    if ( method == QStringLiteral( "create" ) )
    {
      Q_ASSERT( oldValues.isEmpty() );
      Q_ASSERT( ! newValues.isEmpty() );

      const QString geomWkt = newValues.value( QStringLiteral( "geometry" ) ).toString();
      const QVariantMap attributes = newValues.value( QStringLiteral( "attributes" ) ).toMap();

      QgsGeometry geom;
      QgsAttributeMap qgsAttributeMap;

      if ( ! geomWkt.isEmpty() )
        geom = QgsGeometry::fromWkt( geomWkt );

      for ( auto [ attrName, attrValue ] : qfield::asKeyValueRange( attributes ) )
        qgsAttributeMap.insert( fields.indexFromName( attrName ), attrValue );

      QgsFeature createdFeature = QgsVectorLayerUtils::createFeature( vectorLayers[layerId], geom, qgsAttributeMap );

      Q_ASSERT( createdFeature.isValid() );

      if ( ! vectorLayers[layerId]->addFeature( createdFeature ) )
        return false;
    }
    else if ( method == QStringLiteral( "delete" ) )
    {
      Q_ASSERT( newValues.isEmpty() );
      Q_ASSERT( ! oldValues.isEmpty() );
      Q_ASSERT( f.isValid() );

      if ( ! vectorLayers[layerId]->deleteFeature( f.id() ) )
        return false;
    }
    else if ( method == QStringLiteral( "patch" ) )
    {
      Q_ASSERT( ! newValues.isEmpty() );
      Q_ASSERT( ! oldValues.isEmpty() );
      Q_ASSERT( f.isValid() );

      const QString geomWkt = newValues.value( QStringLiteral( "geometry" ) ).toString();
      const QVariantMap attributes = newValues.value( QStringLiteral( "attributes" ) ).toMap();

      if ( ! geomWkt.isEmpty() )
      {
        QgsGeometry geom = QgsGeometry::fromWkt( geomWkt );
        vectorLayers[layerId]->changeGeometry( f.id(), geom );
      }

      for ( auto [ attrName, attrValue ] : qfield::asKeyValueRange( attributes ) )
      {
        if ( ! vectorLayers[layerId]->changeAttributeValue( f.id(), fields.indexOf( attrName ), attrValue ) )
          return false;
      }
    }
    else
    {
      Q_ASSERT( 0 );
    }
  }

  return true;
}


QPair<int, QString> DeltaFileWrapper::getLocalPkAttribute( const QgsVectorLayer *vl )
{
  const QgsFields fields = vl->fields();
  const QList<int> pkAttrs = vl->primaryKeyAttributes() << fields.indexFromName( QStringLiteral( "fid" ) );
  // we assume the first index to be the primary key index... kinda stupid, but memory layers don't have primary key at all, but we use it on geopackages, but... snap!
  const int pkAttrIdx = pkAttrs[0];

  if ( pkAttrIdx == -1 )
    return QPair<int, QString>( -1, QString() );

  const QString pkAttrName = fields.at( pkAttrIdx ).name();

  return QPair<int, QString>( pkAttrIdx, pkAttrName );
}


QPair<int, QString> DeltaFileWrapper::getSourcePkAttribute( const QgsVectorLayer *vl )
{
  QString pkAttrNamesAggr = vl->customProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ) ).toString();

  if ( pkAttrNamesAggr.isEmpty() )
    return getLocalPkAttribute( vl );

  QStringList pkAttrNames = pkAttrNamesAggr.split( QStringLiteral( "," ) );

  if ( pkAttrNames.size() > 1 )
    return QPair<int, QString>( -1, QString() );

  const QString pkAttrName = pkAttrNames[0];
  const QgsFields fields = vl->fields();
  const int pkAttrIdx = fields.indexFromName( pkAttrName );

  if ( pkAttrIdx == -1 )
    return QPair<int, QString>( -1, QString() );

  return QPair<int, QString>( pkAttrIdx, pkAttrName );
}

QString DeltaFileWrapper::getSourceLayerId( const QgsVectorLayer *vl )
{
  return vl ? vl->customProperty( QStringLiteral( "remoteLayerId" ) ).toString() : QString();
}
