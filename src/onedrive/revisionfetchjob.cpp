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
#include "revisionfetchjob.h"
#include "account.h"
#include "onedriveservice.h"
#include "revision.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>


using namespace KMGraph2;
using namespace KMGraph2::OneDrive;

class Q_DECL_HIDDEN RevisionFetchJob::Private
{
  public:
    QString fileId;
    QString revisionId;
};

RevisionFetchJob::RevisionFetchJob(const QString &fileId,
                                   const AccountPtr &account,
                                   QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
}

RevisionFetchJob::RevisionFetchJob(const QString &fileId,
                                   const QString &revisionId,
                                   const AccountPtr &account,
                                   QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->revisionId = revisionId;
}

RevisionFetchJob::~RevisionFetchJob()
{
    delete d;
}

void RevisionFetchJob::start()
{
    QNetworkRequest request;
    if (d->revisionId.isEmpty()) {
        request.setUrl(OneDriveService::fetchRevisionsUrl(d->fileId));
    } else {
        request.setUrl(OneDriveService::fetchRevisionUrl(d->fileId, d->revisionId));
    }
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}

ObjectsList RevisionFetchJob::handleReplyWithItems(const QNetworkReply *reply,
        const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KMGraph2::JSON) {
        if (d->revisionId.isEmpty()) {
            items << Revision::fromJSONFeed(rawData);
        } else {
            items << Revision::fromJSON(rawData);
        }
    } else {
        setError(KMGraph2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
    }

    emitFinished();
    return items;
}


