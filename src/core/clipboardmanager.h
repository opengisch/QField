/***************************************************************************
  clipboardmanager.h - ClipboardManager

 ---------------------
 begin                : 27.5.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QClipboard>
#include <QMimeData>
#include <QObject>

class QgsFeature;
class QgsVectorLayer;

/**
 * This class handles clipboard operations.
 */
class ClipboardManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool hasFeature READ hasFeature NOTIFY hasFeatureChanged )

  public:
    explicit ClipboardManager( QObject *parent = nullptr );
    ~ClipboardManager() {}

    bool hasFeature() const { return mHasFeature; }

    /**
     * Copies a feature into the system's clipboard in both plain text as well as HTML.
     * \param feature the feature from which the attributes will be copied into memory
     * \param includeGeometry set to TRUE when a WKT string representation of the geometry
     * should be included
     */
    Q_INVOKABLE void copyFeatureToClipboard( const QgsFeature &feature, bool includeGeometry = false );

    /**
     * Returns a feature with attributes and geometry matching clipboard values when
     * a feature has been copied into the clipboard.
     */
    Q_INVOKABLE QgsFeature pasteFeatureFromClipboard( QgsVectorLayer *layer );

  signals:

    void hasFeatureChanged();

  private slots:

    void systemClipboardChanged();

  private:
    QClipboard *mClipboard;
    bool mHasFeature;
};

#endif // PROJECTINFO_H
