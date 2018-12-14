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

#include "locatorhighlight.h"
#include "multifeaturelistmodel.h"

class QgsQuickMapSettings;

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
    Q_PROPERTY( LocatorHighlight *locatorHighlight READ locatorHighlight WRITE setLocatorHighlight NOTIFY locatorHighlightChanged )
    Q_PROPERTY( MultiFeatureListModel *model READ model WRITE setModel NOTIFY modelChanged )

  public:
    explicit LocatorModelSuperBridge( QObject *parent = nullptr );
    ~LocatorModelSuperBridge() = default;

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    LocatorHighlight *locatorHighlight() const;
    void setLocatorHighlight( LocatorHighlight *locatorHighlight );

    MultiFeatureListModel *model() const;
    void setModel( MultiFeatureListModel *model );

    Q_INVOKABLE LocatorActionsModel *contextMenuActionsModel( const int row );

    void emitMessage( const QString &text );

  signals:
    void mapSettingsChanged();
    void locatorHighlightChanged();
    void modelChanged();
    void messageEmitted( const QString &text );

  public slots:
    Q_INVOKABLE void triggerResultAtRow( const int row, const int id = -1 );

  private:
    QgsQuickMapSettings *mMapSettings = nullptr;
    LocatorHighlight *mLocatorHighlight = nullptr;
    MultiFeatureListModel *mModel = nullptr;
};

#endif // LOCATORMODELSUPERBRIDGE_H
