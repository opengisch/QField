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

/**
 * \ingroup core
 */
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

    /**
       * the attribute value. A QVariantList or an hstore formatted string, depending on the field type.
       */
    QVariant attributeValue() const;

    /**
       * the attribute value. A QVariantList or an hstore formatted string, depending on the field type.
       */
    void setAttributeValue( const QVariant &attributeValue );

    /**
       * the current attribute field
       */
    QgsField attributeField() const;

    /**
       * the current attribute field
       */
    void setAttributeField( const QgsField &field );

    /**
     * Whether multiple selected items are allowed
     */
    bool allowMulti() const;

    /**
     * Sets whether multiple selected items are allowed
     */
    void setAllowMulti( bool allowMulti );

    /**
      * Sets all element to either checked or unchecked state, depending on \a toggleChecked
      */
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

/**
 * A proxy model for filtering and sorting feature checklist items.
 *
 * This model enhances a source model (typically a FeatureCheckListModel) by allowing:
 * - Text-based filtering using a search term.
 * - Optional sorting that prioritizes checked items.
 * - Fuzzy and prefix-based matching for search refinement.
 *
 * Used in UI contexts where users interact with large lists of features and need
 * to quickly find and sort items by name or selection state.
 *
 * \ingroup core
 */
class FeatureCheckListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    //! The vector layer to list
    Q_PROPERTY( QgsVectorLayer *currentLayer READ currentLayer WRITE setCurrentLayer NOTIFY currentLayerChanged )

    //! The primary key field
    Q_PROPERTY( QString keyField READ keyField WRITE setKeyField NOTIFY keyFieldChanged )

    //! The display value field
    Q_PROPERTY( QString displayValueField READ displayValueField WRITE setDisplayValueField NOTIFY displayValueFieldChanged )

    //! The grouping key field
    Q_PROPERTY( QString groupField READ groupField WRITE setGroupField NOTIFY groupFieldChanged )

    //! Set to TRUE if the group name will be displayed in the list
    Q_PROPERTY( bool displayGroupName READ displayGroupName WRITE setDisplayGroupName NOTIFY displayGroupNameChanged )

    //! Set to TRUE if features should be ordered by value
    Q_PROPERTY( bool orderByValue READ orderByValue WRITE setOrderByValue NOTIFY orderByValueChanged )

    //! Set to TRUE if null values are allowed in the list
    Q_PROPERTY( bool addNull READ addNull WRITE setAddNull NOTIFY addNullChanged )

    //! Expression to filter features with. Empty string if no filter is applied.
    Q_PROPERTY( QString filterExpression READ filterExpression WRITE setFilterExpression NOTIFY filterExpressionChanged )

    //! The current form feature, used to evaluate expressions such as `current_value('attr1')`
    Q_PROPERTY( QgsFeature currentFormFeature READ currentFormFeature WRITE setCurrentFormFeature NOTIFY currentFormFeatureChanged )

    //! The application expression context scope generator used when filtering by expression
    Q_PROPERTY( AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator READ appExpressionContextScopesGenerator WRITE setAppExpressionContextScopesGenerator NOTIFY appExpressionContextScopesGeneratorChanged )

    //! The attribute value to generate checklist
    Q_PROPERTY( QVariant attributeValue READ attributeValue WRITE setAttributeValue NOTIFY attributeValueChanged )

    //! The attribute field to have information about type (JSON/HSTORE) etc.
    Q_PROPERTY( QgsField attributeField READ attributeField WRITE setAttributeField NOTIFY attributeFieldChanged )

    //! The attribute field to have information about type (JSON/HSTORE) etc.
    Q_PROPERTY( bool allowMulti READ allowMulti WRITE setAllowMulti NOTIFY allowMultiChanged )

    //! The current search term used to filter items.
    Q_PROPERTY( QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged )

    //! The sorting method.
    Q_PROPERTY( bool sortCheckedFirst READ sortCheckedFirst WRITE setSortCheckedFirst NOTIFY sortCheckedFirstChanged )

  public:
    explicit FeatureCheckListModel( QObject *parent = nullptr );

    Q_INVOKABLE QVariant dataFromRowIndex( int row, int role ) { return data( index( row, 0, QModelIndex() ), role ); }

    /**
    * Returns the first feature matching the key  \a value.
    */
    Q_INVOKABLE QgsFeature getFeatureFromKeyValue( const QVariant &value ) const;

    /**
    * Returns the first feature matching the feature  \a id.
    */
    Q_INVOKABLE QgsFeature getFeatureById( QgsFeatureId id ) const;

    /**
    * Get the row for a given key value.
    */
    Q_INVOKABLE int findKey( const QVariant &key ) const;

    /**
    * Get rows for a given filter string used to match display values.
    */
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

    /**
    * Orders all the values alphabethically by their displayString.
    */
    bool orderByValue() const;

    /**
     * Orders all the values alphabethically by their displayString.
     */
    void setOrderByValue( bool orderByValue );

    /**
     * Add a NULL value as the first entry.
     */
    bool addNull() const;

    /**
     * Add a NULL value as the first entry.
     */
    void setAddNull( bool addNull );

    /**
    * Expression to filter features with. Empty string if no filter is applied.
    */
    QString filterExpression() const;

    /**
    * Sets an expression to filter features with. Empty string if no filter is applied.
    */
    void setFilterExpression( const QString &filterExpression );

    /**
    * The current form feature, used to evaluate expressions such as `current_value('attr1')`
    */
    QgsFeature currentFormFeature() const;

    /**
    * Sets the current form feature, used to evaluate expressions such as `current_value('attr1')`
    */
    void setCurrentFormFeature( const QgsFeature &feature );

    /**
    * Returns the application expression context scope generator used when filtering by expression
    */
    AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    /**
    * Sets the application expression context scope generator used when filtering by expression
    */
    void setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator );

    /**
     * the attribute value. A QVariantList or an hstore formatted string, depending on the field type.
     */
    QVariant attributeValue() const;

    /**
     * the attribute value. A QVariantList or an hstore formatted string, depending on the field type.
     */
    void setAttributeValue( const QVariant &attributeValue );

    /**
     * the current attribute field
     */
    QgsField attributeField() const;

    /**
     * the current attribute field
     */
    void setAttributeField( const QgsField &field );

    /**
    * Whether multiple selected items are allowed
    */
    bool allowMulti() const;

    /**
    * Sets whether multiple selected items are allowed
    */
    void setAllowMulti( bool allowMulti );

    /**
    * Sets all element to either checked or unchecked state, depending on \a toggleChecked
    */
    Q_INVOKABLE void toggleCheckAll( bool toggleChecked = true );

    /**
    * Returns the current search term used to filter items.
    */
    QString searchTerm() const;

    /**
     * Sets the search term for filtering and updates the filter.
     */
    void setSearchTerm( const QString &searchTerm );

    /**
     * Returns whether checked items are sorted to the top.
     */
    bool sortCheckedFirst() const;

    /**
     * Enables or disables sorting checked items first.
     */
    void setSortCheckedFirst( bool enabled );

  protected:
    /**
     * Determines whether a row should be accepted based on the current filter settings.
     * Checks if the data in the row matches the current search term.
     */
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

    /**
     * Compares two items for sorting in the feature check list.
     *
     * Sorting priority:
     * 1. Null placeholder (if addNull() is enabled).
     * 2. Grouping by groupField() (empty groups first, then alphabetically).
     * 3. Checked-first (if mSortCheckedFirst is enabled and no search term).
     * 4. Search relevance (startsWith + fuzzy score if search term is active).
     * 5. Value-based alphabetical ordering (if orderByValue() is enabled).
     * 6. Key-based fallback (empty keys first, then alphabetically by key).
     *
     * @param left  The model index of the left item.
     * @param right The model index of the right item.
     * @return      True if the left item should appear before the right item.
     */
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
    void currentFormFeatureChanged();
    void appExpressionContextScopesGeneratorChanged();

    // FeatureCheckListModel signals
    void attributeValueChanged();
    void attributeFieldChanged();
    void allowMultiChanged();
    void listUpdated();

    // Proxy-specific signals
    void searchTermChanged();
    void sortCheckedFirstChanged();

  private:
    /**
     * Calculates a fuzzy matching score between a display string and a search term.
     */
    double calcFuzzyScore( const QString &displayString, const QString &searchTerm ) const;

    FeatureCheckListModelBase *mSourceModel = nullptr;
    QString mSearchTerm;
    bool mSortCheckedFirst;
};
#endif // FEATURECHECKLISTMODEL_H
