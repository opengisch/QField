/***************************************************************************
  changelogcontents.h - Changelog

 ---------------------
 begin                : Nov 2020
 copyright            : (C) 2020 by Ivan Ivanov
 email                : ivan@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef CHANGELOGCONTENTS_H
#define CHANGELOGCONTENTS_H

#include <QObject>

//! Obtain the QField changelog contents from the GitHub releases API.
class ChangelogContents : public QObject
{
    Q_OBJECT

  //! Holds the current changelog contents formatted as markdown.
  Q_PROPERTY( QString markdown READ markdown NOTIFY markdownChanged )

  //! Holds the current changelog contents status.
  Q_PROPERTY( Status status READ status NOTIFY statusChanged )

  public:
    //! Constructor
    explicit ChangelogContents( QObject *parent = nullptr );

    //! Changelog contents status.
    enum Status
    {
      //! Changelog has not been requested
      IdleStatus,
      //! Changelog has been requested, but still not received
      LoadingStatus,
      //! Changelog has been successfully generated
      SuccessStatus,
      //! Changelog has been requested, but failed to be generated
      ErrorStatus,
    };

    Q_ENUM( Status )

    /**
     * Initiates a HTTP request to obtain the changelog contents.
     * @see markdownChanged()
     * @see statusChanged()
     */
    Q_INVOKABLE void request();

    //! Returns the current changelog contents formatted as markdown. Null string if no changelog.
    QString markdown();

    //! Returns the current changelog contents status.
    Status status();

  signals:

    //! Emitted when the markdown contents has been changed.
    void markdownChanged();

    //! Emitted when the status has been changed.
    void statusChanged();

  private:
    //! Parses a given \a version string into a list of three numbers, e.g. 'v1.5.6-dev' to [1, 5, 6]. Returns an empty list if cannot be parsed.
    QList<int> parseVersion( const QString &version );

    //! Holds the current changelog contents formatted as markdown. Null string if no changelog.
    QString mMarkdown;

    //! Holds the current status.
    Status mStatus = Status::IdleStatus;
};

#endif // CHANGELOGCONTENTS_H
