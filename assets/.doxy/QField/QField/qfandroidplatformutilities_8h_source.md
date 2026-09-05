

# File qfandroidplatformutilities.h

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**qfandroidplatformutilities.h**](qfandroidplatformutilities_8h.md)

[Go to the documentation of this file](qfandroidplatformutilities_8h.md)


```C++
/***************************************************************************
                            qfandroidplatformutilities.h  -  utilities for qfield

                              -------------------
              begin                : February 2016
              copyright            : (C) 2016 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFANDROIDPLATFORMUTILITIES_H
#define QFANDROIDPLATFORMUTILITIES_H

#include "qfplatformutilities.h"

#include <QJniObject>

class QfAndroidPlatformUtilities : public QfPlatformUtilities
{
  public:
    QfAndroidPlatformUtilities();

    QfPlatformUtilities::Capabilities capabilities() const override;
    void afterUpdate() override;
    QString systemSharedDataLocation() const override;

    bool hasQgsProject() const override;
    void loadQgsProject() const override;

    bool hasQfAction() const override;
    void executeQfAction() const override;

    QStringList appDataDirs() const override;
    QString applicationDirectory() const override;
    QStringList additionalApplicationDirectories() const override;
    QStringList rootDirectories() const override;

    void importProjectFolder() const override;
    void importProjectArchive() const override;
    void importDatasets() const override;

    void updateProjectFromArchive( const QString &projectPath ) const override;

    void sendDatasetTo( const QString &path ) const override;
    void exportDatasetTo( const QString &path ) const override;
    void removeDataset( const QString &path ) const override;

    void exportFolderTo( const QString &path ) const override;
    void sendCompressedFolderTo( const QString &path ) const override;
    void removeFolder( const QString &path ) const override;

    QfResourceSource *getCameraPicture( const QString &prefix, const QString &pictureFilePath, const QString &suffix, QObject *parent = nullptr ) override;
    QfResourceSource *getCameraVideo( const QString &prefix, const QString &videoFilePath, const QString &suffix, QObject *parent = nullptr ) override;
    QfResourceSource *getGalleryPicture( const QString &prefix, const QString &pictureFilePath, QObject *parent = nullptr ) override;
    QfResourceSource *getGalleryVideo( const QString &prefix, const QString &videoFilePath, QObject *parent = nullptr ) override;
    QfResourceSource *getFile( const QString &prefix, const QString &filePath, const QString &mimeType, QObject *parent = nullptr ) override;

    QfViewStatus *open( const QString &filePath, bool isEditing, QObject *parent = nullptr ) override;

    void requestStoragePermission() const override;
    bool checkPositioningPermissions() const override;
    bool checkCameraPermissions() const override;
    bool checkMicrophonePermissions() const override;

    void setScreenLockPermission( const bool allowLock ) override;

    void dimBrightness() override;
    void restoreBrightness() override;

    void setHandleVolumeKeys( const bool handle ) override;

    double systemFontPointSize() const override { return 16.0; }

    void uploadPendingAttachments( QfCloudConnection *connection ) const override;

    bool isSystemDarkTheme() const override;

    void vibrate( int milliseconds ) const override;

    QString startPositioningService() const override;
    void stopPositioningService() const override;

    void requestBackgroundPositioningPermissions() override;

  private:
    // separate multiple permissions using a semi-column (;)
    bool checkAndAcquirePermissions( QStringList permissions, bool forceAsk = false ) const;
    QfResourceSource *processCameraActivity( const QString &prefix, const QString &filePath, const QString &suffix, bool isVideo, QObject *parent = nullptr );
    QfResourceSource *processGalleryActivity( const QString &prefix, const QString &filePath, const QString &mimeType, QObject *parent = nullptr );

    QString getIntentExtra( const QString &, QJniObject = nullptr ) const;

    QJniObject getNativeIntent() const;
    QJniObject getNativeExtras() const;
    QJniObject mActivity;
    QString mSystemGenericDataLocation;
};

#endif // QFANDROIDPLATFORMUTILITIES_H
```


