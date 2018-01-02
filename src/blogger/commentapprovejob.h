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

#ifndef KMGRAPH2_BLOGGER_COMMENTAPPROVEJOB_H
#define KMGRAPH2_BLOGGER_COMMENTAPPROVEJOB_H

#include "modifyjob.h"
#include "kmgraphblogger_export.h"

namespace KMGraph2
{
namespace Blogger
{

class KMGRAPHBLOGGER_EXPORT CommentApproveJob : public KMGraph2::ModifyJob
{
    Q_OBJECT

  public:
    enum ApprovalAction {
        Approve,
        MarkAsSpam
    };
    explicit CommentApproveJob(const QString &blogId,
                               const QString &postId,
                               const QString &commentId,
                               ApprovalAction action,
                               const AccountPtr &account,
                               QObject *parent = nullptr);
    explicit CommentApproveJob(const CommentPtr &comment,
                               ApprovalAction action,
                               const AccountPtr &account,
                               QObject *parent = nullptr);
    virtual ~CommentApproveJob();

  protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KMGraph2

#endif // KMGRAPH2_BLOGGER_COMMENTAPPROVEJOB_H
