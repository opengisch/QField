

# File qfattributeformmodelbase.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfattributeformmodelbase.cpp**](qfattributeformmodelbase_8cpp.md)

[Go to the documentation of this file](qfattributeformmodelbase_8cpp.md)


```C++
/***************************************************************************
  qfattributeformmodelbase.cpp - QfAttributeFormModelBase

 ---------------------
 begin                : 16.8.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfattributeformmodel.h"
#include "qfattributeformmodelbase.h"

#include <QDirIterator>
#include <QRegularExpression>
#include <qgsattributeeditorelement.h>
#include <qgsattributeeditorfield.h>
#include <qgsattributeeditorhtmlelement.h>
#include <qgsattributeeditorqmlelement.h>
#include <qgsattributeeditorrelation.h>
#include <qgsattributeeditorspacerelement.h>
#include <qgsattributeeditortextelement.h>
#include <qgsdatetimefieldformatter.h>
#include <qgseditorwidgetsetup.h>
#include <qgsexpressioncontextutils.h>
#include <qgsmapthemecollection.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvaluerelationfieldformatter.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerutils.h>

Q_GLOBAL_STATIC( QStringList, sSupportedEditorWidgets );

QfAttributeFormModelBase::QfAttributeFormModelBase( QObject *parent )
  : QStandardItemModel( 0, 1, parent )
{
  connect( QgsProject::instance(), &QgsProject::mapThemeCollectionChanged, this, &QfAttributeFormModelBase::onMapThemeCollectionChanged );
  if ( QgsProject::instance()->mapThemeCollection() )
  {
    onMapThemeCollectionChanged();
  }

  if ( sSupportedEditorWidgets->isEmpty() )
  {
    // Our QML files carry a QfEditorWidget prefix, the widget names QGIS hands us don't
    const QString prefix = QStringLiteral( "QfEditorWidget" );
    QDirIterator it( QStringLiteral( ":/qt/qml/org/qfield/gui/editorwidgets" ) );
    while ( it.hasNext() )
    {
      it.next();
      const QFileInfo fileInfo = it.fileInfo();
      if ( !fileInfo.isFile() )
      {
        continue;
      }

      const QString baseName = fileInfo.baseName();
      if ( baseName.startsWith( prefix ) && baseName != prefix + QStringLiteral( "Base" ) )
      {
        sSupportedEditorWidgets->append( baseName.mid( prefix.length() ) );
      }
    }
    sSupportedEditorWidgets->append( QStringLiteral( "RelationEditor" ) );
    sSupportedEditorWidgets->append( QStringLiteral( "Hidden" ) );
  }
}

void QfAttributeFormModelBase::onMapThemeCollectionChanged()
{
  connect( QgsProject::instance()->mapThemeCollection(), &QgsMapThemeCollection::mapThemeChanged, this, [this] { resetModel(); applyFeatureModel(); } );
}

QHash<int, QByteArray> QfAttributeFormModelBase::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[QfAttributeFormModel::ElementType] = "Type";
  roles[QfAttributeFormModel::Name] = "Name";
  roles[QfAttributeFormModel::AttributeValue] = "AttributeValue";
  roles[QfAttributeFormModel::AttributeEditable] = "AttributeEditable";
  roles[QfAttributeFormModel::EditorWidget] = "EditorWidget";
  roles[QfAttributeFormModel::EditorWidgetConfig] = "EditorWidgetConfig";
  roles[QfAttributeFormModel::RelationEditorWidget] = "RelationEditorWidget";
  roles[QfAttributeFormModel::RelationEditorWidgetConfig] = "RelationEditorWidgetConfig";
  roles[QfAttributeFormModel::CanRememberValue] = "CanRememberValue";
  roles[QfAttributeFormModel::RememberValue] = "RememberValue";
  roles[QfAttributeFormModel::Field] = "Field";
  roles[QfAttributeFormModel::RelationId] = "RelationId";
  roles[QfAttributeFormModel::NmRelationId] = "NmRelationId";
  roles[QfAttributeFormModel::ConstraintHardValid] = "ConstraintHardValid";
  roles[QfAttributeFormModel::ConstraintSoftValid] = "ConstraintSoftValid";
  roles[QfAttributeFormModel::ConstraintDescription] = "ConstraintDescription";
  roles[QfAttributeFormModel::AttributeAllowEdit] = "AttributeAllowEdit";
  roles[QfAttributeFormModel::EditorWidgetCode] = "EditorWidgetCode";
  roles[QfAttributeFormModel::TabIndex] = "TabIndex";
  roles[QfAttributeFormModel::GroupColor] = "GroupColor";
  roles[QfAttributeFormModel::GroupName] = "GroupName";
  roles[QfAttributeFormModel::GroupIndex] = "GroupIndex";
  roles[QfAttributeFormModel::ColumnCount] = "ColumnCount";
  roles[QfAttributeFormModel::LabelOverrideColor] = "LabelOverrideColor";
  roles[QfAttributeFormModel::LabelColor] = "LabelColor";
  roles[QfAttributeFormModel::LabelOverrideFont] = "LabelOverrideFont";
  roles[QfAttributeFormModel::LabelFont] = "LabelFont";

  return roles;
}

bool QfAttributeFormModelBase::setData( const QModelIndex &index, const QVariant &value, int role )
{
  QStandardItem *item = itemFromIndex( index );
  if ( !item )
    return false;

  if ( !qgsVariantEqual( data( index, role ), value ) )
  {
    switch ( role )
    {
      case QfAttributeFormModel::AttributeAllowEdit:
      {
        const int fieldIndex = item->data( QfAttributeFormModel::FieldIndex ).toInt();
        mFeatureModel->setData( mFeatureModel->index( fieldIndex ), value, QfFeatureModel::AttributeAllowEdit );
        item->setData( value, QfAttributeFormModel::AttributeAllowEdit );
        updateVisibilityAndConstraints( fieldIndex );
        break;
      }

      case QfAttributeFormModel::RememberValue:
      {
        const int fieldIndex = item->data( QfAttributeFormModel::FieldIndex ).toInt();
        mFeatureModel->setData( mFeatureModel->index( fieldIndex ), value, QfFeatureModel::RememberAttribute );
        item->setData( value, QfAttributeFormModel::RememberValue );
        break;
      }

      case QfAttributeFormModel::AttributeValue:
      {
        const int fieldIndex = item->data( QfAttributeFormModel::FieldIndex ).toInt();
        const bool changed = mFeatureModel->setData( mFeatureModel->index( fieldIndex ), value, QfFeatureModel::AttributeValue );
        if ( changed )
        {
          mExpressionContext.popScope();
          mExpressionContext << QgsExpressionContextUtils::formScope( mFeatureModel->feature() );
          synchronizeFieldValue( fieldIndex, value );
        }
        updateDefaultValues( fieldIndex );
        updateVisibilityAndConstraints( fieldIndex );
        return changed;
      }
    }
  }
  return false;
}

QfFeatureModel *QfAttributeFormModelBase::featureModel() const
{
  return mFeatureModel;
}

void QfAttributeFormModelBase::setFeatureModel( QfFeatureModel *featureModel )
{
  if ( mFeatureModel == featureModel )
    return;

  if ( mFeatureModel )
  {
    disconnect( mFeatureModel, &QfFeatureModel::currentLayerChanged, this, &QfAttributeFormModelBase::onCurrentLayerChanged );
    disconnect( mFeatureModel, &QfFeatureModel::modelReset, this, &QfAttributeFormModelBase::applyFeatureModel );
    disconnect( mFeatureModel, &QfFeatureModel::featureUpdated, this, &QfAttributeFormModelBase::applyFeatureModel );
    disconnect( mFeatureModel, &QfFeatureModel::linkedParentFeatureChanged, this, &QfAttributeFormModelBase::applyFeatureModel );
  }

  mFeatureModel = featureModel;

  connect( mFeatureModel, &QfFeatureModel::currentLayerChanged, this, &QfAttributeFormModelBase::onCurrentLayerChanged );
  connect( mFeatureModel, &QfFeatureModel::modelReset, this, &QfAttributeFormModelBase::applyFeatureModel );
  connect( mFeatureModel, &QfFeatureModel::featureUpdated, this, &QfAttributeFormModelBase::applyFeatureModel );
  connect( mFeatureModel, &QfFeatureModel::linkedParentFeatureChanged, this, &QfAttributeFormModelBase::applyFeatureModel );

  emit featureModelChanged();
}

void QfAttributeFormModelBase::onCurrentLayerChanged()
{
  setIsWizard( QgsProject::instance()->readBoolEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "featureFormWizardModeEnabled" ), false ) );
  resetModel();
}

void QfAttributeFormModelBase::resetModel()
{
  clear();

  mVisibilityExpressions.clear();
  mFields.clear();
  mAliasExpressions.clear();
  mReadOnlyExpressions.clear();
  mEditorWidgetCodes.clear();
  mEditorWidgetCodesRequirements.clear();

  setConstraintsHardValid( true );
  setConstraintsSoftValid( true );
  setHasTabs( false );
  setHasRemembrance( false );
  setHasConstraints( false );

  if ( !mFeatureModel )
    return;

  mLayer = mFeatureModel->layer();

  if ( mLayer )
  {
    QgsAttributeEditorContainer *root;
    if ( mLayer->editFormConfig().layout() == Qgis::AttributeFormLayout::DragAndDrop )
    {
      root = mLayer->editFormConfig().invisibleRootContainer();
      mTemporaryContainer.reset();
    }
    else
    {
      root = generateRootContainer();
      mTemporaryContainer.reset( root );
    }

    const bool hasTabs = !root->children().isEmpty() && Qgis::AttributeEditorType::Container == root->children().first()->type();

    invisibleRootItem()->setColumnCount( 1 );
    QList<QStandardItem *> containers;
    if ( hasTabs )
    {
      setHasTabs( true );

      const QList<QgsAttributeEditorElement *> children { root->children() };
      int currentTab = 0;
      for ( QgsAttributeEditorElement *element : children )
      {
        if ( element->type() == Qgis::AttributeEditorType::Container )
        {
          QgsAttributeEditorContainer *container = static_cast<QgsAttributeEditorContainer *>( element );
          const int columnCount = container->columnCount();

          QStandardItem *item = new QStandardItem();
          item->setData( element->name(), QfAttributeFormModel::Name );
          item->setData( "container", QfAttributeFormModel::ElementType );
          item->setData( QString(), QfAttributeFormModel::GroupName );
          item->setData( QModelIndex(), QfAttributeFormModel::GroupIndex );
          item->setData( true, QfAttributeFormModel::CurrentlyVisible );
          item->setData( true, QfAttributeFormModel::ConstraintHardValid );
          item->setData( true, QfAttributeFormModel::ConstraintSoftValid );

          buildForm( container, item, containers, currentTab, columnCount );
          invisibleRootItem()->appendRow( item );

          if ( container->visibilityExpression().enabled() )
          {
            const QString visibilityExpression = container->visibilityExpression().data().expression();
            if ( !visibilityExpression.isEmpty() )
            {
              mVisibilityExpressions.append( qMakePair( container->visibilityExpression().data(), item ) );
            }
          }

          currentTab++;
        }
      }
    }
    else
    {
      buildForm( invisibleRootContainer(), invisibleRootItem(), containers );
    }

    for ( QStandardItem *container : std::as_const( containers ) )
    {
      container->setData( container->index(), QfAttributeFormModel::GroupIndex );
    }
  }
}

QgsExpressionContext QfAttributeFormModelBase::createExpressionContext() const
{
  QgsExpressionContext expressionContext = mFeatureModel->createExpressionContext();
  expressionContext.setFields( mFeatureModel->feature().fields() );
  expressionContext.setFeature( mFeatureModel->feature() );
  expressionContext << QgsExpressionContextUtils::formScope( mFeatureModel->feature() );
  return expressionContext;
}

void QfAttributeFormModelBase::applyFeatureModel()
{
  mExpressionContext = createExpressionContext();

  for ( int i = 0; i < invisibleRootItem()->rowCount(); ++i )
  {
    updateAttributeValue( invisibleRootItem()->child( i ) );
  }

  updateVisibilityAndConstraints();
}

void QfAttributeFormModelBase::applyParentDefaultValues()
{
  if ( mFeatureModel->linkedParentFeature().isValid() )
  {
    const bool featureIsNew = std::numeric_limits<QgsFeatureId>::min() == mFeatureModel->feature().id();
    QgsFields fields = mFeatureModel->feature().fields();
    mExpressionContext.setFields( fields );
    mExpressionContext.setFeature( mFeatureModel->feature() );

    QMap<QStandardItem *, int>::ConstIterator fieldIterator( mFields.constBegin() );
    for ( ; fieldIterator != mFields.constEnd(); ++fieldIterator )
    {
      const int fidx = fieldIterator.value();
      if ( !fields.at( fidx ).defaultValueDefinition().isValid() || ( !fields.at( fidx ).defaultValueDefinition().applyOnUpdate() && !featureIsNew ) )
        continue;

      QgsExpression exp( fields.at( fidx ).defaultValueDefinition().expression() );
      const QSet<QString> referencedFunctions = exp.referencedFunctions();
      const QSet<QString> referencedVariables = exp.referencedVariables();
      if ( referencedFunctions.contains( QStringLiteral( "current_parent_value" ) ) || referencedVariables.contains( QStringLiteral( "current_parent_feature" ) ) || referencedVariables.contains( QStringLiteral( "current_parent_geometry" ) ) )
      {
        exp.prepare( &mExpressionContext );
        const QVariant defaultValue = exp.evaluate( &mExpressionContext );
        const bool success = mFeatureModel->setData( mFeatureModel->index( fidx ), defaultValue, QfFeatureModel::AttributeValue );
        if ( success )
        {
          synchronizeFieldValue( fidx, defaultValue );
        }
      }
    }
  }
}

void QfAttributeFormModelBase::applyRelationshipDefaultValues()
{
  const bool featureIsNew = std::numeric_limits<QgsFeatureId>::min() == mFeatureModel->feature().id();
  QgsFields fields = mFeatureModel->feature().fields();
  mExpressionContext.setFields( fields );
  mExpressionContext.setFeature( mFeatureModel->feature() );
  mExpressionContext.clearCachedValues();

  QMap<QStandardItem *, int>::ConstIterator fieldIterator( mFields.constBegin() );
  for ( ; fieldIterator != mFields.constEnd(); ++fieldIterator )
  {
    const int fidx = fieldIterator.value();
    if ( !fields.at( fidx ).defaultValueDefinition().isValid() || ( !fields.at( fidx ).defaultValueDefinition().applyOnUpdate() && !featureIsNew ) )
      continue;

    if ( fields.at( fidx ).defaultValueDefinition().expression().indexOf( "relation_aggregate(" ) > -1 )
    {
      QgsExpression exp( fields.at( fidx ).defaultValueDefinition().expression() );
      exp.prepare( &mExpressionContext );
      const QVariant defaultValue = exp.evaluate( &mExpressionContext );
      const bool success = mFeatureModel->setData( mFeatureModel->index( fidx ), defaultValue, QfFeatureModel::AttributeValue );
      if ( success )
      {
        synchronizeFieldValue( fidx, defaultValue );
        updateVisibilityAndConstraints( fidx );
      }
    }
  }
}

void QfAttributeFormModelBase::activateAllRememberValues()
{
  QMap<QStandardItem *, int>::ConstIterator fieldIterator( mFields.constBegin() );
  for ( ; fieldIterator != mFields.constEnd(); ++fieldIterator )
  {
    QStandardItem *item = fieldIterator.key();
    if ( data( item->index(), QfAttributeFormModel::CanRememberValue ).toBool() )
    {
      setData( item->index(), true, QfAttributeFormModel::RememberValue );
    }
  }
}

void QfAttributeFormModelBase::deactivateAllRememberValues()
{
  QMap<QStandardItem *, int>::ConstIterator fieldIterator( mFields.constBegin() );
  for ( ; fieldIterator != mFields.constEnd(); ++fieldIterator )
  {
    QStandardItem *item = fieldIterator.key();
    if ( data( item->index(), QfAttributeFormModel::CanRememberValue ).toBool() )
    {
      setData( item->index(), false, QfAttributeFormModel::RememberValue );
    }
  }
}

QgsAttributeEditorContainer *QfAttributeFormModelBase::generateRootContainer() const
{
  QgsAttributeEditorContainer *root = new QgsAttributeEditorContainer( QString(), nullptr );
  //get fields
  QgsFields fields = mLayer->fields();
  for ( int i = 0; i < fields.size(); ++i )
  {
    if ( fields.at( i ).editorWidgetSetup().type() != QStringLiteral( "Hidden" ) )
    {
      QgsAttributeEditorField *field = new QgsAttributeEditorField( fields.at( i ).name(), i, root );
      root->addChildElement( field );
    }
  }
  //get relations
  const QList<QgsRelation> referencingRelations = QgsProject::instance()->relationManager()->referencedRelations( mLayer );
  for ( const QgsRelation &referencingRelation : referencingRelations )
  {
    QgsAttributeEditorRelation *relation = new QgsAttributeEditorRelation( referencingRelation, root );
    root->addChildElement( relation );
  }
  return root;
}

QgsAttributeEditorContainer *QfAttributeFormModelBase::invisibleRootContainer() const
{
  return mTemporaryContainer ? mTemporaryContainer.get() : mLayer->editFormConfig().invisibleRootContainer();
}

void QfAttributeFormModelBase::updateAttributeValue( QStandardItem *item )
{
  if ( item->data( QfAttributeFormModel::ElementType ) == QStringLiteral( "field" ) )
  {
    int fieldIndex = item->data( QfAttributeFormModel::FieldIndex ).toInt();
    QVariant attributeValue = mFeatureModel->data( mFeatureModel->index( fieldIndex ), QfFeatureModel::AttributeValue );

    if ( attributeValue.userType() == QMetaType::QDate )
    {
      // if the field is a QDate, the automatic conversion to JS date [1]
      // leads to the creation of date time object with the time zone.
      // For instance shapefiles has support for dates but not date/time or time.
      // So a date coming from a shapefile as 2001-01-01 will become 2000-12-31 19:00:00 -05 in QML/JS (in the carribeans).
      // And when formatting this with the display format, this is shown as 2000-12-31.
      // So we detect if the field is a date only and revert the time zone offset.
      // [1] http://doc.qt.io/qt-5/qtqml-cppintegration-data.html#basic-qt-data-types

      const QDate d = attributeValue.toDate();
      attributeValue = QDateTime( d, QTime() );
    }

    item->setData( attributeValue.isNull() ? QVariant() : attributeValue, QfAttributeFormModel::AttributeValue );
    item->setData( mFeatureModel->data( mFeatureModel->index( fieldIndex ), QfFeatureModel::AttributeAllowEdit ), QfAttributeFormModel::AttributeAllowEdit );
    // set item editable state to false in case it's a linked attribute
    if ( mFeatureModel->data( mFeatureModel->index( fieldIndex ), QfFeatureModel::LinkedAttribute ).toBool() )
    {
      item->setData( false, QfAttributeFormModel::AttributeEditable );
    }
    else
    {
      if ( mReadOnlyExpressions.contains( item ) )
      {
        QgsExpression exp( mReadOnlyExpressions[item] );
        exp.prepare( &mExpressionContext );
        QVariant result = exp.evaluate( &mExpressionContext );
        item->setData( result.isValid() && result.toBool() == true, QfAttributeFormModel::AttributeEditable );
      }
    }

    if ( mAliasExpressions.contains( item ) )
    {
      QgsExpression exp( mAliasExpressions[item] );
      exp.prepare( &mExpressionContext );
      QVariant result = exp.evaluate( &mExpressionContext );
      if ( result.isValid() )
      {
        item->setData( result, QfAttributeFormModel::Name );
      }
    }
  }
  else if ( item->data( QfAttributeFormModel::ElementType ) == QStringLiteral( "html" ) )
  {
    QString code = mEditorWidgetCodes[item];

    QRegularExpression re( R"re(expression\.evaluate\s*\(\s*"(.*?[^\\])"\))re", QRegularExpression::PatternOption::MultilineOption | QRegularExpression::PatternOption::DotMatchesEverythingOption );
    QRegularExpressionMatch match = re.match( code );
    while ( match.hasMatch() )
    {
      QString expression = match.captured( 1 );
      expression = expression.replace( QStringLiteral( "\\\"" ), QStringLiteral( "\"" ) );

      mExpressionContext.setFeature( mFeatureModel->feature() );

      QgsExpression exp = QgsExpression( expression );
      exp.prepare( &mExpressionContext );
      QVariant result = exp.evaluate( &mExpressionContext );

      QString resultString;
      switch ( static_cast<QMetaType::Type>( result.typeId() ) )
      {
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::Double:
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
          resultString = result.toString();
          break;
        case QMetaType::Bool:
          resultString = result.toBool() ? QStringLiteral( "true" ) : QStringLiteral( "false" );
          break;
        default:
          resultString = QStringLiteral( "'%1'" ).arg( result.toString() );
          break;
      }
      code = code.mid( 0, match.capturedStart( 0 ) ) + resultString + code.mid( match.capturedEnd( 0 ) );
      match = re.match( code );
    }
    item->setData( code, QfAttributeFormModel::EditorWidgetCode );
  }
  else if ( item->data( QfAttributeFormModel::ElementType ) == QStringLiteral( "text" ) )
  {
    QString code = mEditorWidgetCodes[item];

    mExpressionContext.setFeature( mFeatureModel->feature() );
    code = QgsExpression::replaceExpressionText( code, &mExpressionContext );
    item->setData( code, QfAttributeFormModel::EditorWidgetCode );
  }
  else
  {
    for ( int i = 0; i < item->rowCount(); ++i )
    {
      updateAttributeValue( item->child( i ) );
    }
  }
}

void QfAttributeFormModelBase::buildForm( QgsAttributeEditorContainer *container, QStandardItem *parent, QList<QStandardItem *> &containers, int currentTabIndex, int columnCount )
{
  const QList<QgsAttributeEditorElement *> children { container->children() };
  for ( QgsAttributeEditorElement *element : children )
  {
    QStandardItem *item = new QStandardItem();
    item->setData( columnCount, QfAttributeFormModel::ColumnCount );
    item->setData( currentTabIndex, QfAttributeFormModel::TabIndex );
    item->setData( QString(), QfAttributeFormModel::GroupName );
    item->setData( QModelIndex(), QfAttributeFormModel::GroupIndex );
    item->setData( true, QfAttributeFormModel::ConstraintHardValid );
    item->setData( true, QfAttributeFormModel::ConstraintSoftValid );
    item->setData( false, QfAttributeFormModel::CanRememberValue );

    QgsAttributeEditorElement::LabelStyle labelStyle = element->labelStyle();
    item->setData( labelStyle.overrideColor, QfAttributeFormModel::LabelOverrideColor );
    item->setData( labelStyle.overrideColor ? labelStyle.color : QColor(), QfAttributeFormModel::LabelColor );
    item->setData( labelStyle.overrideFont, QfAttributeFormModel::LabelOverrideFont );
    item->setData( labelStyle.overrideFont ? labelStyle.font : QFont(), QfAttributeFormModel::LabelFont );

    switch ( element->type() )
    {
      case Qgis::AttributeEditorType::Container:
      {
        QgsAttributeEditorContainer *innerContainer = static_cast<QgsAttributeEditorContainer *>( element );
        const int innerColumnCount = innerContainer->columnCount();

        item->setData( "container", QfAttributeFormModel::ElementType );
        item->setData( element->showLabel() ? innerContainer->name() : QString(), QfAttributeFormModel::Name );
        item->setData( true, QfAttributeFormModel::CurrentlyVisible );
        item->setData( false, QfAttributeFormModel::AttributeEditable );
        item->setData( false, QfAttributeFormModel::AttributeAllowEdit );
        item->setData( element->showLabel() ? innerContainer->name() : QString(), QfAttributeFormModel::GroupName );
        if ( innerContainer->backgroundColor().isValid() )
        {
          item->setData( innerContainer->backgroundColor(), QfAttributeFormModel::GroupColor );
        }

        buildForm( innerContainer, item, containers, 0, innerColumnCount );
        parent->appendRow( item );
        containers << item;

        if ( innerContainer->visibilityExpression().enabled() )
        {
          const QString visibilityExpression = innerContainer->visibilityExpression().data().expression();
          if ( !visibilityExpression.isEmpty() )
          {
            mVisibilityExpressions.append( qMakePair( QgsExpression( visibilityExpression ), item ) );
          }
        }
        break;
      }

      case Qgis::AttributeEditorType::Field:
      {
        QgsAttributeEditorField *editorField = static_cast<QgsAttributeEditorField *>( element );

        // editorField->idx() is not working on joined fields
        const QgsFields fields = mLayer->fields();
        int fieldIndex = fields.lookupField( editorField->name() );

        if ( fieldIndex < 0 || fieldIndex >= mLayer->fields().size() )
          continue;

        const QgsField field = mLayer->fields().at( fieldIndex );
        const QgsEditorWidgetSetup setup = findBest( fieldIndex );

        item->setData( element->showLabel() ? mLayer->attributeDisplayName( fieldIndex ) : QString(), QfAttributeFormModel::Name );
        item->setData( !mLayer->editFormConfig().readOnly( fieldIndex ) && setup.type() != QStringLiteral( "Binary" ), QfAttributeFormModel::AttributeEditable );
        item->setData( setup.type(), QfAttributeFormModel::EditorWidget );
        item->setData( setup.config(), QfAttributeFormModel::EditorWidgetConfig );
        const bool canRemember = mLayer->editFormConfig().reuseLastValuePolicy( fieldIndex ) != Qgis::AttributeFormReuseLastValuePolicy::NotAllowed || QgsProject::instance()->lastSaveVersion().majorVersion() < 4;
        item->setData( canRemember, QfAttributeFormModel::CanRememberValue );
        if ( canRemember )
        {
          setHasRemembrance( true );
        }
        item->setData( mFeatureModel->rememberedAttributes().at( fieldIndex ) ? Qt::Checked : Qt::Unchecked, QfAttributeFormModel::RememberValue );
        item->setData( QgsField( field ), QfAttributeFormModel::Field );
        item->setData( "field", QfAttributeFormModel::ElementType );
        item->setData( fieldIndex, QfAttributeFormModel::FieldIndex );
        item->setData( true, QfAttributeFormModel::CurrentlyVisible );
        item->setData( mFeatureModel->data( mFeatureModel->index( fieldIndex ), QfFeatureModel::AttributeAllowEdit ), QfAttributeFormModel::AttributeAllowEdit );

        // create constraint description
        QStringList descriptions;
        if ( field.constraints().constraints() & QgsFieldConstraints::ConstraintExpression )
        {
          descriptions << ( !field.constraints().constraintDescription().isEmpty()
                              ? field.constraints().constraintDescription()
                              : tr( "Expression constraint" ) );
        }
        if ( field.constraints().constraints() & QgsFieldConstraints::ConstraintNotNull )
        {
          descriptions << tr( "Not NULL" );
        }
        if ( field.constraints().constraints() & QgsFieldConstraints::ConstraintUnique )
        {
          descriptions << tr( "Unique" );
        }

        if ( !descriptions.isEmpty() )
        {
          setHasConstraints( true );
          item->setData( descriptions.join( ", " ), QfAttributeFormModel::ConstraintDescription );
        }
        else
        {
          item->setData( QString(), QfAttributeFormModel::ConstraintDescription );
        }

        if ( mLayer->editFormConfig().dataDefinedFieldProperties( field.name() ).isActive( QgsEditFormConfig::DataDefinedProperty::Alias ) )
        {
          QgsProperty property = mLayer->editFormConfig().dataDefinedFieldProperties( field.name() ).property( QgsEditFormConfig::DataDefinedProperty::Alias );
          mAliasExpressions.insert( item, property.asExpression() );
        }

        if ( mLayer->editFormConfig().dataDefinedFieldProperties( field.name() ).isActive( QgsEditFormConfig::DataDefinedProperty::Editable ) )
        {
          QgsProperty property = mLayer->editFormConfig().dataDefinedFieldProperties( field.name() ).property( QgsEditFormConfig::DataDefinedProperty::Editable );
          mReadOnlyExpressions.insert( item, property.asExpression() );
        }

        updateAttributeValue( item );

        mFields.insert( item, fieldIndex );

        parent->appendRow( item );
        break;
      }

      case Qgis::AttributeEditorType::Relation:
      {
        QgsAttributeEditorRelation *editorRelation = static_cast<QgsAttributeEditorRelation *>( element );
        const QgsRelation relation = editorRelation->relation();

        item->setData( element->showLabel() ? !editorRelation->label().isEmpty() ? editorRelation->label() : relation.name() : QString(), QfAttributeFormModel::Name );
        item->setData( true, QfAttributeFormModel::AttributeEditable );
        item->setData( true, QfAttributeFormModel::CurrentlyVisible );
        item->setData( "relation", QfAttributeFormModel::ElementType );
        item->setData( "RelationEditor", QfAttributeFormModel::EditorWidget );
        QString relationWidgetType = editorRelation->relationWidgetTypeId();
        if ( relationWidgetType != QLatin1String( "ordered_relation_editor" ) )
        {
          QgsVectorLayer *referencingLayer = relation.referencingLayer();
          if ( referencingLayer )
          {
            for ( int i = 0; i < referencingLayer->fields().count(); i++ )
            {
              if ( referencingLayer->editorWidgetSetup( i ).type() == QLatin1String( "ExternalResource" ) )
              {
                relationWidgetType = QStringLiteral( "gallery_relation_editor" );
                break;
              }
            }
          }
        }
        item->setData( relationWidgetType, QfAttributeFormModel::RelationEditorWidget );
        item->setData( editorRelation->relationEditorConfiguration(), QfAttributeFormModel::RelationEditorWidgetConfig );
        item->setData( relation.id(), QfAttributeFormModel::RelationId );
        item->setData( editorRelation->nmRelationId(), QfAttributeFormModel::NmRelationId );
        item->setData( true, QfAttributeFormModel::CurrentlyVisible );
        item->setData( true, QfAttributeFormModel::ConstraintHardValid );
        item->setData( true, QfAttributeFormModel::ConstraintSoftValid );
        item->setData( true, QfAttributeFormModel::AttributeAllowEdit );

        parent->appendRow( item );
        break;
      }

      case Qgis::AttributeEditorType::QmlElement:
      {
        QgsAttributeEditorQmlElement *qmlElement = static_cast<QgsAttributeEditorQmlElement *>( element );

        item->setData( "qml", QfAttributeFormModel::ElementType );
        item->setData( element->showLabel() ? qmlElement->name() : QString(), QfAttributeFormModel::Name );
        item->setData( true, QfAttributeFormModel::CurrentlyVisible );
        item->setData( false, QfAttributeFormModel::AttributeEditable );
        item->setData( false, QfAttributeFormModel::AttributeAllowEdit );
        item->setData( qmlElement->qmlCode(), QfAttributeFormModel::EditorWidgetCode );

        parent->appendRow( item );
        break;
      }

      case Qgis::AttributeEditorType::HtmlElement:
      {
        QgsAttributeEditorHtmlElement *htmlElement = static_cast<QgsAttributeEditorHtmlElement *>( element );

        item->setData( "html", QfAttributeFormModel::ElementType );
        item->setData( element->showLabel() ? htmlElement->name() : QString(), QfAttributeFormModel::Name );
        item->setData( true, QfAttributeFormModel::CurrentlyVisible );
        item->setData( false, QfAttributeFormModel::AttributeEditable );
        item->setData( false, QfAttributeFormModel::AttributeAllowEdit );

        updateAttributeValue( item );
        parent->appendRow( item );
        mEditorWidgetCodes.insert( item, htmlElement->htmlCode() );
        break;
      }

      case Qgis::AttributeEditorType::TextElement:
      {
        QgsAttributeEditorTextElement *textElement = static_cast<QgsAttributeEditorTextElement *>( element );

        item->setData( "text", QfAttributeFormModel::ElementType );
        item->setData( element->showLabel() ? textElement->name() : QString(), QfAttributeFormModel::Name );
        item->setData( true, QfAttributeFormModel::CurrentlyVisible );
        item->setData( false, QfAttributeFormModel::AttributeEditable );
        item->setData( false, QfAttributeFormModel::AttributeAllowEdit );

        updateAttributeValue( item );
        parent->appendRow( item );
        mEditorWidgetCodes.insert( item, textElement->text() );
        break;
      }

      case Qgis::AttributeEditorType::SpacerElement:
      {
        QgsAttributeEditorSpacerElement *spacerElement = static_cast<QgsAttributeEditorSpacerElement *>( element );

        item->setData( "spacer", QfAttributeFormModel::ElementType );
        item->setData( spacerElement->drawLine() ? QStringLiteral( "-" ) : QString(), QfAttributeFormModel::Name );
        item->setData( true, QfAttributeFormModel::CurrentlyVisible );
        item->setData( false, QfAttributeFormModel::AttributeEditable );
        item->setData( false, QfAttributeFormModel::AttributeAllowEdit );

        parent->appendRow( item );
        break;
      }

      case Qgis::AttributeEditorType::Action:
      case Qgis::AttributeEditorType::Invalid:
        // TODO: implement
        delete item;
        break;
    }
  }
}

void QfAttributeFormModelBase::synchronizeFieldValue( int fieldIndex, QVariant value )
{
  QMap<QStandardItem *, int>::ConstIterator fieldIterator( mFields.constBegin() );
  for ( ; fieldIterator != mFields.constEnd(); ++fieldIterator )
  {
    QStandardItem *item = fieldIterator.key();
    const int fidx = fieldIterator.value();
    if ( fidx != fieldIndex )
    {
      continue;
    }

    item->setData( value, QfAttributeFormModel::AttributeValue );
  }
}

void QfAttributeFormModelBase::updateDefaultValues( int fieldIndex, QVector<int> updatedFields )
{
  const QgsFields fields = mFeatureModel->feature().fields();
  if ( fieldIndex < 0 || fieldIndex >= fields.size() )
    return;
  const QString fieldName = fields.at( fieldIndex ).name();

  mExpressionContext.setFields( fields );
  mExpressionContext.setFeature( mFeatureModel->feature() );

  QMap<QStandardItem *, int>::ConstIterator fieldIterator( mFields.constBegin() );
  for ( ; fieldIterator != mFields.constEnd(); ++fieldIterator )
  {
    const int fidx = fieldIterator.value();
    if ( fidx == fieldIndex || !fields.at( fidx ).defaultValueDefinition().isValid() || !fields.at( fidx ).defaultValueDefinition().applyOnUpdate() )
      continue;

    QgsExpression exp( fields.at( fidx ).defaultValueDefinition().expression() );
    exp.prepare( &mExpressionContext );

    // avoid cost of value update if expression doesn't contain the field which triggered the default values update
    if ( !exp.referencedColumns().contains( fieldName ) && !exp.referencedColumns().contains( QgsFeatureRequest::ALL_ATTRIBUTES ) )
    {
      continue;
    }

    const QVariant defaultValue = exp.evaluate( &mExpressionContext );
    const QVariant previousValue = mFeatureModel->data( mFeatureModel->index( fidx ), QfFeatureModel::AttributeValue );
    const bool success = mFeatureModel->setData( mFeatureModel->index( fidx ), defaultValue, QfFeatureModel::AttributeValue );
    const QVariant updatedValue = mFeatureModel->data( mFeatureModel->index( fidx ), QfFeatureModel::AttributeValue );
    if ( success && updatedValue != previousValue )
    {
      mExpressionContext.popScope();
      mExpressionContext << QgsExpressionContextUtils::formScope( mFeatureModel->feature() );

      synchronizeFieldValue( fidx, updatedValue );
      if ( !updatedFields.contains( fidx ) )
      {
        updatedFields.append( fidx );
        updateDefaultValues( fidx, updatedFields );
      }
      updateVisibilityAndConstraints( fidx );
    }
  }

  updateDataDefinedProperties( fieldName );
  updateEditorWidgetCodes( fieldName );
}

bool QfAttributeFormModelBase::codeRequiresUpdate( const QString &fieldName, const QString &code, const QRegularExpression &regEx )
{
  if ( !mEditorWidgetCodesRequirements.contains( code ) )
  {
    CodeRequirements codeRequirements;
    QRegularExpressionMatchIterator matchIt = regEx.globalMatch( code );
    while ( matchIt.hasNext() )
    {
      const QRegularExpressionMatch match = matchIt.next();
      QString expression = match.captured( 1 );
      expression = expression.replace( QStringLiteral( "\\\"" ), QStringLiteral( "\"" ) );

      QgsExpression exp( expression );
      exp.prepare( &mExpressionContext );
      codeRequirements.referencedColumns.unite( exp.referencedColumns() );
      codeRequirements.formScope = codeRequirements.formScope || QgsValueRelationFieldFormatter::expressionRequiresFormScope( expression );
    }
    mEditorWidgetCodesRequirements.insert( code, codeRequirements );
  }

  return mEditorWidgetCodesRequirements[code].referencedColumns.contains( fieldName ) || mEditorWidgetCodesRequirements[code].referencedColumns.contains( QgsFeatureRequest::ALL_ATTRIBUTES ) || mEditorWidgetCodesRequirements[code].formScope;
}

void QfAttributeFormModelBase::updateDataDefinedProperties( const QString &fieldName )
{
  QMap<QStandardItem *, QString>::ConstIterator aliasExpressionsIterator( mAliasExpressions.constBegin() );
  for ( ; aliasExpressionsIterator != mAliasExpressions.constEnd(); aliasExpressionsIterator++ )
  {
    QStandardItem *item = aliasExpressionsIterator.key();
    if ( !item )
    {
      continue;
    }

    QgsExpression exp( aliasExpressionsIterator.value() );
    exp.referencedColumns().contains( fieldName );
    {
      exp.prepare( &mExpressionContext );
      QVariant result = exp.evaluate( &mExpressionContext );
      if ( result.isValid() )
      {
        item->setData( result, QfAttributeFormModel::Name );
      }
    }
  }

  QMap<QStandardItem *, QString>::ConstIterator readOnlyExpressionsIterator( mReadOnlyExpressions.constBegin() );
  for ( ; readOnlyExpressionsIterator != mReadOnlyExpressions.constEnd(); readOnlyExpressionsIterator++ )
  {
    QStandardItem *item = readOnlyExpressionsIterator.key();
    if ( !item )
    {
      continue;
    }
    const int fieldIndex = item->data( QfAttributeFormModel::FieldIndex ).toInt();
    if ( mFeatureModel->data( mFeatureModel->index( fieldIndex ), QfFeatureModel::LinkedAttribute ).toBool() )
    {
      continue;
    }

    QgsExpression exp( readOnlyExpressionsIterator.value() );
    exp.referencedColumns().contains( fieldName );
    {
      exp.prepare( &mExpressionContext );
      QVariant result = exp.evaluate( &mExpressionContext );
      item->setData( result.isValid() && result.toBool() == true, QfAttributeFormModel::AttributeEditable );
    }
  }
}

void QfAttributeFormModelBase::updateEditorWidgetCodes( const QString &fieldName )
{
  QMap<QStandardItem *, QString>::ConstIterator editorWidgetCodesIterator( mEditorWidgetCodes.constBegin() );
  for ( ; editorWidgetCodesIterator != mEditorWidgetCodes.constEnd(); editorWidgetCodesIterator++ )
  {
    QStandardItem *item = editorWidgetCodesIterator.key();
    if ( !item )
    {
      continue;
    }
    QString code = editorWidgetCodesIterator.value();
    if ( item->data( QfAttributeFormModel::ElementType ) == QStringLiteral( "qml" ) || item->data( QfAttributeFormModel::ElementType ) == QStringLiteral( "html" ) )
    {
      const thread_local QRegularExpression sRegEx( "expression\\.evaluate\\(\\s*\\\"(.*?[^\\\\])\\\"\\s*\\)", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption );
      if ( codeRequiresUpdate( fieldName, code, sRegEx ) )
      {
        QRegularExpressionMatch match = sRegEx.match( code );
        while ( match.hasMatch() )
        {
          QString expression = match.captured( 1 );
          expression = expression.replace( QStringLiteral( "\\\"" ), QStringLiteral( "\"" ) );

          QgsExpression exp = QgsExpression( expression );
          exp.prepare( &mExpressionContext );
          QVariant result = exp.evaluate( &mExpressionContext );

          QString resultString;
          switch ( static_cast<QMetaType::Type>( result.typeId() ) )
          {
            case QMetaType::Int:
            case QMetaType::UInt:
            case QMetaType::Double:
            case QMetaType::LongLong:
            case QMetaType::ULongLong:
              resultString = result.toString();
              break;
            case QMetaType::Bool:
              resultString = result.toBool() ? QStringLiteral( "true" ) : QStringLiteral( "false" );
              break;
            default:
              resultString = QStringLiteral( "'%1'" ).arg( result.toString() );
              break;
          }
          code = code.mid( 0, match.capturedStart( 0 ) ) + resultString + code.mid( match.capturedEnd( 0 ) );
          match = sRegEx.match( code );
        }
        item->setData( code, QfAttributeFormModel::EditorWidgetCode );
      }
    }
    else if ( item->data( QfAttributeFormModel::ElementType ) == QStringLiteral( "text" ) )
    {
      const thread_local QRegularExpression sRegEx( QStringLiteral( "\\[%(.*?)%\\]" ), QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption );
      if ( codeRequiresUpdate( fieldName, code, sRegEx ) )
      {
        code = QgsExpression::replaceExpressionText( code, &mExpressionContext );
        item->setData( code, QfAttributeFormModel::EditorWidgetCode );
      }
    }
  }
}

void _checkChildrenValidity( const QStandardItem *parent, bool &hardValidity, bool &softValidity )
{
  QStandardItem *item = parent->child( 0, 0 );
  while ( item )
  {
    const bool isVisible = item->data( QfAttributeFormModel::CurrentlyVisible ).toBool();
    if ( isVisible )
    {
      if ( !item->data( QfAttributeFormModel::ConstraintHardValid ).toBool() )
      {
        hardValidity = false;
        break;
      }
      if ( !item->data( QfAttributeFormModel::ConstraintSoftValid ).toBool() )
      {
        softValidity = false;
      }
    }

    if ( isVisible && item->hasChildren() )
    {
      _checkChildrenValidity( item, hardValidity, softValidity );
    }
    item = parent->child( item->row() + 1, 0 );
  }
};

void QfAttributeFormModelBase::updateVisibilityAndConstraints( int fieldIndex )
{
  const QString fieldName = fieldIndex > -1 && fieldIndex < mLayer->fields().size() ? mLayer->fields().at( fieldIndex ).name() : QString();
  QgsFields fields = mFeatureModel->feature().fields();
  mExpressionContext.setFields( fields );
  mExpressionContext.setFeature( mFeatureModel->feature() );

  bool visibilityChanged = false;
  for ( const VisibilityExpression &it : std::as_const( mVisibilityExpressions ) )
  {
    // If triggered by an updated field index (fieldIndex), check if the visibility
    // expression refers to that field
    if ( fieldIndex == -1 || it.first.referencedColumns().contains( fieldName ) || it.first.referencedColumns().contains( QgsFeatureRequest::ALL_ATTRIBUTES ) )
    {
      QgsExpression exp = it.first;
      exp.prepare( &mExpressionContext );

      bool visible = exp.evaluate( &mExpressionContext ).toInt();
      QStandardItem *item = it.second;
      if ( item->data( QfAttributeFormModel::CurrentlyVisible ).toBool() != visible )
      {
        item->setData( visible, QfAttributeFormModel::CurrentlyVisible );
        visibilityChanged = true;
      }
    }
  }

  QMap<QStandardItem *, int>::ConstIterator fieldIterator( mFields.constBegin() );
  QMap<int, bool> hardConstraintsCache;
  QMap<int, bool> softConstraintsCache;
  bool validityChanged = false;
  for ( ; fieldIterator != mFields.constEnd(); ++fieldIterator )
  {
    QStandardItem *item = fieldIterator.key();
    int fidx = fieldIterator.value();
    if ( fieldIndex != -1 && fidx != fieldIndex )
    {
      // Check whether the current field iterator index (fidx) has an expression constraints referencing the
      // updated field index (fieldIndex) which triggered a constraints update
      const QgsExpression expression = mLayer->fields().at( fidx ).constraints().constraintExpression();
      if ( !expression.referencedColumns().contains( fieldName ) && !expression.referencedColumns().contains( QgsFeatureRequest::ALL_ATTRIBUTES ) )
      {
        continue;
      }
    }

    if ( mFeatureModel->data( mFeatureModel->index( fidx ), QfFeatureModel::AttributeAllowEdit ) == true )
    {
      QStringList errors;

      QgsFeature feature = mFeatureModel->feature();
      QString defaultValueClause = mLayer->dataProvider()->defaultValueClause( fidx );
      QString attrValue = feature.attribute( fidx ).toString();

      // Providers will check for a literal "defaultValueClause" to autogenerate PKs.
      // For example, the gpkg provider will generate a fid if it is set to "Autogenerate".
      // On QField, if the user leaves the field empty, we will assume he wants to autogenerate it.
      // This makes sure, the NOT NULL constraint is skipped in this case.
      if ( attrValue.isEmpty() && !defaultValueClause.isEmpty() )
      {
        feature.setAttribute( fidx, defaultValueClause );
      }

      bool hardConstraintSatisfied = false;
      if ( !hardConstraintsCache.contains( fidx ) )
      {
        hardConstraintSatisfied = QgsVectorLayerUtils::validateAttribute( mLayer, feature, fidx, errors, QgsFieldConstraints::ConstraintStrengthHard );
        hardConstraintsCache[fidx] = hardConstraintSatisfied;
      }
      else
      {
        hardConstraintSatisfied = hardConstraintsCache.value( fidx );
      }
      if ( hardConstraintSatisfied != item->data( QfAttributeFormModel::ConstraintHardValid ).toBool() )
      {
        item->setData( hardConstraintSatisfied, QfAttributeFormModel::ConstraintHardValid );
        validityChanged = true;
      }

      bool softConstraintSatisfied = false;
      if ( !softConstraintsCache.contains( fidx ) )
      {
        softConstraintSatisfied = QgsVectorLayerUtils::validateAttribute( mLayer, mFeatureModel->feature(), fidx, errors, QgsFieldConstraints::ConstraintStrengthSoft );
        softConstraintsCache[fidx] = softConstraintSatisfied;
      }
      else
      {
        softConstraintSatisfied = softConstraintsCache.value( fidx );
      }
      if ( softConstraintSatisfied != item->data( QfAttributeFormModel::ConstraintSoftValid ).toBool() )
      {
        item->setData( softConstraintSatisfied, QfAttributeFormModel::ConstraintSoftValid );
        validityChanged = true;
      }
    }
    else
    {
      item->setData( true, QfAttributeFormModel::ConstraintHardValid );
      item->setData( true, QfAttributeFormModel::ConstraintSoftValid );
    }
  }

  // reset contrainsts status of containers
  if ( validityChanged || visibilityChanged )
  {
    bool allConstraintsHardValid = true;
    bool allConstraintsSoftValid = true;

    if ( mHasTabs )
    {
      QStandardItem *root = invisibleRootItem();
      for ( int i = 0; i < root->rowCount(); i++ )
      {
        bool hardValidity = true;
        bool softValidity = true;

        QStandardItem *tab = root->child( i, 0 );
        _checkChildrenValidity( tab, hardValidity, softValidity );
        tab->setData( hardValidity, QfAttributeFormModel::ConstraintHardValid );
        tab->setData( softValidity, QfAttributeFormModel::ConstraintSoftValid );
        if ( tab->data( QfAttributeFormModel::CurrentlyVisible ).toBool() )
        {
          if ( !hardValidity )
          {
            allConstraintsHardValid = false;
          }
          if ( !softValidity )
          {
            allConstraintsSoftValid = false;
          }
        }
      }
    }
    else
    {
      bool hardValidity = true;
      bool softValidity = true;
      const QStandardItem *tab = invisibleRootItem();
      _checkChildrenValidity( tab, hardValidity, softValidity );

      if ( !hardValidity )
      {
        allConstraintsHardValid = false;
      }
      if ( !softValidity )
      {
        allConstraintsSoftValid = false;
      }
    }

    setConstraintsHardValid( allConstraintsHardValid );
    setConstraintsSoftValid( allConstraintsSoftValid );
  }
}

bool QfAttributeFormModelBase::constraintsHardValid() const
{
  return mConstraintsHardValid;
}

bool QfAttributeFormModelBase::constraintsSoftValid() const
{
  return mConstraintsSoftValid;
}

QVariant QfAttributeFormModelBase::attribute( const QString &name )
{
  if ( !mLayer )
    return QVariant();

  const int fieldIndex = mLayer->fields().indexOf( name );
  return mFeatureModel->data( mFeatureModel->index( fieldIndex ), QfFeatureModel::AttributeValue );
}

bool QfAttributeFormModelBase::changeAttribute( const QString &name, const QVariant &value )
{
  if ( !mLayer || !mFeatureModel )
    return false;

  const int fieldIndex = mLayer->fields().indexOf( name );
  const bool changed = mFeatureModel->setData( mFeatureModel->index( fieldIndex ), value, QfFeatureModel::AttributeValue );
  if ( changed )
  {
    mExpressionContext.popScope();
    mExpressionContext << QgsExpressionContextUtils::formScope( mFeatureModel->feature() );
    synchronizeFieldValue( fieldIndex, value );
    updateDefaultValues( fieldIndex );
    updateVisibilityAndConstraints( fieldIndex );
  }

  return changed;
}

bool QfAttributeFormModelBase::changeGeometry( const QgsGeometry &geometry )
{
  if ( !mLayer || !mFeatureModel )
    return false;

  const bool changed = mFeatureModel->changeGeometry( geometry );
  if ( changed )
  {
    mExpressionContext.popScope();
    mExpressionContext << QgsExpressionContextUtils::formScope( mFeatureModel->feature() );
    updateDefaultValues();
    updateVisibilityAndConstraints();
  }

  return changed;
}

void QfAttributeFormModelBase::setConstraintsHardValid( bool constraintsHardValid )
{
  if ( constraintsHardValid == mConstraintsHardValid )
    return;

  mConstraintsHardValid = constraintsHardValid;
  emit constraintsHardValidChanged();
}

void QfAttributeFormModelBase::setConstraintsSoftValid( bool constraintsSoftValid )
{
  if ( constraintsSoftValid == mConstraintsSoftValid )
    return;

  mConstraintsSoftValid = constraintsSoftValid;
  emit constraintsSoftValidChanged();
}

QgsEditorWidgetSetup QfAttributeFormModelBase::findBest( const int fieldIndex )
{
  QgsFields fields = mLayer->fields();
  if ( fieldIndex >= 0 && fieldIndex < fields.count() )
  {
    //when field has a configured setup, take it
    QgsEditorWidgetSetup configuredSetup = mLayer->editorWidgetSetup( fieldIndex );
    if ( !configuredSetup.isNull() )
    {
      if ( !sSupportedEditorWidgets->contains( configuredSetup.type() ) )
      {
        return QgsEditorWidgetSetup( QStringLiteral( "TextEdit" ), QVariantMap() );
      }
      return configuredSetup;
    }

    //when it's a provider field with default value clause, take Textedit
    if ( fields.fieldOrigin( fieldIndex ) == Qgis::FieldOrigin::Provider )
    {
      const int providerOrigin = fields.fieldOriginIndex( fieldIndex );
      if ( !mLayer->dataProvider()->defaultValueClause( providerOrigin ).isEmpty() )
      {
        return QgsEditorWidgetSetup( QStringLiteral( "TextEdit" ), QVariantMap() );
      }
    }

    if ( !mLayer->referencingRelations( fieldIndex ).isEmpty() )
    {
      QgsRelation relation = mLayer->referencingRelations( fieldIndex )[0];
      QVariantMap config;
      config.insert( QStringLiteral( "Relation" ), relation.id() );
      config.insert( QStringLiteral( "AllowAddFeatures" ), false );
      config.insert( QStringLiteral( "ShowOpenFormButton" ), true );
      return QgsEditorWidgetSetup( QStringLiteral( "RelationReference" ), config );
    }

    // Find the best one based on field type
    const QgsField field = fields.at( fieldIndex );
    if ( field.type() == QMetaType::Bool )
    {
      // on a boolean type, take "CheckBox"
      return QgsEditorWidgetSetup( QStringLiteral( "CheckBox" ), QVariantMap() );
    }
    else if ( field.isDateOrTime() )
    {
      // on a time types, take "DateTime"
      QVariantMap config;
      config.insert( QStringLiteral( "field_format" ), QgsDateTimeFieldFormatter::defaultFormat( field.type() ) );
      config.insert( QStringLiteral( "display_format" ), QgsDateTimeFieldFormatter::defaultFormat( field.type() ) );
      config.insert( QStringLiteral( "calendar_popup" ), true );
      config.insert( QStringLiteral( "allow_null" ), true );
      return QgsEditorWidgetSetup( QStringLiteral( "DateTime" ), config );
    }
    else if ( field.type() == QMetaType::Int || field.type() == QMetaType::Double || field.isNumeric() )
    {
      // on numeric types, take "Range"
      return QgsEditorWidgetSetup( QStringLiteral( "Range" ), QVariantMap() );
    }
    else if ( field.typeName() == QStringLiteral( "Binary" ) )
    {
      // on blob type, take "Binary"
      return QgsEditorWidgetSetup( QStringLiteral( "Binary" ), QVariantMap() );
    }
  }

  return QgsEditorWidgetSetup( QStringLiteral( "TextEdit" ), QVariantMap() );
}

bool QfAttributeFormModelBase::isWizard() const
{
  return mIsWizard;
}

void QfAttributeFormModelBase::setIsWizard( bool isWizard )
{
  if ( mIsWizard == isWizard )
    return;

  mIsWizard = isWizard;
  emit isWizardChanged();
}

bool QfAttributeFormModelBase::hasTabs() const
{
  return mHasTabs;
}

void QfAttributeFormModelBase::setHasTabs( bool hasTabs )
{
  if ( hasTabs == mHasTabs )
    return;

  mHasTabs = hasTabs;
  emit hasTabsChanged();
}

bool QfAttributeFormModelBase::hasRemembrance() const
{
  return mHasRemembrance;
}

void QfAttributeFormModelBase::setHasRemembrance( bool hasRemembrance )
{
  if ( hasRemembrance == mHasRemembrance )
    return;

  mHasRemembrance = hasRemembrance;
  emit hasRemembranceChanged();
}

bool QfAttributeFormModelBase::hasConstraints() const
{
  return mHasConstraints;
}

void QfAttributeFormModelBase::setHasConstraints( bool hasConstraints )
{
  if ( hasConstraints == mHasConstraints )
    return;

  mHasConstraints = hasConstraints;
  emit hasConstraintsChanged();
}

bool QfAttributeFormModelBase::save()
{
  return mFeatureModel->save();
}

bool QfAttributeFormModelBase::create()
{
  return mFeatureModel->create();
}

bool QfAttributeFormModelBase::deleteFeature()
{
  return mFeatureModel->deleteFeature();
}
```


