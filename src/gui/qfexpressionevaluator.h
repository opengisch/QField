/***************************************************************************
 qfexpressionevaluator.h - QfExpressionEvaluator
                              -------------------
 begin                : January 2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFEXPRESSIONEVALUATOR_H
#define QFEXPRESSIONEVALUATOR_H

#include "qfappexpressioncontextscopesgenerator.h"
#include "qfattributeformmodel.h"
#include "qgsquickmapsettings.h"

#include <QObject>
#include <qgsexpression.h>
#include <qgsexpressioncontext.h>
#include <qgsmaplayer.h>
#include <qgsproject.h>


/**
 * @brief The QfExpressionEvaluator class enables evaluation of expression
 * strings and expression templates.
 * \ingroup core
 */
class QfExpressionEvaluator : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Mode mode READ mode WRITE setMode NOTIFY modeChanged )

    Q_PROPERTY( QString expressionText READ expressionText WRITE setExpressionText NOTIFY expressionTextChanged )

    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QgsMapLayer *layer READ layer WRITE setLayer NOTIFY layerChanged )
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QfAppExpressionContextScopesGenerator *appExpressionContextScopesGenerator READ appExpressionContextScopesGenerator WRITE setAppExpressionContextScopesGenerator NOTIFY appExpressionContextScopesGeneratorChanged )
    Q_PROPERTY( QfAttributeFormModel *attributeFormModel READ attributeFormModel WRITE setAttributeFormModel NOTIFY attributeFormModelChanged )
    Q_PROPERTY( QVariantMap variables READ variables WRITE setVariables NOTIFY variablesChanged )

  public:
    //! Expression evaluator modes
    enum Mode
    {
      ExpressionMode,         //!< Handle expression text as expression strings
      ExpressionTemplateMode, //!< Handle expression text as expression templates
    };
    Q_ENUM( Mode )

    explicit QfExpressionEvaluator( QObject *parent = nullptr );

    //! Returns the expression evaluator mode
    Mode mode() const { return mMode; }

    //! Sets the expression evaluator \a mode
    void setMode( Mode mode );

    //! Returns the expression text used when evaluating
    QString expressionText() { return mExpressionText; }

    //! Sets the expression text used when evaluating
    void setExpressionText( const QString &expressionText );

    //! Returns the feature attached to the expression context
    QgsFeature feature() const { return mFeature; }

    //! Sets the feature attached to the expression context
    void setFeature( const QgsFeature &feature );

    //! Returns the map layer attached to the expression context
    QgsMapLayer *layer() const { return mLayer; }

    //! Sets the map layer attached to the expression context
    void setLayer( QgsMapLayer *layer );

    //! Returns the project attached to the expression context
    QgsProject *project() const { return mProject; }

    //! Sets the project attached to the expression context
    void setProject( QgsProject *project );

    //! Returns the map settings attached to the expression context
    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    //! Sets the map settings attached to the expression context
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! Returns the application expression context scopes generator object
    QfAppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    //! Sets the application expression context scopes \a generator object
    void setAppExpressionContextScopesGenerator( QfAppExpressionContextScopesGenerator *generator );

    //! Returns the attribute form model providing an expression context
    QfAttributeFormModel *attributeFormModel() const;

    //! Sets the attribute form model providing an expression context
    void setAttributeFormModel( QfAttributeFormModel *attributeFormModel );

    //! Returns the map of variables injected into the expression context when evaluating
    QVariantMap variables() const { return mVariables; }

    //! Sets the map of variables injected into the expression context when evaluating
    void setVariables( const QVariantMap &variables );

    //! Evalutes and returns the current expression text property value
    Q_INVOKABLE QVariant evaluate();

    //! Evalutes and returns the expression text passed as a parameter
    Q_INVOKABLE QVariant evaluate( const QString &expressionText );

  signals:
    void modeChanged();
    void expressionTextChanged();
    void featureChanged();
    void layerChanged();
    void projectChanged();
    void mapSettingsChanged();
    void appExpressionContextScopesGeneratorChanged();
    void attributeFormModelChanged();
    void variablesChanged();

  private:
    Mode mMode = ExpressionMode;

    QString mExpressionText;

    QgsFeature mFeature;
    QgsMapLayer *mLayer = nullptr;
    QgsProject *mProject = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    QPointer<QfAppExpressionContextScopesGenerator> mAppExpressionContextScopesGenerator;
    QPointer<QfAttributeFormModel> mAttributeFormModel;
    QVariantMap mVariables;
};
#endif // QFEXPRESSIONEVALUATOR_H
