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

class QgsProcessingAlgorithm;
class QgsProcessingParameterDefinition;

/**
 * \brief A model for configuring processing algorithm parameters
 */
class ProcessingAlgorithmParametersModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QString algorithmId READ algorithmId WRITE setAlgorithmId NOTIFY algorithmIdChanged )
    Q_PROPERTY( bool isValid READ isValid NOTIFY algorithmIdChanged )

    Q_PROPERTY( QString algorithmDisplayName READ algorithmDisplayName NOTIFY algorithmIdChanged )
    Q_PROPERTY( QString algorithmShortDescription READ algorithmShortDescription NOTIFY algorithmIdChanged )

  public:
    //!Roles of the model.
    enum Role
    {
      ParameterTypeRole = Qt::UserRole, //! the parameter type
      ParameterDescriptionRole,         //! the parameter description
      ParameterDefaultValueRole,        //! the parameter default value
      ParameterValueRole,               //! the parameter value
    };
    Q_ENUM( Role )

    explicit ProcessingAlgorithmParametersModel( QObject *parent = nullptr );

    /**
     * Rebuilds the processing algorithm parameters model
     */
    Q_INVOKABLE void rebuild();

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
     * Returns the display name of the algorithm.
     */
    QString algorithmDisplayName() const;

    /**
     * Returns a short description of the algorithm.
     */
    QString algorithmShortDescription() const;

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

  signals:
    /**
     * Emitted when the algorithm ID has changed
     */
    void algorithmIdChanged();

  private:
    QString mAlgorithmId;
    const QgsProcessingAlgorithm *mAlgorithm = nullptr;
    QList<const QgsProcessingParameterDefinition *> mParameters;
    QList<QVariant> mValues;
};


#endif // PROCESSINGALGORITHMPARAMETERSMODEL
