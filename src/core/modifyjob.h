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

#ifndef LIBKMGRAPH2_MODIFYJOB_H
#define LIBKMGRAPH2_MODIFYJOB_H

#include "job.h"
#include "kmgraphcore_export.h"

namespace KMGraph2 {

/**
 * @headerfile FetchJob
 * @brief Abstract superclass for all jobs that somehow modify resources on Microsoft Graph
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KMGRAPHCORE_EXPORT ModifyJob : public KMGraph2::Job
{
    Q_OBJECT
  public:

    /**
     * @brief Constructor for jobs that don't require authentication
     *
     * @param parent
     */
    explicit ModifyJob(QObject* parent = nullptr);

    /**
     * @brief Constructor for jobs that require authentication
     *
     * @param account Account to use to authenticate the requests sent by this job
     * @param parent
     */
    explicit ModifyJob(const KMGraph2::AccountPtr& account, QObject* parent = nullptr);

    /**
     * Destructor
     */
    virtual ~ModifyJob();

    /**
     * @return Returns modified items.
     */
    ObjectsList items();

  protected:
    /**
     * @brief A reply handler that returns items parsed from \@ rawData
     *
     * This method can be reimplemented in a FetchJob subclasses. It is called
     * automatically when a reply is received and the returned items are stored
     * in FetchJob and accessible via FetchJob::items when the job has finished.
     *
     * If you need more control over handling reply and items, you can reimplement
     * FetchJob::handleReply. Note that reimplementing FetchJob::handleReply
     * usually requires reimplementing FetchJob::items as well and storing the
     * parsed items in your implementation.
     *
     * @param reply A QNetworkReply received from the Microsoft Graph server
     * @param rawData Content of body of the @p reply. Don't use
     *        QNetworkReply::readAll(), because the content has already been read
     *        by Job implementation and thus it would return empty data.
     *
     * @return Items parsed from @p rawData
     */
    virtual ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData);


    /**
     * KMGraph2::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    void dispatchRequest(QNetworkAccessManager* accessManager,
                                 const QNetworkRequest& request,
                                 const QByteArray& data,
                                 const QString& contentType) override;

    /**
     * KMGraph2::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply *reply, const QByteArray& rawData) override;

    /**
     * KMGraph2::Job::aboutToStart() implementation
     */
    void aboutToStart() override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace KMGraph2

#endif // LIBKMGRAPH2_MODIFYJOB_H
