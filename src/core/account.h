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
 * The class stores an access token, refresh token (to retrieve a new access token
 * when the old one expires) and list of scopes (APIs that current access token
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
     * \brief Sets new scopes.
     *
     * @note Note that changing scopes requires makes current tokens invalid.
     * This means that when this Account is used next time, AuthJob will be
     * automatically started and user will be prompted with a dialog to grant
     * access to all scopes.
     * 
     * TODO: AuthJob is no longer there, either rewrite the comment or clean up.
     * 
     * @param scopes
     */
    void setScopes(const QList< QUrl > &scopes);

    /**
     * Adds a single scope to account scopes.
     *
     * @param scope
     * @see Account::setScopes(const QList<QUrl>)
     */
    void addScope(const QUrl &scope);

    /**
     * Removes scope from the list.
     *
     * @param scope
     * @see Account::setScopes(const QList<QUrl>)
     */
    void removeScope(const QUrl &scope);

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

    /**
     * Returns scope URL for AccountInfo service.
     */
    static QUrl accountInfoScopeUrl();

    /**
     * Returns scope URL to retrieve AccountInfo with email.
     */
    static QUrl accountInfoEmailScopeUrl();

private:
    class Private;
    Private * const d;

    /**
     * @internal
     * Whether scopes were changed or not.
     *
     * AuthJob reads this attribute when Account is passed to it to
     * determine whether a completely new process of authentication is needed,
     * or whether just refreshing tokens is enough.
     *
     * When m_scopesChanged is \p true and AuthJob successfully performs full
     * re-authentication it sets this attribute to \p false and next time it
     * will just refresh existing tokens until the scopes are changed again.
     * 
     * TODO: AuthJob is no longer there, either rewrite the comment or clean up.
     * 
     */
    bool m_scopesChanged; //krazy:exclude=dpointer

};

} // namespace KMGraph2

Q_DECLARE_METATYPE(KMGraph2::AccountPtr)

#endif // LIBKMGRAPH2_ACCOUNT_H
