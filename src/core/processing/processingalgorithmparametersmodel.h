/***************************************************************************
  processingalgorithmparametersmodel.h - ProcessingAlgorithmParametersModel

 ---------------------
 begin                : 21.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PROCESSINGALGORITHMPARAMETERSMODEL
#define PROCESSINGALGORITHMPARAMETERSMODEL

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

class QgsProcessingAlgorithm;
class QgsProcessingParameterDefinition;

class ProcessingAlgorithmParametersModelBase;


/**
 * \brief A sort/filter proxy model for providers and algorithms available within QField
 * which automatically sorts the toolbox in a logical fashion and supports filtering
 * the results.
 */
class ProcessingAlgorithmParametersModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( ProcessingAlgorithmParametersModel::Filters filters READ filters WRITE setFilters NOTIFY filtersChanged )

    Q_PROPERTY( QString algorithmId READ algorithmId WRITE setAlgorithmId NOTIFY algorithmIdChanged )
    Q_PROPERTY( bool isValid READ isValid NOTIFY algorithmIdChanged )
    Q_PROPERTY( bool hasAdvancedParameters READ hasAdvancedParameters NOTIFY algorithmIdChanged )
    Q_PROPERTY( QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged )

    Q_PROPERTY( QString algorithmDisplayName READ algorithmDisplayName NOTIFY algorithmIdChanged )
    Q_PROPERTY( QString algorithmShortHelp READ algorithmDisplayName NOTIFY algorithmIdChanged )

  public:
    //! Available filter flags for filtering the model
    enum Filter
    {
      GeneralParameterFilter = 1 << 1,  //!< Show general (i.e. non-advanced) paremeters
      AdvancedParameterFilter = 1 << 2, //!< Show advanced paremeters
    };
    Q_DECLARE_FLAGS( Filters, Filter )
    Q_FLAGS( Filters )

    explicit ProcessingAlgorithmParametersModel( QObject *parent = nullptr );

    /**
       * Returns any filters that affect how toolbox content is filtered.
       * \see setFilters()
       */
    ProcessingAlgorithmParametersModel::Filters filters() const { return mFilters; }

    /**
       * Set \a filters that affect how toolbox content is filtered.
       * \see filters()
       */
    void setFilters( ProcessingAlgorithmParametersModel::Filters filters );

    /**
     * Returns the current algorithm ID from which parameters are taken from.
     */
    QString algorithmId() const;

    /**
     * Sets the current algorithm \a ID from which parameters are taken from.
     */
    void setAlgorithmId( const QString &id );

    /**
     * Returns whether the current model refers to a valid algorithm.
     */
    bool isValid() const;

    /**
     * Returns whether the current model has advanced parameters.
     */
    bool hasAdvancedParameters() const;

    /**
     * Returns the display name of the algorithm.
     */
    QString algorithmDisplayName() const;

    /**
     * Returns a short description of the algorithm.
     */
    QString algorithmShortHelp() const;

    /**
     * Returns a variant map of parameter names and values.
     */
    QVariantMap parameters();

    /**
     * Sets the values of the parameters model from variant map of parameter names and values.
     */
    void setParameters( const QVariantMap &parameters );

    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;

  signals:
    /**
     * Emitted when the active filters have changed.
     */
    void filtersChanged();

    /**
     * Emitted when the algorithm ID has changed.
     */
    void algorithmIdChanged( const QString &id );

    /**
     * Emitted when the parameters have changed.
     */
    void parametersChanged();

  private:
    ProcessingAlgorithmParametersModel::Filters mFilters;
    ProcessingAlgorithmParametersModelBase *mModel = nullptr;
};

/**
 * \brief A model for configuring processing algorithm parameters
 */
class ProcessingAlgorithmParametersModelBase : public QAbstractListModel
{
    Q_OBJECT

  public:
    //!Roles of the model.
    enum Role
    {
      ParameterTypeRole = Qt::UserRole, //! the parameter type
      ParameterDescriptionRole,         //! the parameter description
      ParameterFlagsRole,               //! the parameter description
      ParameterDefaultValueRole,        //! the parameter default value
      ParameterValueRole,               //! the parameter value
      ParameterConfigurationRole,       //! the parameter configuration
    };
    Q_ENUM( Role )

    explicit ProcessingAlgorithmParametersModelBase( QObject *parent = nullptr );

    /**
     * Returns the current algorithm ID from which parameters are taken from.
     */
    QString algorithmId() const { return mAlgorithmId; }

    /**
     * Sets the current algorithm \a ID from which parameters are taken from.
     */
    void setAlgorithmId( const QString &id );

    /**
     * Returns whether the current model refers to a valid algorithm.
     */
    bool isValid() const { return mAlgorithm; }

    /**
     * Returns whether the current model has advanced parameters.
     */
    bool hasAdvancedParameters() const { return mHasAdvancedParameters; }

    /**
     * Returns the display name of the algorithm.
     */
    QString algorithmDisplayName() const;

    /**
     * Returns a short description of the algorithm.
     */
    QString algorithmShortHelp() const;

    /**
     * Returns a variant map of parameter names and values.
     */
    QVariantMap parameters();

    /**
     * Sets the values of the parameters model from variant map of parameter names and values.
     */
    void setParameters( const QVariantMap &parameters );

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

  signals:
    /**
     * Emitted when the algorithm ID has changed.
     */
    void algorithmIdChanged( const QString &id );

    /**
     * Emitted when the parameters have changed.
     */
    void parametersChanged();

  private:
    /**
     * Rebuilds the processing algorithm parameters model.
     */
    void rebuild();

    QString mAlgorithmId;
    const QgsProcessingAlgorithm *mAlgorithm = nullptr;

    bool mHasAdvancedParameters = false;

    QList<const QgsProcessingParameterDefinition *> mParameters;
    QList<QVariant> mValues;
};


#endif // PROCESSINGALGORITHMPARAMETERSMODEL
