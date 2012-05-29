/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGOOGLE_AUTH_H
#define LIBKGOOGLE_AUTH_H

#include <exception>

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtGui/qwindowdefs.h>

#include <libkgoogle/libkgoogle_export.h>
#include <libkgoogle/common.h>
#include <libkgoogle/account.h>

#include <kdemacros.h>

namespace KWallet
{
class Wallet;
}

class QNetworkReply;

namespace KGoogle
{

class AuthPrivate;
class AuthWidget;

/**
 * Auth provides API for authentication against Google services.
 *
 * The Auth object is a singleton.
 *
 * When created for the first time, the KWallet is opened asynchronously.
 * When the operation is finished, the signal KGoogle::Auth::ready() is
 * emitted. When Auth fails to open the KWallet, KGoogle::Auth::error() is
 * emitted respective error code.
 */
class LIBKGOOGLE_EXPORT Auth: public QObject
{

    Q_OBJECT

  public:
    /**
     * @return Returns single instance of KGoogle::Auth.
     */
    static Auth* instance();

    virtual ~Auth();

    /**
     * Initialize the authentication framework.
     *
     * Every application should call this before using Auth for the first
     * time.
     *
     * @param folder Default KWallet folder, @see setKWalletFolder()
     * @param apiClient Client's API key
     * @param apiSecret Client's sercret API key
     */
    void init(const QString &folder, const QString &apiKey, const QString &apiSecret);

    /**
     * Sets KWallet folder in which libkgoogle should store tokens
     *
     * When there is no folder set libKGoogle will use default "libkgoogle"
     * folder. There is no problem storing tokens in the "libkgoogle" folder but
     * remember that the tokens will then be shared across all applications that
     * won't set the folder either. It is strongly recommended for each application
     * to use it's own folder.
     *
     * @param folder Folder in KWallet in which tokens and accounts for current application
     *               should be stored. If the \p folder does not exist, it will be created.
     */
    void setKWalletFolder(const QString &folder);

    /**
     * Returns current KWallet folder.
     *
     * @return Returns current KWallet folder
     */
    QString kwalletFolder() const;

    /**
     * Returns API key set during module initialization.
     *
     * @return Returns client's API key.
     */
    const QString &apiKey() const;

    /**
     * Returns API secret key set during module initialization.
     *
     * @return Returns client's secret API key.
     */
    const QString &apiSecret() const;

    /**
     * Retrieves \p account from KWallet.
     *
     * It is up to user to delete the Account when no longer needed.
     *
     * Throwns KGoogle::Exception::BackendNotReady when KWallet is not opened
     * or connection fails for any reason. When account does not exist
     * KGoogle::Exception::UnknownAccount is thrown.
     *
     * @param account Name of account whose authentication to obtain
     * @return Returns KGoogle::Account or NULL when no such account is found in the
     *         KWallet or access to KWallet failed.
     */
    KGoogle::Account::Ptr getAccount(const QString &account);

    /**
     * Retrieves list of all accounts from KWallet.
     *
     * It's up to caller to delete all accounts in the list when no longer needed.
     *
     * Throws KGoogle::Exception::BackendNotReady when KWallet is not opened or
     * connection fails for any reason.
     *
     * @return Returns list of all Google accounts from KWallet.
     */
    QList< KGoogle::Account::Ptr > getAccounts();

    /**
     * Stores \p account in KWallet.
     *
     * When account with same name already exists in KWallet
     * it will be overwritten by the new \p account.
     *
     * The \p account must have name, refresh and access tokens
     * set, otherwise KGoogle::Exception::InvalidAccount is thrown
     * and nothing is stored, e.g. you are not allowed to store incomplete
     * or unauthenticated account.
     *
     * Throws KGoogle::Exception::BackendNotReady when KWallet is
     * not opened or connection fails for any reason.
     *
     * @param account Account to store in KWallet
     */
    void storeAccount(const KGoogle::Account::Ptr &account);

    /**
     * Authenticates \p account against Google services.
     *
     * When \p account contains no access or refresh token,
     * a complete authentication is performed (including dislaying
     * KGoogle::AuthDialog).
     *
     * When the \p account has both, access and refresh token set then
     * the refresh token is used to only obtain a new access token.
     *
     * The operations runs asynchronously. When finished,
     * KGoogle::Auth::authenticated() signal is emitted.
     * Upon error, KGoogle::Auth::error() signal is emitted.
     *
     * Throws KGoogle::Exception::InvalidAccount when \p account is not
     * valid or null. You are only allowed to pass an invalid \p account
     * (but not a null pointer!) if you want to run the full authentication,
     * otherwise at least name and scopes are required to be set.
     *
     * @param account Account whose token is to be refreshed.
     * @param autoSave Whether KGoogle::Auth should automatically
     *                 call KGoogle::Auth::storeAccount() to save changes
     *                 to KWallet.
     */
    void authenticate(KGoogle::Account::Ptr &account, bool autoSave = true);

    /**
     * @see authenticate()
     * @since 0.3.2
     *
     * Behaves exactly like Auth::authneticate(), but instead of displaying
     * a dialog for full authentication, the method returns a AuthWidget,
     * which can be simply embedded into an another application.
     *
     * If there is no need to display a widget (like only refreshing
     * authentication tokens), the method will return null and continue
     * like authenticate().
     *
     * To start the actual process of authentication, user must call
     * Kgoogle::AuthWidget::authneticate().
     *
     * It's up to user to hide and destroy the widget when the authentication
     * is finished (authneticated() or error() is emitted).
     *
     * @param account Account whose token is to be refreshed.
     * @param autoSave Whether KGoogle::Auth should automatically
     *                 call KGoogle::Auth::storeAccount() to save changes
     *                 to KWallet.
     * @return Returns a KGoogle::AuthWidget or null if there is no need for user interaction.
     */
    KGoogle::AuthWidget* authenticateWithWidget(KGoogle::Account::Ptr &account, bool autoSave = true);

    /**
     * Revokes tokens for \p account and removes it from KWallet.
     *
     * This method will not throw any exception when the account
     * is invalid or does not exist, only throws
     * KGoogle::Exception::BackendNotReady when KWallet is not opened
     * or connection fails (for any reason).
     *
     * @param account Account to which revoke access.
     * @return Returns \p true when account is successfully removed, \p false
     *         when account does not exist or something fails.
     */
    bool revoke(KGoogle::Account::Ptr &account);

    /**
     * Set the username that will be used when authenticate is called
     *
     * When \sa authenticate is called and the dialog to introduce the
     * credentials is open, the username input field will be automatically
     * filled with the string set here.
     *
     * Be aware that the username will be set every time \sa authenticate is
     * called so if you want to change or remove it call \sa setUsername again
     * with empty string or \sa clearCredentials.
     *
     * @param QString username to use
     */
    void setUsername(const QString &username);

    /**
     * Set the password that will be used when authenticate is called
     *
     * When \sa authenticate is called and the dialog to introduce the
     * credentials is open, the password input field will be automatically
     * filled with the string set here.
     *
     * Be aware that the password will be set every time \sa authenticate is
     * called so if you want to change or remove it call \sa setPassword again
     * with empty string or \sa clearCredentials.
     *
     * @param QString password to use
     */
    void setPassword(const QString &password);

    /**
     * Set whether to auto close the authentication dialog on success or error
     *
     * When \sa authenticate is called a dialog will appear to authenticate against
     * Google auth system, if set to true the dialog will be autoclosed in either
     * on success or on error.
     *
     * By default the value is true.
     *
     * As of 0.3.2 this method is deprecated and will be removed in 0.4 release.
     * If you want to prevent the dialog from closing, use your own
     * KDialog and use authenticateWithWidget() method to obtain an AuthWidget.
     *
     * @param bool Whether auto close or not the dialog
     */
    void KDE_DEPRECATED setDialogAutoClose(bool close);

    /**
     * Sets to empty username and password
     *
     * Sets to empty the username and the password which were set by
     * calling \sa setUsername and \sa setPassword.
     */
    void clearCredentials();

  Q_SIGNALS:
    /**
     * Emitted when an error occurs
     *
     * Signalizes any authentication error or when communication
     * with KWallet fails.
     */
    void error(KGoogle::Error errCode, QString msg);

    /**
     * Account was successfully (re-)authenticated.
     *
     * This signal is emitted when \p account was successfully authenticated
     * (for the first time), or when just tokens were refreshed.
     */
    void authenticated(KGoogle::Account::Ptr &account);

  private:
    AuthPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Auth)

    Auth();
    Auth(const Auth& other);
    Auth& operator=(const Auth &other);

};

} // namespace KGoogle

#endif // LIBKGOOGLE_AUTH_H
