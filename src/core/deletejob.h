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

#ifndef LIBKMGRAPH2_DELETEJOB_H
#define LIBKMGRAPH2_DELETEJOB_H

#include "job.h"
#include "kmgraphcore_export.h"

namespace KMGraph2 {

/**
 * @headerfile DeleteJob
 * @brief Abstract superclass for all jobs that delete resources from Google
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KMGRAPHCORE_EXPORT DeleteJob : public KMGraph2::Job
{
    Q_OBJECT

  public:

    /**
     * Constructor for jobs that don't require authentication
     *
     * @param parent
     */
    explicit DeleteJob(QObject* parent = nullptr);

    /**
     * @brief Constructor for jobs that require authentication
     *
     * @param account Account to use to authenticate the requests send by this job
     * @param parent
     */
    explicit DeleteJob(const KMGraph2::AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    virtual ~DeleteJob();

  protected:

    /**
     * @brief KMGraph::Job::dispatchRequest immplementation
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
     * Ths implementation will automatically call start() again in order to
     * process another item in queue. Programmer should call emitFinished()
     * from start() if there are no further items to delete.
     *
     * If you need more control over deleting or handling the reply, you can
     * reimplement this method in your subclass.
     *
     * @param rawData
     * @param contentType
     */
    void handleReply(const QNetworkReply *reply, const QByteArray& rawData) override;

  private:
    class Private;
    Private const * d;
    friend class Private;

};

} // namespace KMGraph2

#endif // LIBKMGRAPH2_DELETEJOB_H
