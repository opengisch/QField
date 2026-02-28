

# File expressionevaluator.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**expressionevaluator.h**](expressionevaluator_8h.md)

[Go to the documentation of this file](expressionevaluator_8h.md)


```C++
/***************************************************************************
 expressionevaluator.h - ExpressionEvaluator
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

#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H

#include "appexpressioncontextscopesgenerator.h"
#include "qgsquickmapsettings.h"

#include <QObject>
#include <qgsexpression.h>
#include <qgsexpressioncontext.h>
#include <qgsmaplayer.h>
#include <qgsproject.h>


class ExpressionEvaluator : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Mode mode READ mode WRITE setMode NOTIFY modeChanged )

    Q_PROPERTY( QString expressionText READ expressionText WRITE setExpressionText NOTIFY expressionTextChanged )

    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QgsMapLayer *layer READ layer WRITE setLayer NOTIFY layerChanged )
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator READ appExpressionContextScopesGenerator WRITE setAppExpressionContextScopesGenerator NOTIFY appExpressionContextScopesGeneratorChanged )
    Q_PROPERTY( QVariantMap variables READ variables WRITE setVariables NOTIFY variablesChanged )

  public:
    enum Mode
    {
      ExpressionMode,         
      ExpressionTemplateMode, 
    };
    Q_ENUM( Mode )

    explicit ExpressionEvaluator( QObject *parent = nullptr );

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

    AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    void setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator );

    QVariantMap variables() const { return mVariables; }

    void setVariables( const QVariantMap &variables );

    Q_INVOKABLE QVariant evaluate();

  signals:
    void modeChanged();
    void expressionTextChanged();
    void featureChanged();
    void layerChanged();
    void projectChanged();
    void mapSettingsChanged();
    void appExpressionContextScopesGeneratorChanged();
    void variablesChanged();

  private:
    Mode mMode = ExpressionMode;

    QString mExpressionText;

    QgsFeature mFeature;
    QgsMapLayer *mLayer = nullptr;
    QgsProject *mProject = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    QPointer<AppExpressionContextScopesGenerator> mAppExpressionContextScopesGenerator;
    QVariantMap mVariables;
};
#endif // EXPRESSIONEVALUATOR_H
```


