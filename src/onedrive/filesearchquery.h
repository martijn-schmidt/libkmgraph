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

#ifndef KMGRAPH2_ONEDRIVE_FILESEARCHQUERY_H
#define KMGRAPH2_ONEDRIVE_FILESEARCHQUERY_H

#include "kmgraphonedrive_export.h"

#include <QVariant>
#include <QSharedDataPointer>

namespace KMGraph2
{
namespace OneDrive
{

/**
 * FileSearchQuery class allows simply building even complex file search queries
 * for FileFetchJob.
 *
 * See https://developer.microsoft.com/en-us/graph/docs/api-reference/v1.0/api/driveitem_search for allowed
 * combinations of fields, compare operators, and value types.
 *
 * @since 2.3
 */
class KMGRAPHONEDRIVE_EXPORT FileSearchQuery
{
public:
    enum Field {
        Title,
        FullText,
        MimeType,
        ModifiedDate,
        LastViewedByMeDate,
        Trashed,
        Starred,
        Parents,
        Owners,
        Writers,
        Readers,
        SharedWithMe,
        /*Properties            FIXME: Not supported atm */
    };

    enum CompareOperator {
        Contains,
        Equals,
        NotEquals,
        Less,
        LessOrEqual,
        Greater,
        GreaterOrEqual,
        In,
        Has
    };

    enum LogicOperator {
        And,
        Or
    };

    explicit FileSearchQuery(LogicOperator op = And);
    FileSearchQuery(const FileSearchQuery &other);
    FileSearchQuery &operator=(const FileSearchQuery &other);
    ~FileSearchQuery();

    void addQuery(Field field, CompareOperator op, const QVariant &value);
    void addQuery(const FileSearchQuery &query);

    bool isEmpty() const;

    QString serialize() const;

  private:
    class Private;
    QSharedDataPointer<Private> d;
};
}
}

#endif // KMGRAPH2_ONEDRIVE_FILESEARCHQUERY_H
