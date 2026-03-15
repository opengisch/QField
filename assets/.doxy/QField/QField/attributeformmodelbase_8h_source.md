

# File attributeformmodelbase.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**attributeformmodelbase.h**](attributeformmodelbase_8h.md)

[Go to the documentation of this file](attributeformmodelbase_8h.md)


```C++
/***************************************************************************
  attributeformmodelbase.h - AttributeFormModelBase

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
#ifndef ATTRIBUTEFORMMODELBASE_H
#define ATTRIBUTEFORMMODELBASE_H

#include "featuremodel.h"

#include <QStack>
#include <QStandardItemModel>
#include <qgsattributeeditorcontainer.h>
#include <qgseditformconfig.h>
#include <qgsexpressioncontext.h>

class AttributeFormModelBase : public QStandardItemModel
{
    Q_OBJECT

  public:
    explicit AttributeFormModelBase( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    FeatureModel *featureModel() const;
    void setFeatureModel( FeatureModel *featureModel );

    bool hasTabs() const;
    void setHasTabs( bool hasTabs );

    bool hasRemembrance() const;
    void setHasRemembrance( bool hasRemembrance );

    bool hasConstraints() const;
    void setHasConstraints( bool hasConstraints );

    bool save();

    bool create();

    bool deleteFeature();

    bool constraintsHardValid() const;

    bool constraintsSoftValid() const;

    QVariant attribute( const QString &name );

    bool changeAttribute( const QString &name, const QVariant &value );

    bool changeGeometry( const QgsGeometry &geometry );

    void applyFeatureModel();

    void applyParentDefaultValues();

    void applyRelationshipDefaultValues();

    void activateAllRememberValues();

    void deactivateAllRememberValues();

  signals:
    void featureModelChanged();
    void hasTabsChanged();
    void hasRemembranceChanged();
    void hasConstraintsChanged();
    void featureChanged();
    void constraintsHardValidChanged();
    void constraintsSoftValidChanged();

  private:
    struct CodeRequirements
    {
        QSet<QString> referencedColumns;
        bool formScope = false;
    };

    QgsAttributeEditorContainer *generateRootContainer() const;

    QgsAttributeEditorContainer *invisibleRootContainer() const;

    void updateAttributeValue( QStandardItem *item );

    void buildForm( QgsAttributeEditorContainer *container,
                    QStandardItem *parent,
                    const QString &parentVisibilityExpressions,
                    QList<QStandardItem *> &containers,
                    int currentTabIndex = 0,
                    int columnCount = 1 );

    void synchronizeFieldValue( int fieldIndex, QVariant value );

    void updateDefaultValues( int fieldIndex = -1, QVector<int> updatedFields = QVector<int>() );

    void updateEditorWidgetCodes( const QString &fieldName );

    void updateDataDefinedProperties( const QString &fieldName );

    bool codeRequiresUpdate( const QString &fieldName, const QString &code, const QRegularExpression &regEx );

    void updateVisibilityAndConstraints( int fieldIndex = -1 );

    void setConstraintsHardValid( bool constraintsHardValid );

    void setConstraintsSoftValid( bool constraintsSoftValid );

    QgsEditorWidgetSetup findBest( int fieldIndex );

    void resetModel();

    void onMapThemeCollectionChanged();

    FeatureModel *mFeatureModel = nullptr;
    QPointer<QgsVectorLayer> mLayer;
    std::unique_ptr<QgsAttributeEditorContainer> mTemporaryContainer;

    bool mHasTabs = false;
    bool mHasRemembrance = false;
    bool mHasConstraints = false;

    typedef QPair<QgsExpression, QStandardItem *> VisibilityExpression;
    QList<VisibilityExpression> mVisibilityExpressions;
    QMap<QStandardItem *, int> mFields;
    QMap<QStandardItem *, QString> mAliasExpressions;
    QMap<QStandardItem *, QString> mReadOnlyExpressions;
    QMap<QStandardItem *, QString> mEditorWidgetCodes;
    QMap<QString, CodeRequirements> mEditorWidgetCodesRequirements;

    QgsExpressionContext mExpressionContext;
    bool mConstraintsHardValid = true;
    bool mConstraintsSoftValid = true;
};

#endif // ATTRIBUTEFORMMODELBASE_H
```


