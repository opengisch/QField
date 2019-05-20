/***************************************************************************
  attributeformmodel.h - attributeformmodel

 ---------------------
 begin                : 22.9.2016
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

#include <QSortFilterProxyModel>

#include "attributeformmodelbase.h"

class FeatureModel;

class AttributeFormModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( FeatureModel *featureModel READ featureModel WRITE setFeatureModel NOTIFY featureModelChanged )
    Q_PROPERTY( bool hasTabs READ hasTabs WRITE setHasTabs NOTIFY hasTabsChanged )
    Q_PROPERTY( bool constraintsValid READ constraintsValid NOTIFY constraintsValidChanged )

  public:
    enum FeatureRoles
    {
      ElementType = Qt::UserRole + 1,
      Name,
      AttributeValue,
      AttributeEditable,
      EditorWidget,
      EditorWidgetConfig,
      RememberValue,
      Field,
      RelationId,
      NmRelationId,
      FieldIndex,
      Group,
      AttributeEditorElement,
      CurrentlyVisible,
      ConstraintValid,
      ConstraintDescription
    };

    Q_ENUM( FeatureRoles )

    AttributeFormModel( QObject *parent = nullptr );

    bool hasTabs() const;
    void setHasTabs( bool hasTabs );

    FeatureModel *featureModel() const;
    void setFeatureModel( FeatureModel *featureModel );

    bool constraintsValid() const;
    /**
     * Save the current (already existing) feature
     */
    Q_INVOKABLE void save();
    /**
     * Create the current (not existing yet) feature
     */
    Q_INVOKABLE void create();
    /**
     * Delete the current feature
     */
    Q_INVOKABLE void deleteFeature();
    /**
     * Get the attribute of the current feature by name
     * \param name the name of the attribute
     * \return value of the attribute
     */
    Q_INVOKABLE QVariant attribute( const QString &name );

  signals:
    void featureModelChanged();
    void hasTabsChanged();
    void featureChanged();
    void constraintsValidChanged();

  protected:
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    AttributeFormModelBase *mSourceModel;
};

#endif // ATTRIBUTEFORMMODEL_H
