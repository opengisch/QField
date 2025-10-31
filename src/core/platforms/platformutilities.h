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
      NoCapabilities = 0,                 //!< No capabilities
      NativeCamera = 1,                   //!< Native camera handling support
      AdjustBrightness = 1 << 1,          //!< Screen brightness adjustment support
      SentryFramework = 1 << 2,           //!< Sentry framework support
      NativeLocalDataPicker = 1 << 3,     //!< Native local data picker support
      CustomImport = 1 << 4,              //!< Import project and dataset support
      CustomExport = 1 << 5,              //!< Export project and dataset support
      CustomSend = 1 << 6,                //!< Send/share files support
      FilePicker = 1 << 7,                //!< File picker support
      VolumeKeys = 1 << 8,                //!< Volume keys handling support
      Vibrate = 1 << 9,                   //!< Haptic feedback / vibration support
      UpdateProjectFromArchive = 1 << 10, //!< Update local project from a ZIP archive support
      PositioningService = 1 << 11,       //!< Positioning service support
    };
    Q_DECLARE_FLAGS( Capabilities, Capability )
    Q_FLAGS( Capabilities )

    enum FileType
    {
      AllFiles = 0, //!< All file types
      AudioFiles,   //!< Audio file types
    };
    Q_ENUM( FileType )

    virtual ~PlatformUtilities();

    /**
     * Returns flags containing the supported capabilities of the platform.
     */
    virtual PlatformUtilities::Capabilities capabilities() const;

    /**
     * This method will do initialization tasks and copy sample projects to a writable location
     * It will also call afterUpdate whenever a new version is detected.
     */
    void initSystem();

    /**
     * The path to share data location.
     * Under this path, there should be the app specific directories qgis/ proj/ qfield/ ...
     * Refers to /share or /usr/share on Linux.
     * This path is assumed to be read only.
     */
    virtual QString systemSharedDataLocation() const;

    /**
     * \returns a directory where local data can be stored.
     *          this includes local qfieldcloud data or sample projects.
     *          A \a subDir is appended to the path.
     */
    virtual QString systemLocalDataLocation( const QString &subDir = QString() ) const;

    /**
     * Returns TRUE is a project file has been provided and should be opened at launch.
     * \see loadQgsProject
     */
    virtual bool hasQgsProject() const;

    /**
     * Loads the project file provided during launch.
     * \see hasQgsProject
     */
    virtual void loadQgsProject() const;

    /**
     * Returns the current action string
     * \see loadQfAction
     */
    virtual bool hasQfAction() const;

    /**
     * Loads the action provided during launch.
     * \see hasQfAction
     */
    virtual void executeQfAction() const;

    /**
     * \returns a list of data directories where user data is searched.
     *          User data are pg_service.conf, authentication config, grids, ...
     */
    Q_INVOKABLE virtual QStringList appDataDirs() const;
    Q_INVOKABLE QStringList availableGrids() const;

    // TODO: move these functions to fileutils. Make sure to adjust any qml code relying on this.
    Q_INVOKABLE bool createDir( const QString &path, const QString &dirname ) const;
    Q_INVOKABLE bool rmFile( const QString &filename ) const;
    Q_INVOKABLE bool renameFile( const QString &oldFilePath, const QString &newFilePath, bool overwrite = true ) const;


    /**
     * The main application directory within which projects and datasets can be imported.
     */
    Q_INVOKABLE virtual QString applicationDirectory() const;

    /**
     * Secondary application directories which can be used by individual platforms.
     */
    virtual QStringList additionalApplicationDirectories() const;

    /**
     * A list of root directories (e.g. root of an external storage) when system-wide access is available.
     */
    virtual QStringList rootDirectories() const;

    //! Requests and imports a project folder into QField's application directory action
    Q_INVOKABLE virtual void importProjectFolder() const;
    //! Requests and imports a project archive into QField's application directory action
    Q_INVOKABLE virtual void importProjectArchive() const;
    //! Requests and imports one or more datasets into QField's application directory action
    Q_INVOKABLE virtual void importDatasets() const;

    /**
     * Update a local project content from a user-picked archive file action
     * \param projectPath the project file path
     */
    Q_INVOKABLE virtual void updateProjectFromArchive( const QString &projectPath ) const;

    //! Exports a folder \a path to a user-specified location
    Q_INVOKABLE virtual void exportFolderTo( const QString &path ) const;
    //! Exports a dataset \a path to a user-specified location
    Q_INVOKABLE virtual void exportDatasetTo( const QString &path ) const;

    //! Sends a dataset \a path via the platform native API
    Q_INVOKABLE virtual void sendDatasetTo( const QString &path ) const;
    //! Compresses a folder \a path and sends it via the platform native API
    Q_INVOKABLE virtual void sendCompressedFolderTo( const QString &path ) const;

    //! Removes a given dataset \a path
    Q_INVOKABLE virtual void removeDataset( const QString &path ) const;
    ;

    //! Removes a given folder \a path
    Q_INVOKABLE virtual void removeFolder( const QString &path ) const;

    /**
     * Get a picture from camera and copy it to the requested prefix
     * @param prefix The project folder
     * @param pictureFilePath The path (including subfolders and name) of the file
     * @return The name of the picture or null
     */
    Q_INVOKABLE virtual ResourceSource *getCameraPicture( const QString &prefix, const QString &pictureFilePath, const QString &suffix, QObject *parent = nullptr );

    /**
     * Get a video from camera and copy it to the requested prefix
     * @param prefix The project folder
     * @param videoFilePath The path (including subfolders and name) of the file
     * @return The name of the video or null
     */
    Q_INVOKABLE virtual ResourceSource *getCameraVideo( const QString &prefix, const QString &videoFilePath, const QString &suffix, QObject *parent = nullptr );

    /**
     * Get a picture from gallery and copy it to the requested prefix
     * @param prefix The project folder
     * @param pictureFilePath The path (including subfolders and name) of the file
     * @return The name of the picture or null
     */
    Q_INVOKABLE virtual ResourceSource *getGalleryPicture( const QString &prefix, const QString &pictureFilePath, QObject *parent = nullptr );

    /**
     * Get a video from gallery and copy it to the requested prefix
     * @param prefix The project folder
     * @param videoFilePath The path (including subfolders and name) of the video
     * @return The name of the video or null
     */
    Q_INVOKABLE virtual ResourceSource *getGalleryVideo( const QString &prefix, const QString &videoFilePath, QObject *parent = nullptr );

    /**
     * Get a file from the operating system and copy it to the requested prefix
     * @param prefix The project folder
     * @param filePath The path (including subfolders and name) of the file
     * @return The name of the file or null
     */
    Q_INVOKABLE virtual ResourceSource *getFile( const QString &prefix, const QString &filePath, FileType fileType = AllFiles, QObject *parent = nullptr );

    /**
     * Open the resource (file, image, ...) that is available under \a uri.
     * The mimetype is detected to indicate the system how the file should
     * be opened. An optional \a editing parameter can be set to true to indicate
     * to supported systems the resource is expected to be edited.
     */
    Q_INVOKABLE virtual ViewStatus *open( const QString &uri, bool editing = false, QObject *parent = nullptr );

    /**
     * Indicates the system that we want to open a project.
     * The system shall show a suitable user interface element (like a filebrowser)
     * to let the user select a project.
     * The call returns immediately and the returned ProjectSource will notify
     * when the project has actually been chosen.
     */
    Q_INVOKABLE virtual ProjectSource *openProject( QObject *parent = nullptr );

    /**
     * Checks for positioning (GPS etc) permissions on the device.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     * It will return true, if at least coarse permissions are granted. It will
     * ask for fine permissions if none are granted.
     * \deprecated Since QField 3.1
     */
    Q_DECL_DEPRECATED Q_INVOKABLE virtual bool checkPositioningPermissions() const;

    /**
     * Checks for camera permissions on the device.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     * \deprecated Since QField 3.1
     */
    Q_DECL_DEPRECATED Q_INVOKABLE virtual bool checkCameraPermissions() const;

    /**
     * Checks for camera microphone on the device.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     * \deprecated Since QField 3.1
     */
    Q_DECL_DEPRECATED Q_INVOKABLE virtual bool checkMicrophonePermissions() const;

    /**
     * Sets whether the device screen is allowed to go in lock mode.
     * @param allowLock if set to FALSE, the screen will not be allowed to lock.
     */
    Q_INVOKABLE virtual void setScreenLockPermission( const bool allowLock ) { Q_UNUSED( allowLock ); }

    /**
     * Dims the brightness of the screen on supported devices.
     */
    Q_INVOKABLE virtual void dimBrightness() { return; };

    /**
     * Restores the brightness of the screen to its original value on supported devices.
     */
    Q_INVOKABLE virtual void restoreBrightness() { return; };

    /**
     * Sets whether the device volume keys are handled by QField.
     */
    Q_INVOKABLE virtual void setHandleVolumeKeys( const bool handle ) { Q_UNUSED( handle ); }

    /**
     * Copies a text \a string to the system clipboard.
     */
    Q_INVOKABLE virtual void copyTextToClipboard( const QString &string ) const;

    /**
     * Returns the current text from the system clipboard.
     */
    Q_INVOKABLE virtual QString getTextFromClipboard() const;

    /**
     * Returns the default system font size.
     */
    Q_INVOKABLE virtual double systemFontPointSize() const;

    /**
     * Uploads any pending attachments linked to the logged in user account.
     */
    Q_INVOKABLE virtual void uploadPendingAttachments( QFieldCloudConnection *connection ) const;

    /**
     * Returns TRUE is the system uses a dark theme.
     */
    Q_INVOKABLE virtual bool isSystemDarkTheme() const;

    /**
     * Vibrates the device on supported platforms.
     */
    Q_INVOKABLE virtual void vibrate( int milliseconds ) const { Q_UNUSED( milliseconds ) }

    Q_INVOKABLE virtual void minimize( QQuickWindow *window ) const;

    /**
     * Starts a positioning service on supported platforms and returns the remote object URL.
     */
    virtual QString startPositioningService() const { return QString(); }

    /**
     * Stops a positioning service on supported platforms.
     */
    virtual void stopPositioningService() const {}

    Q_INVOKABLE virtual void requestStoragePermission() const {};
    virtual Qt::PermissionStatus checkCameraPermission() const;
    virtual void requestCameraPermission( std::function<void( Qt::PermissionStatus )> func );
    virtual Qt::PermissionStatus checkMicrophonePermission() const;
    virtual void requestMicrophonePermission( std::function<void( Qt::PermissionStatus )> func );
    virtual void requestBackgroundPositioningPermissions() {};

    static PlatformUtilities *instance();

  signals:
    //! Emitted when a resource has been received.
    void resourceReceived( const QString &path );

    //! Emitted when a resource has been opened.
    void resourceOpened( const QString &path );

    //! Emitted when a resource has been opened.
    void resourceCanceled( const QString &message );

  private:
    /**
     * This method can be used to implement platform specific initialization tasks to be performed after an app update.
     * This is implemented for Android to extract app assets to location where it can
     * be accessed via filesystem.
     */
    virtual void afterUpdate();

    void copySampleProjects();

    ResourceSource *createResource( const QString &prefix, const QString &filePath, const QString &fileName, QObject *parent );
};
#endif // PLATFORMUTILITIES_H
