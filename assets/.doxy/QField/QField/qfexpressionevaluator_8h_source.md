

# File qfexpressionevaluator.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfexpressionevaluator.h**](qfexpressionevaluator_8h.md)

[Go to the documentation of this file](qfexpressionevaluator_8h.md)


```C++
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
    enum Mode
    {
      ExpressionMode,         
      ExpressionTemplateMode, 
    };
    Q_ENUM( Mode )

    explicit QfExpressionEvaluator( QObject *parent = nullptr );

    Mode mode() const { return mMode; }

    void setMode( Mode mode );

    QString expressionText() { return mExpressionText; }

    void setExpressionText( const QString &expressionText );

    QgsFeature feature() const { return mFeature; }

    void setFeature( const QgsFeature &feature );

    QgsMapLayer *layer() const { return mLayer; }

    void setLayer( QgsMapLayer *layer );

    QgsProject *project() const { return mProject; }

    void setProject( QgsProject *project );

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QfAppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    void setAppExpressionContextScopesGenerator( QfAppExpressionContextScopesGenerator *generator );

    QfAttributeFormModel *attributeFormModel() const;

    void setAttributeFormModel( QfAttributeFormModel *attributeFormModel );

    QVariantMap variables() const { return mVariables; }

    void setVariables( const QVariantMap &variables );

    Q_INVOKABLE QVariant evaluate();

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
```


