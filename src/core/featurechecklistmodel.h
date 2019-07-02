#ifndef FEATURECHECKLISTMODEL_H
#define FEATURECHECKLISTMODEL_H

#include <featurelistmodel.h>

class FeatureCheckListModel : public FeatureListModel
{
    Q_OBJECT

    /**
     * The attribute value
     */
    Q_PROPERTY( QString attributeValue READ attributeValue WRITE setAttributeValue NOTIFY attributeValueChanged )

  public:
    FeatureCheckListModel();

    enum FeatureListRoles
    {
      CheckedRole
    };

    /*
    virtual QModelIndex index( int row, int column, const QModelIndex &parent ) const override;
    virtual QModelIndex parent( const QModelIndex &child ) const override;
    virtual int rowCount( const QModelIndex &parent ) const override;
    virtual int columnCount( const QModelIndex &parent ) const override;
    */
    virtual QVariant data( const QModelIndex &index, int role ) const override;
    virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QHash<int, QByteArray> roleNames() const override;

    /**
     * the attribute value (hstore or json list)
     */
    QString attributeValue() const;

    /**
     * the attribute value (hstore or json list)
     */
    void setAttributeValue( const QString &attributeValue );

  signals:
    void attributeValueChanged();

  private:
    void setChecked( const QModelIndex &index );
    void setUnchecked( const QModelIndex &index );

    QString mAttributeValue;
    QStringList mCheckedEntries;
};

#endif // FEATURECHECKLISTMODEL_H
