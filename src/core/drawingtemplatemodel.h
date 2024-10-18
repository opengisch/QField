/***************************************************************************
  drawingtemplatemodel.h

 ---------------------
 begin                : 28.03.2024
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
#ifndef DRAWINGTEMPLATEMODEL_H
#define DRAWINGTEMPLATEMODEL_H

#include <QAbstractListModel>

/**
 * \ingroup core
 */
class DrawingTemplateModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * This property holds the project file path where project templates will be looked for.
     */
    Q_PROPERTY( QString projectFilePath READ projectFilePath WRITE setProjectFilePath NOTIFY projectFilePathChanged )

    /**
     * This property holds whether the model contains project templates.
     */
    Q_PROPERTY( bool hasProjectTemplate READ hasProjectTemplate NOTIFY hasProjectTemplateChanged )

  public:
    enum TemplateType
    {
      AppTemplate,
      ProjectTemplate,
    };

    enum Role
    {
      TemplateTypeRole = Qt::UserRole,
      TemplateTitleRole,
      TemplatePathRole,
    };
    Q_ENUM( Role )

    explicit DrawingTemplateModel( QObject *parent = nullptr );

    //! \copydoc DrawingTemplateModel::projectFilePath
    QString projectFilePath() const;

    //! \copydoc DrawingTemplateModel::projectFilePath
    void setProjectFilePath( const QString &path );

    //! \copydoc DrawingTemplateModel::hasProjectTemplate
    bool hasProjectTemplate() const;

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    /**
     * Reloads the drawing template model.
     */
    Q_INVOKABLE void reloadModel();

  signals:
    void projectFilePathChanged();
    void hasProjectTemplateChanged();

  private:
    struct Template
    {
        Template( TemplateType type, const QString &title, const QString &path )
          : type( type )
          , title( title )
          , path( path )
        {}

        TemplateType type = TemplateType::AppTemplate;
        QString title;
        QString path;
    };

    QList<Template> mTemplates;

    QString mProjectFilePath;
    bool mHasProjectTemplate = false;
};

#endif // DRAWINGTEMPLATEMODEL_H
