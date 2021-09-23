/***************************************************************************
                            iosplatformutilities.h  -  utilities for qfield

                              -------------------
              begin                : November 2020
              copyright            : (C) 2020 by Denis Rouzaud
              email                : denis@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef IOSPLATFORMUTILITIES_H
#define IOSPLATFORMUTILITIES_H

#include "platformutilities.h"
#include "picturesource.h"
#include <QQuickItem>

class ImagePicker;

/*

class ImagePicker : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(float imageScale READ imageScale WRITE setImageScale NOTIFY imageScaleChanged)
    Q_PROPERTY(float imageQuality READ imageQuality WRITE setImageQuality NOTIFY imageQualityChanged)
    Q_PROPERTY(bool saveImageToCameraRoll READ saveImageToCameraRoll WRITE setSaveImageToCameraRoll NOTIFY saveImageToCameraRollChanged)
public:
    explicit ImagePicker(QQuickItem *parent = 0);
    Q_INVOKABLE void openPicker();
    Q_INVOKABLE void openCamera();
    QString imagePath() const
    {
        return m_imagePath;
    }

    float imageScale() const
    {
        return m_imageScale;
    }

    float imageQuality() const
    {
        return m_imageQuality;
    }

    bool saveImageToCameraRoll() const
    {
        return m_saveImageToCameraRoll;
    }

signals:
    void pictureReceived( const QString &path );
    void imagePathChanged(QString imagePath);
    void imageScaleChanged(float arg);
    void imageQualityChanged(float arg);
    void saveImageToCameraRollChanged(bool arg);

public slots:

    void setImagePath(QString arg)
    {
        if (m_imagePath != arg) {
            m_imagePath = arg;
            emit imagePathChanged(arg);
        }
    }

    void setImageScale(float arg)
    {
        if (m_imageScale != arg) {
            m_imageScale = arg;
            emit imageScaleChanged(arg);
        }
    }

    void setImageQuality(float arg)
    {
        if (m_imageQuality != arg) {
            m_imageQuality = arg;
            emit imageQualityChanged(arg);
        }
    }

    void setSaveImageToCameraRoll(bool arg)
    {
        if (m_saveImageToCameraRoll != arg) {
            m_saveImageToCameraRoll = arg;
            emit saveImageToCameraRollChanged(arg);
        }
    }

private:
    class ImagePickerDelegateContainer;
    ImagePickerDelegateContainer *m_delegate;
    QString m_imagePath;
    float m_imageScale;
    float m_imageQuality;
    bool m_saveImageToCameraRoll;
};

*/
class IosPlatformUtilities : public PlatformUtilities
{
  public:
    IosPlatformUtilities();
    PlatformUtilities::Capabilities capabilities() const override { return Capabilities() | NativeCamera | AdjustBrightness; }
    QString systemGenericDataLocation() const override;
    bool checkPositioningPermissions() const override;
    bool checkCameraPermissions() const override;
    virtual ImagePicker *getCameraPicture( QQuickItem* item, const QString &prefix, const QString &pictureFilePath, const QString &suffix ) override;
};

#endif
