/***************************************************************************
  locatormodelsuperbridge.h

 ---------------------
 begin                : 01.12.2018
 copyright            : (C) 2018 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LOCATORMODELSUPERBRIDGE_H
#define LOCATORMODELSUPERBRIDGE_H

#include <QStandardItemModel>
#include <qgslocatormodelbridge.h>

class QgsQuickMapSettings;
class FeatureListExtentController;

/**
 * LocatorActionsModel is a model used to dislay
 * additional actions from the result of a filter.
 * These are displayed as contextual menu in QGIS
 * while they are shown in a row of buttons in QField.
 */
class LocatorActionsModel : public QStandardItemModel
{
    Q_OBJECT
  public:
    enum ActionRoles
    {
      IdRole = Qt::UserRole + 1,
      IconPathRole
    };
    explicit LocatorActionsModel( QObject *parent = nullptr );
    LocatorActionsModel( int rows, int columns, QObject *parent = nullptr );
    QHash<int, QByteArray> roleNames() const override;
};


/**
 * LocatorModelSuperBridge reimplements QgsLocatorModelBridge
 *  for specific needs of QField / QML implementation.
 */
class LocatorModelSuperBridge : public QgsLocatorModelBridge
{
    Q_OBJECT
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QObject *locatorHighlightGeometry READ locatorHighlightGeometry WRITE setLocatorHighlightGeometry NOTIFY locatorHighlightGeometryChanged )
    Q_PROPERTY( FeatureListExtentController *featureListController READ featureListController WRITE setFeatureListController NOTIFY featureListControllerChanged )

  public:
    explicit LocatorModelSuperBridge( QObject *parent = nullptr );
    ~LocatorModelSuperBridge() = default;

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QObject *locatorHighlightGeometry() const;
    void setLocatorHighlightGeometry( QObject *locatorHighlightGeometry );

    FeatureListExtentController *featureListController() const;
    void setFeatureListController( FeatureListExtentController *featureListController );

    Q_INVOKABLE LocatorActionsModel *contextMenuActionsModel( const int row );

    void emitMessage( const QString &text );

  signals:
    void mapSettingsChanged();
    void locatorHighlightGeometryChanged();
    void featureListControllerChanged();
    void messageEmitted( const QString &text );

  public slots:
    Q_INVOKABLE void triggerResultAtRow( const int row, const int id = -1 );

  private:
    QgsQuickMapSettings *mMapSettings = nullptr;
    QObject *mLocatorHighlightGeometry = nullptr;
    FeatureListExtentController *mFeatureListController = nullptr;
};

#endif // LOCATORMODELSUPERBRIDGE_H
