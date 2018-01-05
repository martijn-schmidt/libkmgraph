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

#include "job.h"
#include "job_p.h"
#include "account.h"

#include "../debug.h"


#include <QJsonDocument>

using namespace KMGraph2;


Job::Private::Private(Job *parent):
    isRunning(false),
    error(KMGraph2::NoError),
    accessManager(nullptr),
    maxTimeout(0),
    q(parent)
{
}

void Job::Private::init()
{
    QTimer::singleShot(0, q, [this]() { _k_doStart(); });

    accessManager = new KIO::Integration::AccessManager(q);
    connect(accessManager, &KIO::AccessManager::finished,
            q, [this](QNetworkReply *reply) { _k_replyReceived(reply); });

    dispatchTimer = new QTimer(q);
    connect(dispatchTimer, &QTimer::timeout,
            q, [this]() { _k_dispatchTimeout(); });
}

QString Job::Private::parseErrorMessage(const QByteArray &json)
{
    QJsonDocument document = QJsonDocument::fromJson(json);
    if (!document.isNull()) {
        QVariantMap map = document.toVariant().toMap();
        QString message;

        if (map.contains(QStringLiteral("error"))) {
            map = map.value(QStringLiteral("error")).toMap();
        }

        if (map.contains(QStringLiteral("message"))) {
            message.append(map.value(QStringLiteral("message")).toString());
        } else {
            message = QLatin1String(json);
        }

        return message;

    } else {
        return QLatin1String(json);
    }
}

void Job::Private::_k_doStart()
{
    isRunning = true;
    q->aboutToStart();
    q->start();
}

void Job::Private::_k_doEmitFinished()
{
    Q_EMIT q->finished(q);
}

void Job::Private::_k_replyReceived(QNetworkReply* reply)
{
    int replyCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (replyCode == 0) {

        /* Workaround for a bug (??), when QNetworkReply does not report HTTP/1.1 401 Unauthorized
         * as an error. */
        if (!reply->rawHeaderList().isEmpty()) {
            QString status = QLatin1String(reply->rawHeaderList().first());
            if (status.startsWith(QLatin1String("HTTP/1.1 401")))
                replyCode = KMGraph2::Unauthorized;
        }
    }

    const QByteArray rawData = reply->readAll();

    qCDebug(KMGraphDebug) << "Received reply from" << reply->url();
    qCDebug(KMGraphDebug) << "Status code: " << replyCode;
    qCDebug(KMGraphRaw) << rawData;

    switch (replyCode) {
        case KMGraph2::NoError:
        case KMGraph2::OK:           /** << OK status (fetched, updated, removed) */
        case KMGraph2::Created:      /** << OK status (created) */
        case KMGraph2::NoContent:    /** << OK status (removed file using OneDrive API) */
            break;

        case KMGraph2::TemporarilyMoved: {  /** << Temporarily moved - Microsoft Graph provides a new URL where to send the request */
            qCDebug(KMGraphDebug) << "Microsoft Graph says: Temporarily moved to " << reply->header(QNetworkRequest::LocationHeader).toUrl();
            QNetworkRequest request = reply->request();
            request.setUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
            q->enqueueRequest(request);
            return;
        }

        case KMGraph2::BadRequest: /** << Bad request - malformed data, API changed, something went wrong... */
            qCWarning(KMGraphDebug) << "Bad request, Microsoft Graph replied '" << rawData << "'";
            q->setError(KMGraph2::BadRequest);
            q->setErrorString(tr("Bad request."));
            q->emitFinished();
            return;

        case KMGraph2::Unauthorized: /** << Unauthorized - Access token has expired, request a new token */
            qCWarning(KMGraphDebug) << "Unauthorized. Access token has expired or is invalid.";
            q->setError(KMGraph2::Unauthorized);
            q->setErrorString(tr("Invalid authentication."));
            q->emitFinished();
            return;

        case KMGraph2::Forbidden: {
            qCWarning(KMGraphDebug) << "Requested resource is forbidden.";
            qCDebug(KMGraphRaw) << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KMGraph2::Forbidden);
            q->setErrorString(tr("Requested resource is forbidden.\n\nMicrosoft Graph replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }

        case KMGraph2::NotFound: {
            qCWarning(KMGraphDebug) << "Requested resource does not exist";
            qCDebug(KMGraphRaw) << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KMGraph2::NotFound);
            q->setErrorString(tr("Requested resource does not exist.\n\nMicrosoft Graph replied '%1'").arg(msg));
            // don't emit finished() here, we can get 404 when fetching contact photos or so,
            // in that case 404 is not fatal. Let subclass decide whether to terminate or not.
            q->handleReply(reply, rawData);

            if (requestQueue.isEmpty()) {
                q->emitFinished();
            }
            return;
        }

        case KMGraph2::Conflict: {
            qCWarning(KMGraphDebug) << "Conflict. Remote resource is newer then local.";
            qCDebug(KMGraphRaw) << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KMGraph2::Conflict);
            q->setErrorString(tr("Conflict. Remote resource is newer than local.\n\nMicrosoft Graph replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }

        case KMGraph2::Gone: {
            qCWarning(KMGraphDebug) << "Requested resource does not exist anymore.";
            qCDebug(KMGraphRaw) << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KMGraph2::Gone);
            q->setErrorString(tr("Requested resource does not exist anymore.\n\nMicrosoft Graph replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }

        case KMGraph2::InternalError: {
            qCWarning(KMGraphDebug) << "Internal server error.";
            qCDebug(KMGraphRaw) << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KMGraph2::InternalError);
            q->setErrorString(tr("Internal server error. Try again later.\n\nMicrosoft Graph replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }

        case KMGraph2::QuotaExceeded: {
            qCWarning(KMGraphDebug) << "User quota exceeded.";
            qCDebug(KMGraphRaw) << rawData;

            // Extend the interval (if possible) and enqueue the request again
            int interval = dispatchTimer->interval() / 1000;
            if (interval == 0) {
                interval = 1;
            } else if (interval == 1) {
                interval = 2;
            } else if ((interval > maxTimeout) && (maxTimeout > 0)) {
                const QString msg = parseErrorMessage(rawData);
                q->setError(KMGraph2::QuotaExceeded);
                q->setErrorString(tr("Maximum quota exceeded. Try again later.\n\nMicrosoft Graph replied '%1'").arg(msg));
                q->emitFinished();
                return;
            } else {
                interval = interval ^ 2;
            }
            qCDebug(KMGraphDebug) << "Increasing dispatch interval to" << interval * 1000 << "msecs";
            dispatchTimer->setInterval(interval * 1000);

            const QNetworkRequest request = reply->request();
            q->enqueueRequest(request);
            if (!dispatchTimer->isActive()) {
                dispatchTimer->start();
            }
            return;
        }

        default:{  /** Something went wrong, there's nothing we can do about it */
            qCWarning(KMGraphDebug) << "Unknown error" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qCDebug(KMGraphRaw) << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KMGraph2::UnknownError);
            q->setErrorString(tr("Unknown error.\n\nMicrosoft Graph replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }
    }

    q->handleReply(reply, rawData);

    // handleReply has terminated the job, don't continue
    if (!q->isRunning()) {
        return;
    }

    qCDebug(KMGraphDebug) << requestQueue.length() << "requests in requestQueue.";
    if (requestQueue.isEmpty()) {
        q->emitFinished();
        return;
    }

    if (!dispatchTimer->isActive()) {
        dispatchTimer->start();
    }
}

void Job::Private::_k_dispatchTimeout()
{
    if (requestQueue.isEmpty()) {
        dispatchTimer->stop();
        return;
    }

    const Request r = requestQueue.dequeue();
    currentRequest = r;

    qCDebug(KMGraphDebug) << q << "Dispatching request to" << r.request.url();
    qCDebug(KMGraphRaw) << r.rawData;

    q->dispatchRequest(accessManager, r.request, r.rawData, r.contentType);

    if (requestQueue.isEmpty()) {
        dispatchTimer->stop();
    }
}

/************************* PUBLIC **********************/

Job::Job(QObject* parent):
    QObject(parent),
    d(new Private(this))
{
    d->init();
}

Job::Job(const AccountPtr& account, QObject* parent):
    QObject(parent),
    d(new Private(this))
{
    d->account = account;

    d->init();
}


Job::~Job()
{
    delete d;
}

void Job::setError(Error error)
{
    d->error = error;
}

Error Job::error() const
{
    if (isRunning()) {
        qCWarning(KMGraphDebug) << "Called error() on running job, returning nothing";
        return KMGraph2::NoError;
    }

    return d->error;
}

void Job::setErrorString(const QString& errorString)
{
    d->errorString = errorString;
}

QString Job::errorString() const
{
    if (isRunning()) {
        qCWarning(KMGraphDebug) << "Called errorString() on running job, returning nothing";
        return QString();
    }

    return d->errorString;
}

bool Job::isRunning() const
{
    return d->isRunning;
}

int Job::maxTimeout() const
{
    return d->maxTimeout;
}

void Job::setMaxTimeout(int maxTimeout)
{
    if (isRunning()) {
        qCWarning(KMGraphDebug) << "Called setMaxTimeout() on running job. Ignoring.";
        return;
    }

    d->maxTimeout = maxTimeout;
}

AccountPtr Job::account() const
{
    return d->account;
}

void Job::setAccount(const AccountPtr& account)
{
    if (d->isRunning) {
        qCWarning(KMGraphDebug) << "Called setAccount() on running job. Ignoring.";
        return;
    }

    d->account = account;
}

void Job::restart()
{
    if (d->isRunning) {
        qCWarning(KMGraphDebug) << "Running job cannot be restarted.";
        return;
    }

    QTimer::singleShot(0, this, [this]() { d->_k_doStart();});
}

void Job::emitFinished()
{
    qCDebug(KMGraphDebug);
    aboutToFinish();

    d->isRunning = false;
    d->dispatchTimer->stop();
    d->requestQueue.clear();

    // Emit in next event loop iteration so that the method caller can finish
    // before user is notified
    QTimer::singleShot(0, this, [this]() { d->_k_doEmitFinished(); });
}

void Job::emitProgress(int processed, int total)
{
    Q_EMIT progress(this, processed, total);
}

void Job::enqueueRequest(const QNetworkRequest& request, const QByteArray& data, const QString& contentType)
{
    if (!isRunning()) {
        qCDebug(KMGraphDebug) << "Can't enqueue requests when job is not running.";
        qCDebug(KMGraphDebug) << "Not enqueueing" << request.url();
        return;
    }

    qCDebug(KMGraphDebug) << "Queued" << request.url();

    Request r_;
    r_.request = request;
    r_.rawData = data;
    r_.contentType = contentType;

    d->requestQueue.enqueue(r_);

    if (!d->dispatchTimer->isActive()) {
        d->dispatchTimer->start();
    }
}

void Job::aboutToFinish()
{
}

void Job::aboutToStart()
{
    d->error = KMGraph2::NoError;
    d->errorString.clear();
    d->currentRequest.contentType.clear();
    d->currentRequest.rawData.clear();
    d->currentRequest.request = QNetworkRequest();
    d->dispatchTimer->setInterval(0);
}

#include "moc_job.cpp"
#include "moc_job_p.cpp"
