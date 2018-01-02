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

#ifndef KMGRAPH2_BLOGGERSERVICE_H
#define KMGRAPH2_BLOGGERSERVICE_H

#include "types.h"
#include "kmgraphblogger_export.h"

#include <QUrl>

namespace KMGraph2
{

namespace BloggerService
{

    KMGRAPHBLOGGER_EXPORT QUrl fetchBlogByBlogIdUrl(const QString &blogId);
    KMGRAPHBLOGGER_EXPORT QUrl fetchBlogByBlogUrlUrl(const QString &blogUrl);
    KMGRAPHBLOGGER_EXPORT QUrl fetchBlogsByUserIdUrl(const QString &userId);

    KMGRAPHBLOGGER_EXPORT QUrl fetchCommentsUrl(const QString &blogId,
                                           const QString &postId = QString(),
                                           const QString &commentId = QString());

    KMGRAPHBLOGGER_EXPORT QUrl approveCommentUrl(const QString &blogId,
                                            const QString &postId,
                                            const QString &commentId);
    KMGRAPHBLOGGER_EXPORT QUrl markCommentAsSpamUrl(const QString &blogId,
                                               const QString &postId,
                                               const QString &commentId);
    KMGRAPHBLOGGER_EXPORT QUrl deleteCommentUrl(const QString &blogId,
                                           const QString &postId,
                                           const QString &commentId);
    KMGRAPHBLOGGER_EXPORT QUrl deleteCommentContentUrl(const QString &blogId,
                                                  const QString &postId,
                                                  const QString &commentId);

    KMGRAPHBLOGGER_EXPORT QUrl fetchPageUrl(const QString &blogId,
                                       const QString &pageId);
    KMGRAPHBLOGGER_EXPORT QUrl deletePageUrl(const QString &blogId,
                                        const QString &pageId);
    KMGRAPHBLOGGER_EXPORT QUrl modifyPageUrl(const QString &blogId,
                                        const QString &pageId);
    KMGRAPHBLOGGER_EXPORT QUrl createPageUrl(const QString &blogId);

    KMGRAPHBLOGGER_EXPORT QUrl fetchPostUrl(const QString &blogId,
                                       const QString &postId);
    KMGRAPHBLOGGER_EXPORT QUrl searchPostUrl(const QString &blogId);
    KMGRAPHBLOGGER_EXPORT QUrl createPostUrl(const QString &blogId);
    KMGRAPHBLOGGER_EXPORT QUrl deletePostUrl(const QString &blogId,
                                        const QString &postId);
    KMGRAPHBLOGGER_EXPORT QUrl modifyPostUrl(const QString &blogId,
                                        const QString &postId);
    KMGRAPHBLOGGER_EXPORT QUrl publishPostUrl(const QString &blogId,
                                         const QString &postId);
    KMGRAPHBLOGGER_EXPORT QUrl revertPostUrl(const QString &blogId,
                                        const QString &postId);

} // namespace BloggerService
} // namespace KMGraph2

#endif // KMGRAPH2_BLOGGERSERVICE_H
