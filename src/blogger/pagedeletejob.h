/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
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

#ifndef KMGRAPH2_BLOGGER_PAGEDELETEJOB_H
#define KMGRAPH2_BLOGGER_PAGEDELETEJOB_H

#include "deletejob.h"
#include "kmgraphblogger_export.h"

namespace KMGraph2
{
namespace Blogger
{

class KMGRAPHBLOGGER_EXPORT PageDeleteJob : public KMGraph2::DeleteJob
{
    Q_OBJECT

  public:
    explicit PageDeleteJob(const PagePtr &page,
                           const AccountPtr &account,
                           QObject *parent = nullptr);
    explicit PageDeleteJob(const QString &blogId,
                           const QString &pageId,
                           const AccountPtr &account,
                           QObject *parent = nullptr );
    virtual ~PageDeleteJob();

  protected:
    void start() override;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KMGRAPH2_BLOGGER_PAGEDELETEJOB_H
