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

#ifndef LIBKMGRAPH2_CONTACTDELETEJOB_H
#define LIBKMGRAPH2_CONTACTDELETEJOB_H

#include "deletejob.h"
#include "kmgraphcontacts_export.h"

namespace KMGraph2 {

/**
 * @brief A job to delete one or more contacts from addressbook in
 *        user's Google Contacts account
 *
 * Note that operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KMGRAPHCONTACTS_EXPORT ContactDeleteJob : public KMGraph2::DeleteJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a new job that will delete given @p contact from
     *        user's addressbook
     *
     * @param contact Contact to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const ContactPtr &contact,
                              const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete given @p contacts from
     *        user's addressbook
     *
     * @param contacts Contacts to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const ContactsList &contacts,
                              const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete contact with given
     *        @p contactId from user's addressbook
     *
     * @param contactId ID of contact to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const QString &contactId,
                              const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete contacts with given
     *        @p contactsIds from user's addressbook
     *
     * @param contactsIds IDs of contacts to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const QStringList contactIds,
                              const AccountPtr &account, QObject* parent);

    /**
     * @brief Destructor
     */
    virtual ~ContactDeleteJob();

  protected:

    /**
     * @brief KMGraph::Job::start implementation
     */
    void start() override;

    /**
     * @brief KMGraph::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply *reply,
                             const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KMGraph2

#endif // LIBKMGRAPH2_CONTACTDELETEJOB_H
