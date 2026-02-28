

# File platformutilities.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**platforms**](dir_4ed093858358730befc336a35ce794a4.md) **>** [**platformutilities.h**](platformutilities_8h.md)

[Go to the documentation of this file](platformutilities_8h.md)


```C++
/***************************************************************************
                            platformutilities.h  -  utilities for qfield

                              -------------------
              begin                : Wed Dec 04 10:48:28 CET 2015
              copyright            : (C) 2015 by Marco Bernasocchi
              email                : marco@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PLATFORMUTILITIES_H
#define PLATFORMUTILITIES_H

#include "qfield_core_export.h"
#include "viewstatus.h"

#include <QObject>
#include <QPermission>
#include <qgsfield.h>


class QFieldCloudConnection;
class ProjectSource;
class ResourceSource;

class QQuickItem;
class QQuickWindow;

class QFIELD_CORE_EXPORT PlatformUtilities : public QObject
{
    Q_OBJECT

    Q_PROPERTY( PlatformUtilities::Capabilities capabilities READ capabilities CONSTANT )

  public:
    enum Capability
    {
      NoCapabilities = 0,                 
      NativeCamera = 1,                   
      AdjustBrightness = 1 << 1,          
      SentryFramework = 1 << 2,           
      NativeLocalDataPicker = 1 << 3,     
      CustomImport = 1 << 4,              
      CustomExport = 1 << 5,              
      CustomSend = 1 << 6,                
      FilePicker = 1 << 7,                
      VolumeKeys = 1 << 8,                
      Vibrate = 1 << 9,                   
      UpdateProjectFromArchive = 1 << 10, 
      PositioningService = 1 << 11,       
    };
    Q_DECLARE_FLAGS( Capabilities, Capability )
    Q_FLAGS( Capabilities )

    enum FileType
    {
      AllFiles = 0, 
      AudioFiles,   
    };
    Q_ENUM( FileType )

    virtual ~PlatformUtilities();

    virtual PlatformUtilities::Capabilities capabilities() const;

    void initSystem();

    virtual QString systemSharedDataLocation() const;

    virtual QString systemLocalDataLocation( const QString &subDir = QString() ) const;

    virtual bool hasQgsProject() const;

    virtual void loadQgsProject() const;

    virtual bool hasQfAction() const;

    virtual void executeQfAction() const;

    Q_INVOKABLE virtual QStringList appDataDirs() const;
    Q_INVOKABLE QStringList availableGrids() const;

    // TODO: move these functions to fileutils. Make sure to adjust any qml code relying on this.
    Q_INVOKABLE bool createDir( const QString &path, const QString &dirname ) const;
    Q_INVOKABLE bool rmFile( const QString &filename ) const;
    Q_INVOKABLE bool renameFile( const QString &oldFilePath, const QString &newFilePath, bool overwrite = true ) const;


    Q_INVOKABLE virtual QString applicationDirectory() const;

    virtual QStringList additionalApplicationDirectories() const;

    virtual QStringList rootDirectories() const;

    Q_INVOKABLE virtual void importProjectFolder() const;
    Q_INVOKABLE virtual void importProjectArchive() const;
    Q_INVOKABLE virtual void importDatasets() const;

    Q_INVOKABLE virtual void updateProjectFromArchive( const QString &projectPath ) const;

    Q_INVOKABLE virtual void exportFolderTo( const QString &path ) const;
    Q_INVOKABLE virtual void exportDatasetTo( const QString &path ) const;

    Q_INVOKABLE virtual void sendDatasetTo( const QString &path ) const;
    Q_INVOKABLE virtual void sendCompressedFolderTo( const QString &path ) const;

    Q_INVOKABLE virtual void removeDataset( const QString &path ) const;
    ;

    Q_INVOKABLE virtual void removeFolder( const QString &path ) const;

    Q_INVOKABLE virtual ResourceSource *getCameraPicture( const QString &prefix, const QString &pictureFilePath, const QString &suffix, QObject *parent = nullptr );

    Q_INVOKABLE virtual ResourceSource *getCameraVideo( const QString &prefix, const QString &videoFilePath, const QString &suffix, QObject *parent = nullptr );

    Q_INVOKABLE virtual ResourceSource *getGalleryPicture( const QString &prefix, const QString &pictureFilePath, QObject *parent = nullptr );

    Q_INVOKABLE virtual ResourceSource *getGalleryVideo( const QString &prefix, const QString &videoFilePath, QObject *parent = nullptr );

    Q_INVOKABLE virtual ResourceSource *getFile( const QString &prefix, const QString &filePath, FileType fileType = AllFiles, QObject *parent = nullptr );

    Q_INVOKABLE virtual ViewStatus *open( const QString &uri, bool editing = false, QObject *parent = nullptr );

    Q_INVOKABLE virtual ProjectSource *openProject( QObject *parent = nullptr );

    Q_DECL_DEPRECATED Q_INVOKABLE virtual bool checkPositioningPermissions() const;

    Q_DECL_DEPRECATED Q_INVOKABLE virtual bool checkCameraPermissions() const;

    Q_DECL_DEPRECATED Q_INVOKABLE virtual bool checkMicrophonePermissions() const;

    Q_INVOKABLE virtual void setScreenLockPermission( const bool allowLock ) { Q_UNUSED( allowLock ); }

    Q_INVOKABLE virtual void dimBrightness() { return; };

    Q_INVOKABLE virtual void restoreBrightness() { return; };

    Q_INVOKABLE virtual void setHandleVolumeKeys( const bool handle ) { Q_UNUSED( handle ); }

    Q_INVOKABLE virtual void copyTextToClipboard( const QString &string ) const;

    Q_INVOKABLE virtual QString getTextFromClipboard() const;

    Q_INVOKABLE virtual double systemFontPointSize() const;

    Q_INVOKABLE virtual void uploadPendingAttachments( QFieldCloudConnection *connection ) const;

    Q_INVOKABLE virtual bool isSystemDarkTheme() const;

    Q_INVOKABLE virtual void vibrate( int milliseconds ) const { Q_UNUSED( milliseconds ) }

    virtual QString startPositioningService() const { return QString(); }

    virtual void stopPositioningService() const {}

    Q_INVOKABLE virtual void requestStoragePermission() const {};
    virtual Qt::PermissionStatus checkCameraPermission() const;
    virtual void requestCameraPermission( std::function<void( Qt::PermissionStatus )> func );
    virtual Qt::PermissionStatus checkMicrophonePermission() const;
    virtual void requestMicrophonePermission( std::function<void( Qt::PermissionStatus )> func );
    virtual void requestBackgroundPositioningPermissions() {};

    static PlatformUtilities *instance();

  signals:
    void resourceReceived( const QString &path );

    void resourceOpened( const QString &path );

    void resourceCanceled( const QString &message );

  private:
    virtual void afterUpdate();

    void copySampleProjects();

    ResourceSource *createResource( const QString &prefix, const QString &filePath, const QString &fileName, QObject *parent );
};
#endif // PLATFORMUTILITIES_H
```


