#ifndef FEATURECHECKLISTMODEL_H
#define FEATURECHECKLISTMODEL_H

#include <featurelistmodel.h>
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

  public:
    FeatureCheckListModel();

    enum FeatureListRoles
    {
      CheckedRole
    };

    virtual QVariant data( const QModelIndex &index, int role ) const override;
    virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QHash<int, QByteArray> roleNames() const override;

    /**
     * the attribute value (hstore or json list)
     */
    QVariant attributeValue() const;

    /**
     * the attribute value (hstore or json list)
     */
    void setAttributeValue( const QVariant &attributeValue );

    /**
     * the current attribute field
     */
    QgsField attributeField() const;

    /**
     * the current attribute field
     */
    void setAttributeField( QgsField field );

  signals:
    void attributeValueChanged();
    void attributeFieldChanged();
    void listUpdated();

  private:
    void setChecked( const QModelIndex &index );
    void setUnchecked( const QModelIndex &index );
    QVariant::Type fkType() const;

    QgsField mAttributeField;
    QStringList mCheckedEntries;
};

#endif // FEATURECHECKLISTMODEL_H
