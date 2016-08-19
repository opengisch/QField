/***************************************************************************
  attributeformmodel.h - AttributeFormModel

 ---------------------
 begin                : 16.8.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ATTRIBUTEFORMMODEL_H
#define ATTRIBUTEFORMMODEL_H

#include <QAbstractItemModel>

#include <qgseditformconfig.h>

#include "featuremodel.h"

class AttributeFormModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY( FeatureModel* featureModel READ featureModel WRITE setFeatureModel NOTIFY featureModelChanged )

  public:
    enum FeatureRoles
    {
      ElementType = Qt::UserRole + 1,
      AttributeName,
      AttributeValue,
      AttributeEditable,
      EditorWidget,
      EditorWidgetConfig,
      RememberValue,
      Field
    };

    explicit AttributeFormModel( QObject* parent = nullptr );
    ~AttributeFormModel();

    // Basic functionality:
    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex& index ) const override;

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

    // Editable:
    bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;

    // Add data:
    bool insertRows( int row, int count, const QModelIndex& parent = QModelIndex() ) override;

    // Remove data:
    bool removeRows( int row, int count, const QModelIndex& parent = QModelIndex() ) override;

    FeatureModel* featureModel() const;
    void setFeatureModel( FeatureModel* featureModel );

  signals:
    void featureModelChanged();

  private slots:
    void onLayerChanged();
    void onFeatureChanged();

  private:
    template <typename T = QgsAttributeEditorElement*>
    T indexToElement( const QModelIndex& index ) const
    {
      return static_cast<T>( index.internalPointer() );
    }

    FeatureModel* mFeatureModel;
    QVector<bool> mRememberedAttributes;
    QgsVectorLayer* mLayer;
};

#endif // ATTRIBUTEFORMMODEL_H
