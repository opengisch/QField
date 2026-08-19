/***************************************************************************
  qfdeltachangesmodel.h - QfDeltaChangesModel

 ---------------------
 begin                : 15.8.2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFDELTACHANGESMODEL_H
#define QFDELTACHANGESMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>
#include <QPointer>
#include <qgis.h>

class QfDeltaFileWrapper;
class QJsonObject;
class QgsVectorLayer;

/**
 * \brief A single attribute modified by a delta, with the values on both sides of the change.
 * \ingroup core
 */
class QfDeltaAttributeChange
{
    Q_GADGET

    Q_PROPERTY( QString name MEMBER name )
    Q_PROPERTY( QVariant oldValue MEMBER oldValue )
    Q_PROPERTY( QVariant newValue MEMBER newValue )

  public:
    explicit QfDeltaAttributeChange( const QString &name = QString(), const QVariant &oldValue = QVariant(), const QVariant &newValue = QVariant() )
      : name( name )
      , oldValue( oldValue )
      , newValue( newValue )
    {}

    QString name;
    QVariant oldValue;
    QVariant newValue;
};

Q_DECLARE_METATYPE( QfDeltaAttributeChange )


/**
 * A list model of the deltas not uploaded to QFieldCloud yet, one row per
 * changed feature, sorted so that the rows of a layer are adjacent and a view
 * can section them by LayerId.
 *
 * \ingroup core
 */
class QfDeltaChangesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QfDeltaFileWrapper *deltaFileWrapper READ deltaFileWrapper WRITE setDeltaFileWrapper NOTIFY deltaFileWrapperChanged )

  public:
    enum ColumnRole
    {
      LayerIdRole = Qt::UserRole + 1,
      MethodRole,
      FeatureNameRole,
      AttributeChangesRole,
      HasGeometryChangeRole,
    };
    Q_ENUM( ColumnRole )

    enum DeltaMethod
    {
      UnknownMethod,
      CreateMethod,
      PatchMethod,
      DeleteMethod,
    };
    Q_ENUM( DeltaMethod )

    explicit QfDeltaChangesModel( QObject *parent = nullptr );

    //! Returns the attached delta file wrapper, if any.
    QfDeltaFileWrapper *deltaFileWrapper() const;

    //! Attaches the \a deltaFileWrapper the rows are read from, pass NULLPTR to detach and clear the model.
    void setDeltaFileWrapper( QfDeltaFileWrapper *deltaFileWrapper );

    //! Returns number of rows.
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    //! Returns the data at given \a index with given \a role.
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Returns the model role names.
    QHash<int, QByteArray> roleNames() const override;

    //! Returns the name of the layer with the given \a layerId.
    Q_INVOKABLE QString layerName( const QString &layerId ) const;

    //! Returns the geometry type of the layer with the given \a layerId.
    Q_INVOKABLE Qgis::GeometryType layerGeometryType( const QString &layerId ) const;

    //! Returns how many pending changes the layer with the given \a layerId carries.
    Q_INVOKABLE int layerChangesCount( const QString &layerId ) const;

  signals:
    void deltaFileWrapperChanged();

  private:
    struct DeltaChange
    {
        QString layerId;
        DeltaMethod method = UnknownMethod;
        QString featureName;
        QList<QfDeltaAttributeChange> attributeChanges;
        bool hasGeometryChange = false;
    };

    struct LayerSummary
    {
        QString name;
        Qgis::GeometryType geometryType = Qgis::GeometryType::Unknown;
        QJsonArray deltas;
        QList<DeltaChange> changes;
    };

    //! Returns the method \a delta applies.
    static DeltaMethod deltaMethod( const QJsonObject &delta );

    //! Rebuilds the rows from the attached delta file wrapper, if any.
    void refresh();

    //! Returns one row per delta of the \a deltas of the \a layer with the given \a layerId.
    QList<DeltaChange> layerChanges( const QString &layerId, const QJsonArray &deltas, QgsVectorLayer *layer ) const;

    /**
     * Returns the names to show for the features the \a deltas of the \a layer apply to, keyed by local primary key.
     *
     * Features still present in the layer are named after their display expression and fetched in a single
     * request, the ones already gone are described by the values their delta kept of them.
     */
    QHash<QString, QString> featureDisplayNames( QgsVectorLayer *layer, const QJsonArray &deltas ) const;

    //! Returns the attributes the \a delta changed, honouring the semantics of \a method.
    QList<QfDeltaAttributeChange> attributeChanges( const QJsonObject &delta, DeltaMethod method ) const;

    QPointer<QfDeltaFileWrapper> mDeltaFileWrapper;
    QList<DeltaChange> mDeltaChanges;
    QHash<QString, LayerSummary> mLayerSummaries;
};

#endif // QFDELTACHANGESMODEL_H
