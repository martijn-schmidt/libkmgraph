/*
    Copyright 2012, 2013  Daniel Vrátil <dvratil@redhat.com>

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

#include "account.h"

#include <QDateTime>

using namespace KMGraph2;

class Q_DECL_HIDDEN Account::Private
{
  public:
    Private();
    Private(const Private &other);

    QString accName;
    QString accessToken;
    QString refreshToken;
    QDateTime expireDateTime;
};

Account::Private::Private()
    : expireDateTime(QDateTime())
{ }


Account::Private::Private(const Private& other):
    accName(other.accName),
    accessToken(other.accessToken),
    refreshToken(other.refreshToken),
    expireDateTime(other.expireDateTime)
{ }


Account::Account():
    d(new Private)
{ }


Account::Account(const QString &accName, const QString &accessToken,
                 const QString &refreshToken):
    d(new Private)
{
    d->accName = accName;
    d->accessToken = accessToken;
    d->refreshToken = refreshToken;
}

Account::Account(const Account& other):
    d(new Private(*(other.d)))
{ }


Account::~Account()
{
    delete d;
}

QString Account::accountName() const
{
    return d->accName;
}

void Account::setAccountName(const QString& accountName)
{
    d->accName = accountName;
}

QString Account::accessToken() const
{
    return d->accessToken;
}

void Account::setAccessToken(const QString &accessToken)
{
    d->accessToken = accessToken;
}

QString Account::refreshToken() const
{
    return d->refreshToken;
}

void Account::setRefreshToken(const QString &refreshToken)
{
    d->refreshToken = refreshToken;
}

QDateTime Account::expireDateTime() const
{
    return d->expireDateTime;
}

void Account::setExpireDateTime(const QDateTime &expire)
{
    d->expireDateTime = expire;
}
