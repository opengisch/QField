

# File drawingtemplatemodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**drawingtemplatemodel.h**](drawingtemplatemodel_8h.md)

[Go to the documentation of this file](drawingtemplatemodel_8h.md)


```C++
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

class DrawingTemplateModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QString projectFilePath READ projectFilePath WRITE setProjectFilePath NOTIFY projectFilePathChanged )

    
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

    QString projectFilePath() const;

    void setProjectFilePath( const QString &path );

    bool hasProjectTemplate() const;

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

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
```


