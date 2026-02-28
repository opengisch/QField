

# File featurelistmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featurelistmodel.h**](featurelistmodel_8h.md)

[Go to the documentation of this file](featurelistmodel_8h.md)


```C++
/***************************************************************************
  featurelistmodel.h - FeatureListModel

 ---------------------
 begin                : 1.2.2017
 copyright            : (C) 2017 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FEATURELISTMODEL_H
#define FEATURELISTMODEL_H

#include "appexpressioncontextscopesgenerator.h"
#include "featureexpressionvaluesgatherer.h"

#include <QAbstractItemModel>
#include <QTimer>
#include <qgsfeature.h>
#include <qgsstringutils.h>

#include <stringutils.h>

class QgsVectorLayer;

class FeatureListModel : public QAbstractItemModel
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

    Q_PROPERTY( QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged )

    Q_PROPERTY( QgsFeature currentFormFeature READ currentFormFeature WRITE setCurrentFormFeature NOTIFY currentFormFeatureChanged )

    Q_PROPERTY( AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator READ appExpressionContextScopesGenerator WRITE setAppExpressionContextScopesGenerator NOTIFY appExpressionContextScopesGeneratorChanged )

  public:
    enum FeatureListRoles
    {
      KeyFieldRole = Qt::UserRole + 1,
      DisplayStringRole,
      GroupFieldRole,
      FeatureIdRole,
    };

    Q_ENUM( FeatureListRoles )

    explicit FeatureListModel( QObject *parent = nullptr );
    ~FeatureListModel();

    virtual QModelIndex index( int row, int column, const QModelIndex &parent ) const override;
    virtual QModelIndex parent( const QModelIndex &child ) const override;
    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    virtual int columnCount( const QModelIndex &parent ) const override;
    virtual QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE QVariant dataFromRowIndex( int row, int role ) { return data( index( row, 0, QModelIndex() ), role ); }

    Q_INVOKABLE QgsFeature getFeatureFromKeyValue( const QVariant &value ) const;

    Q_INVOKABLE QgsFeature getFeatureById( QgsFeatureId id ) const;

    virtual QHash<int, QByteArray> roleNames() const override;

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

    Q_INVOKABLE int findKey( const QVariant &key ) const;

    Q_INVOKABLE QList<int> findDisplayValueMatches( const QString &filter ) const;

    bool orderByValue() const;

    void setOrderByValue( bool orderByValue );

    bool addNull() const;

    void setAddNull( bool addNull );

    QString filterExpression() const;

    void setFilterExpression( const QString &filterExpression );

    QString searchTerm() const;

    void setSearchTerm( const QString &searchTerm );

    QgsFeature currentFormFeature() const;

    void setCurrentFormFeature( const QgsFeature &feature );

    AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    void setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator );

  signals:
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

  private slots:
    void onFeatureAdded();
    void onAttributeValueChanged( QgsFeatureId fid, int idx, const QVariant &value );
    void onFeatureDeleted();

    void gatherFeatureList();

    void processFeatureList();

  private:
    struct Entry
    {
        Entry( const QString &displayString, const QVariant &key, const QVariant &group, const QgsFeatureId &fid )
          : displayString( displayString )
          , key( key )
          , group( group )
          , fid( fid )
          , fuzzyScore( 0 )
        {}

        Entry() = default;

        QString displayString;
        QVariant key;
        QVariant group;
        QgsFeatureId fid;
        double fuzzyScore = 0.0;
    };
    Entry entryFromRow( int row );

    void reloadLayer();

    void cleanupGatherer();

    QPointer<QgsVectorLayer> mCurrentLayer;

    FeatureExpressionValuesGatherer *mGatherer = nullptr;

    QList<Entry> mEntries;
    QString mKeyField;
    QString mDisplayValueField;
    QString mGroupField;
    bool mDisplayGroupName = false;
    bool mOrderByValue = false;
    bool mAddNull = false;
    QString mFilterExpression;
    QString mSearchTerm;
    QgsFeature mCurrentFormFeature;
    QPointer<AppExpressionContextScopesGenerator> mAppExpressionContextScopesGenerator;

    QTimer mReloadTimer;

    friend class FeatureCheckListModel;
    friend class FeatureCheckListModelBase;
};

#endif // FEATURELISTMODEL_H
```


