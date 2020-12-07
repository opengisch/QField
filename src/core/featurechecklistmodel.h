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

#include <qgsvectorlayer.h>

class FeatureCheckListModel : public FeatureListModel
{
    Q_OBJECT

    /**
       * The attribute value to generate checklist
       */
    Q_PROPERTY( QVariant attributeValue READ attributeValue WRITE setAttributeValue NOTIFY attributeValueChanged )

    /**
       * The attribute field to have information about type (JSON/HSTORE) etc.
       */
    Q_PROPERTY( QgsField attributeField READ attributeField WRITE setAttributeField NOTIFY attributeFieldChanged )

    /**
     * The attribute field to have information about type (JSON/HSTORE) etc.
     */
    Q_PROPERTY( bool allowMulti READ allowMulti WRITE setAllowMulti NOTIFY allowMultiChanged )

  public:
    explicit FeatureCheckListModel( QObject *parent = nullptr );

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
    void setChecked( const QModelIndex &index );
    void setUnchecked( const QModelIndex &index );
    QVariant::Type fkType() const;

    QgsField mAttributeField;
    QStringList mCheckedEntries;
    bool mAllowMulti = false;
};

#endif // FEATURECHECKLISTMODEL_H
