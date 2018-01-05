/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

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

#ifndef LIBKMGRAPH2_ACCOUNT_H
#define LIBKMGRAPH2_ACCOUNT_H

#include <QString>
#include <QUrl>
#include <QMetaType>
#include <QSharedPointer>

#include "kmgraphcore_export.h"
#include "types.h"

namespace KMGraph2
{

/**
 * @headerfile Account
 * @brief A Microsoft account
 *
 * This class represents a single Microsoft account. The account is uniquely
 * identified by Account::accountName (which is actually the user's e-mail address).
 *
 * The class stores an access token, a refresh token (to retrieve a new access token
 * when the old one expires) and a list of scopes (APIs that the current access token
 * can be used to access).
 *
 * Unlike in previous versions, account management is not handled by LibKMGraph
 * anymore and it's up to programmer to store the account in a persistent storage.
 *
 * To obtain a new account, use KAccounts.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class KMGRAPHCORE_EXPORT Account
{

  public:
    /**
     * @brief Constructs an invalid account.
     */
    Account();

    /**
     * @brief Constructs a new valid account
     *
     * @param account Microsoft account name (usually user.name@subdomain.onmicrosoft.com)
     * @param accessToken Access token to \p scopes for \p account
     * @param refreshToken Refresh token
     * @param scopes List of scopes
     */
    explicit Account(const QString &account, const QString &accessToken = QString(),
                     const QString &refreshToken = QString(),
                     const QList< QUrl > &scopes = QList< QUrl >());

    /**
     * @brief Copy constructor
     */
    Account(const Account &other);

    /**
     * @brief Destructor
     */
    virtual ~Account();

    /**
     * @returns Returns unique account identifier
     */
    QString accountName() const;

    /**
     * Sets account name.
     *
     * @param accountName
     */
    void setAccountName(const QString &accountName);

    /**
     * @return Returns access token.
     */
    QString accessToken() const;

    /**
     * Sets a new access token.
     *
     * @param accessToken
     */
    void setAccessToken(const QString &accessToken);

    /**
     * @return Returns refresh token.
     */
    QString refreshToken() const;

    /**
     * Sets a new refresh token for the access token.
     *
     * @param refreshToken
     */
    void setRefreshToken(const QString &refreshToken);

    /**
     * @return Returns list of scopes the account is authenticated against.
     */
    QList< QUrl > scopes() const;

    /**
     * Adds a single scope to account scopes.
     *
     * @param scope
     * @see Account::setScopes(const QList<QUrl>)
     */
    void addScope(const QUrl &scope);

    /**
     * @since 2.0.82
     * Returns expire date time token
     */
    QDateTime expireDateTime() const;

    /**
     * @since 2.0.82
     * set expire date time
     */
    void setExpireDateTime(const QDateTime &expire);

private:
    class Private;
    Private * const d;

};

} // namespace KMGraph2

Q_DECLARE_METATYPE(KMGraph2::AccountPtr)

#endif // LIBKMGRAPH2_ACCOUNT_H
