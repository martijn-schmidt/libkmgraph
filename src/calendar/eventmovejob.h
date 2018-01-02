/*
 * This file is part of LibKMGraph library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KMGRAPH2_EVENTMOVEJOB_H
#define KMGRAPH2_EVENTMOVEJOB_H

#include "modifyjob.h"
#include "kmgraphcalendar_export.h"

namespace KMGraph2
{

/**
 * @brief A job that moves events between given calendars
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KMGRAPHCALENDAR_EXPORT EventMoveJob : public KMGraph2::ModifyJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will move given \p event from a calendar
     *        with \p sourceCalendarId to calendar with \p destinationCalendarId
     *
     * @param event Event to move
     * @param sourceCalendarId ID of calendar where the event is currently stored
     * @param destinationCalendarId ID of calendar where to move the event
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventMoveJob(const EventPtr &event, const QString &sourceCalendarId,
                          const QString &destinationCalendarId,
                          const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will move given \p events from a calendar
     *        with \p sourceCalendarId to calendar with \p destinationCalendarId
     *
     * @param events Events to move
     * @param sourceCalendarId ID of calendar where the events are currently stored
     * @param destinationCalendarId ID of calendar where to move the events
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventMoveJob(const EventsList &events, const QString &sourceCalendarId,
                          const QString &destinationCalendarId,
                          const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will move event with given \p eventId from a 
     *        calendar with \p sourceCalendarId to calendar
     *        with \p destinationCalendarId
     *
     * @param eventId ID of event to move
     * @param sourceCalendarId ID of calendar where the event is currently stored
     * @param destinationCalendarId ID of calendar where to move the event
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventMoveJob(const QString &eventId, const QString &sourceCalendarId,
                          const QString &destinationCalendarId,
                          const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will move events with given \p eventsIds
     *        from a calendar with \p sourceCalendarId to calendar
     *        with \p destinationCalendarId
     *
     * @param eventsIds IDs of events to move
     * @param sourceCalendarId ID of calendar where the events are currently stored
     * @param destinationCalendarId ID of calendar where to move the events
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventMoveJob(const QStringList &eventsIds,
                          const QString &sourceCalendarId,
                          const QString &destinationCalendarId,
                          const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    virtual ~EventMoveJob();

  protected:

    /**
     * @brief KMGraph2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KMGraph2::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType) override;

    /**
     * @brief KMGraph2::Job::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    KMGraph2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                                     const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KMGraph2

#endif // KMGRAPH2_EVENTMOVEJOB_H
