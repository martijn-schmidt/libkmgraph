/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "parentreference.h"
#include "parentreference_p.h"

#include "../debug.h"

#include <QVariantMap>
#include <QJsonDocument>

using namespace KMGraph2;
using namespace KMGraph2::OneDrive;

ParentReference::Private::Private():
    isRoot(false)
{
}

ParentReference::Private::Private(const Private &other):
    id(other.id),
    selfLink(other.selfLink),
    parentLink(other.parentLink),
    isRoot(other.isRoot)
{
}

ParentReferencePtr ParentReference::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QStringLiteral("kind")) ||
            map[QStringLiteral("kind")].toString() != QLatin1String("drive#parentReference")) {
        return ParentReferencePtr();
    }

    ParentReferencePtr reference(new ParentReference(map[QStringLiteral("id")].toString()));
    reference->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    reference->d->parentLink = map[QStringLiteral("parentLink")].toUrl();
    reference->d->isRoot = map[QStringLiteral("isRoot")].toBool();

    return reference;
}

QVariantMap ParentReference::Private::toJSON(const ParentReferencePtr &reference)
{
    QVariantMap map;
    if (!reference->d->id.isEmpty()) {
        map[QStringLiteral("id")] = reference->id();
    }
    if (!reference->d->selfLink.isEmpty()) {
        map[QStringLiteral("selfLink")] = reference->d->selfLink;
    }
    if (!reference->d->parentLink.isEmpty()) {
        map[QStringLiteral("parentLink")] = reference->d->parentLink;
    }
    if (reference->d->isRoot) { // default is false
        map[QStringLiteral("isRoot")] = reference->d->isRoot;
    }

    return map;
}

ParentReference::ParentReference(const QString &id):
    KMGraph2::Object(),
    d(new Private)
{
    d->id = id;
}

ParentReference::ParentReference(const ParentReference &other):
    KMGraph2::Object(other),
    d(new Private(*(other.d)))
{
}

ParentReference::~ParentReference()
{
    delete d;
}

QString ParentReference::id() const
{
    return d->id;
}

QUrl ParentReference::selfLink() const
{
    return d->selfLink;
}

QUrl ParentReference::parentLink() const
{
    return d->parentLink;
}

bool ParentReference::isRoot() const
{
    return d->isRoot;
}

ParentReferencePtr ParentReference::fromJSON(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return ParentReferencePtr();
    }

    const QVariant data = document.toVariant();
    return Private::fromJSON(data.toMap());
}

ParentReferencesList ParentReference::fromJSONFeed(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    qCDebug(KMGraphDebug) << "Dumping the content of document:" << document;
    if (document.isNull()) {
        return ParentReferencesList();
    }

    const QVariant data = document.toVariant();
    qCDebug(KMGraphDebug) << "Dumping the content of data:" << data;
    const QVariantMap map = data.toMap();
    qCDebug(KMGraphDebug) << "Dumping the content of map:" << map;
    if (!map.contains(QStringLiteral("kind")) ||
            map[QStringLiteral("kind")].toString() != QLatin1String("kind#parentList")) {
        return ParentReferencesList();
    }

    ParentReferencesList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    qCDebug(KMGraphDebug) << "Dumping the content of items:" << items;
    for (const QVariant & item : items) {
        const ParentReferencePtr reference = Private::fromJSON(item.toMap());

        if (!reference.isNull()) {
            list << reference;
        }
    }
    qCDebug(KMGraphDebug) << "Dumping the content of list:" << list;
    return list;
}

QByteArray ParentReference::toJSON(const ParentReferencePtr &reference)
{
    const QVariantMap map = Private::toJSON(reference);

    QJsonDocument document = QJsonDocument::fromVariant(map);
    return document.toJson(QJsonDocument::Compact);
}
