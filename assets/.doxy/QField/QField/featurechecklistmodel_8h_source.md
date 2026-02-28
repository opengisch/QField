

# File featurechecklistmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featurechecklistmodel.h**](featurechecklistmodel_8h.md)

[Go to the documentation of this file](featurechecklistmodel_8h.md)


```C++
/***************************************************************************
  featurechecklistmodel.h - FeatureCheckListModel

 ---------------------
 begin                : August 2019
 copyright            : (C) 2019 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FEATURECHECKLISTMODEL_H
#define FEATURECHECKLISTMODEL_H

#include "featurelistmodel.h"

#include <QSortFilterProxyModel>
#include <qgsvectorlayer.h>

class FeatureCheckListModelBase : public FeatureListModel
{
    Q_OBJECT

  public:
    explicit FeatureCheckListModelBase( QObject *parent = nullptr );

    enum FeatureListRoles
    {
      CheckedRole = Qt::UserRole + 100 // do not overlap with the roles of the base model
    };

    virtual QVariant data( const QModelIndex &index, int role ) const override;
    virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant attributeValue() const;

    void setAttributeValue( const QVariant &attributeValue );

    QgsField attributeField() const;

    void setAttributeField( const QgsField &field );

    bool allowMulti() const;

    void setAllowMulti( bool allowMulti );

    Q_INVOKABLE void toggleCheckAll( bool toggleChecked = true );

  signals:
    void attributeValueChanged();
    void attributeFieldChanged();
    void allowMultiChanged();
    void listUpdated();

  private:
    void setChecked( const QModelIndex &idx );
    void setUnchecked( const QModelIndex &idx );
    QMetaType::Type fkType() const;

    QgsField mAttributeField;
    QStringList mCheckedEntries;
    bool mAllowMulti = false;
};

class FeatureCheckListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QgsVectorLayer *currentLayer READ currentLayer WRITE setCurrentLayer NOTIFY currentLayerChanged )

    
    Q_PROPERTY( QString keyField READ keyField WRITE setKeyField NOTIFY keyFieldChanged )

    Q_PROPERTY( QString displayValueField READ displayValueField WRITE setDisplayValueField NOTIFY displayValueFieldChanged )

    Q_PROPERTY( QString groupField READ groupField WRITE setGroupField NOTIFY groupFieldChanged )

    Q_PROPERTY( bool displayGroupName READ displayGroupName WRITE setDisplayGroupName NOTIFY displayGroupNameChanged )

    Q_PROPERTY( bool orderByValue READ orderByValue WRITE setOrderByValue NOTIFY orderByValueChanged )

    Q_PROPERTY( bool addNull READ addNull WRITE setAddNull NOTIFY addNullChanged )

    Q_PROPERTY( QString filterExpression READ filterExpression WRITE setFilterExpression NOTIFY filterExpressionChanged )

    Q_PROPERTY( QgsFeature currentFormFeature READ currentFormFeature WRITE setCurrentFormFeature NOTIFY currentFormFeatureChanged )

    Q_PROPERTY( AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator READ appExpressionContextScopesGenerator WRITE setAppExpressionContextScopesGenerator NOTIFY appExpressionContextScopesGeneratorChanged )

    Q_PROPERTY( QVariant attributeValue READ attributeValue WRITE setAttributeValue NOTIFY attributeValueChanged )

    Q_PROPERTY( QgsField attributeField READ attributeField WRITE setAttributeField NOTIFY attributeFieldChanged )

    Q_PROPERTY( bool allowMulti READ allowMulti WRITE setAllowMulti NOTIFY allowMultiChanged )

    Q_PROPERTY( QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged )

    Q_PROPERTY( bool sortCheckedFirst READ sortCheckedFirst WRITE setSortCheckedFirst NOTIFY sortCheckedFirstChanged )

  public:
    explicit FeatureCheckListModel( QObject *parent = nullptr );

    Q_INVOKABLE QVariant dataFromRowIndex( int row, int role ) { return data( index( row, 0, QModelIndex() ), role ); }

    Q_INVOKABLE QgsFeature getFeatureFromKeyValue( const QVariant &value ) const;

    Q_INVOKABLE QgsFeature getFeatureById( QgsFeatureId id ) const;

    Q_INVOKABLE int findKey( const QVariant &key ) const;

    Q_INVOKABLE QList<int> findDisplayValueMatches( const QString &filter ) const;

    QgsVectorLayer *currentLayer() const;
    void setCurrentLayer( QgsVectorLayer *currentLayer );

    QString keyField() const;
    void setKeyField( const QString &keyField );

    QString displayValueField() const;
    void setDisplayValueField( const QString &displayValueField );

    QString groupField() const;
    void setGroupField( const QString &groupField );

    bool displayGroupName() const;
    void setDisplayGroupName( bool displayGroupName );

    bool orderByValue() const;

    void setOrderByValue( bool orderByValue );

    bool addNull() const;

    void setAddNull( bool addNull );

    QString filterExpression() const;

    void setFilterExpression( const QString &filterExpression );

    QgsFeature currentFormFeature() const;

    void setCurrentFormFeature( const QgsFeature &feature );

    AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    void setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator );

    QVariant attributeValue() const;

    void setAttributeValue( const QVariant &attributeValue );

    QgsField attributeField() const;

    void setAttributeField( const QgsField &field );

    bool allowMulti() const;

    void setAllowMulti( bool allowMulti );

    Q_INVOKABLE void toggleCheckAll( bool toggleChecked = true );

    QString searchTerm() const;

    void setSearchTerm( const QString &searchTerm );

    bool sortCheckedFirst() const;

    void setSortCheckedFirst( bool enabled );

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;


  signals:
    // FeatureListModel signals
    void currentLayerChanged();
    void keyFieldChanged();
    void displayValueFieldChanged();
    void groupFieldChanged();
    void displayGroupNameChanged();
    void orderByValueChanged();
    void addNullChanged();
    void filterExpressionChanged();
    void searchTermChanged();
    void currentFormFeatureChanged();
    void appExpressionContextScopesGeneratorChanged();

    // FeatureCheckListModel signals
    void attributeValueChanged();
    void attributeFieldChanged();
    void allowMultiChanged();
    void listUpdated();

    // Proxy-specific signals
    void sortCheckedFirstChanged();

  private:
    FeatureCheckListModelBase *mSourceModel = nullptr;
    bool mSortCheckedFirst = false;
};
#endif // FEATURECHECKLISTMODEL_H
```


