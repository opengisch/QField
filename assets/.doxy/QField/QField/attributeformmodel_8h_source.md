

# File attributeformmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**attributeformmodel.h**](attributeformmodel_8h.md)

[Go to the documentation of this file](attributeformmodel_8h.md)


```C++
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

#include "attributeformmodelbase.h"

#include <QSortFilterProxyModel>

class FeatureModel;

class AttributeFormModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( FeatureModel *featureModel READ featureModel WRITE setFeatureModel NOTIFY featureModelChanged )
    Q_PROPERTY( bool hasTabs READ hasTabs NOTIFY hasTabsChanged )
    Q_PROPERTY( bool hasRemembrance READ hasRemembrance NOTIFY hasRemembranceChanged )
    Q_PROPERTY( bool hasConstraints READ hasConstraints NOTIFY hasConstraintsChanged )
    Q_PROPERTY( bool constraintsHardValid READ constraintsHardValid NOTIFY constraintsHardValidChanged )
    Q_PROPERTY( bool constraintsSoftValid READ constraintsSoftValid NOTIFY constraintsSoftValidChanged )

  public:
    enum FeatureRoles
    {
      ElementType = Qt::UserRole + 1,
      Name,
      AttributeValue,
      AttributeEditable,
      EditorWidget,
      EditorWidgetConfig,
      RelationEditorWidget,
      RelationEditorWidgetConfig,
      CanRememberValue,
      RememberValue,
      Field,
      RelationId,
      NmRelationId,
      FieldIndex,
      AttributeEditorElement,
      CurrentlyVisible,
      ConstraintHardValid,
      ConstraintSoftValid,
      ConstraintDescription,
      AttributeAllowEdit,
      EditorWidgetCode, //<! Returns a QML or HTML code string used by the relevant widgets
      TabIndex,
      GroupColor,
      GroupName,
      GroupIndex,
      ColumnCount,
      LabelOverrideColor,
      LabelColor,
      LabelOverrideFont,
      LabelFont,
    };

    Q_ENUM( FeatureRoles )

    explicit AttributeFormModel( QObject *parent = nullptr );

    bool hasTabs() const;
    bool hasRemembrance() const;
    bool hasConstraints() const;

    FeatureModel *featureModel() const;
    void setFeatureModel( FeatureModel *featureModel );

    bool constraintsHardValid() const;
    bool constraintsSoftValid() const;

    Q_INVOKABLE bool save();

    Q_INVOKABLE bool create();

    Q_INVOKABLE bool deleteFeature();

    Q_INVOKABLE QVariant attribute( const QString &name );

    Q_INVOKABLE bool changeAttribute( const QString &name, const QVariant &value );

    Q_INVOKABLE bool changeGeometry( const QgsGeometry &geometry );

    Q_INVOKABLE void applyFeatureModel();

    Q_INVOKABLE void applyParentDefaultValues();

    Q_INVOKABLE void applyRelationshipDefaultValues();

    Q_INVOKABLE void activateAllRememberValues();

    Q_INVOKABLE void deactivateAllRememberValues();

  signals:
    void featureModelChanged();
    void hasTabsChanged();
    void hasRemembranceChanged();
    void hasConstraintsChanged();
    void featureChanged();
    void constraintsHardValidChanged();
    void constraintsSoftValidChanged();

  protected:
    virtual bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  private:
    AttributeFormModelBase *mSourceModel = nullptr;
};

#endif // ATTRIBUTEFORMMODEL_H
```


