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

#ifndef LIBKMGRAPH2_CONTACTSSERVICE_H
#define LIBKMGRAPH2_CONTACTSSERVICE_H

#include "types.h"
#include "kmgraphcontacts_export.h"

#include <QUrl>

namespace KMGraph2
{

/**
 * @brief Additional methods for implementing support for Google Contacts service
 *
 * You should never need to use these methods, unless implementing your own Job
 *
 * There are no methods to serialize objects into JSON, because Google Contacts
 * API only supports XML for writing.
 */
namespace ContactsService
{

    /**
     * @brief Parses JSON data into Contact object
     *
     * @param jsonData
     */
    KMGRAPHCONTACTS_EXPORT ContactPtr JSONToContact(const QByteArray& jsonData);

    /**
     * @brief Parses JSON data into ContactsGroup object
     *
     * @param jsonData
     */
    KMGRAPHCONTACTS_EXPORT ContactsGroupPtr JSONToContactsGroup(const QByteArray& jsonData);

    /**
     * @brief Parses JSON feed into list of ContactsGroups or Events.
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional data
     */
    KMGRAPHCONTACTS_EXPORT ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Parses XML data into Contact object
     *
     * @param xmlData
     */
    KMGRAPHCONTACTS_EXPORT ContactPtr XMLToContact(const QByteArray& xmlData);

    /**
     * @brief Serializes a Contact object into XML
     *
     * @param contact
     */
    KMGRAPHCONTACTS_EXPORT QByteArray contactToXML(const ContactPtr& contact);

    /**
     * @brief Parses XML data into ContactsGroup object
     *
     * @param xmlData
     */
    KMGRAPHCONTACTS_EXPORT ContactsGroupPtr XMLToContactsGroup(const QByteArray& xmlData);

    /**
     * @brief Serializes a ContactsGroup object into XML
     *
     * @param group
     */
    KMGRAPHCONTACTS_EXPORT QByteArray contactsGroupToXML(const ContactsGroupPtr& group);

    /**
     * @brief Supported API version
     */
    KMGRAPHCONTACTS_EXPORT QString APIVersion();

    /**
     * @brief Returns URL for creating contacts
     *
     * @param user User. "default" or user@gmail.com}
     */
    KMGRAPHCONTACTS_EXPORT QUrl createContactUrl(const QString &user);

    /**
     * @brief Returns URL for fetching all contacts in addressbook
     *
     * @param user User. "default" or user@gmail.com
     * @param showDeleted Whether to fetch deleted contacts placeholders as well
     */
    KMGRAPHCONTACTS_EXPORT QUrl fetchAllContactsUrl(const QString &user, bool showDeleted);

    /**
     * @brief Returns URL for fetching specific contact
     *
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to fetch
     */
    KMGRAPHCONTACTS_EXPORT QUrl fetchContactUrl(const QString &user, const QString &contactID);

    /**
     * @brief Returns URL for updating a single contact
     *
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to update
     */
    KMGRAPHCONTACTS_EXPORT QUrl updateContactUrl(const QString &user, const QString &contactID);

    /**
     * @brief Returns URL for removing a single contact
     *
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to remove
     */
    KMGRAPHCONTACTS_EXPORT QUrl removeContactUrl(const QString &user, const QString &contactID);

    /**
     * Returns URL for KMGraph::Request::FetchAll requests.
     *
     * @param user User ("default" or user@gmail.com) in whose addressbook to look.
     */
    KMGRAPHCONTACTS_EXPORT QUrl fetchAllGroupsUrl(const QString &user);

    /**
     * @brief Returns URL for fetching all contacts groups
     *
     * @param user User ("default" or user@gmail.com) in whose addressbook to look
     * @param groupId ID of group of which to fetch all data.
     */
    KMGRAPHCONTACTS_EXPORT QUrl fetchGroupUrl(const QString &user, const QString &groupId);

    /**
     * @brief Returns URL for creating a new contacts group
     *
     * @param user User (user@gmail.com) in whose addressbook to create a group.
     */
    KMGRAPHCONTACTS_EXPORT QUrl createGroupUrl(const QString &user);

    /**
     * @brief Returns URL for updating a single contacts group
     *
     * @param user User (user@gmail.com)) in whose addressbook to look
     * @param groupId ID of group to update.
     */
    KMGRAPHCONTACTS_EXPORT QUrl updateGroupUrl(const QString &user, const QString &groupId);

    /**
     * @brief Returns URL for removing a single contacts group
     *
     * @param user User (user@gmail.com) in whose addressbook to look
     * @param groupId ID of group to delete
     */
    KMGRAPHCONTACTS_EXPORT QUrl removeGroupUrl(const QString &user, const QString &groupId);

    /**
     * @brief Returns URL for modifying photo.
     *
     * Send a GET request to fetch the photo, PUT to change it and DELETE to
     * remove it.
     *
     * @param user User (user@gmail.com) in whose addressbook to lok
     * @param contactId ID of contacts whose photo to update
     */
    KMGRAPHCONTACTS_EXPORT QUrl photoUrl(const QString &user, const QString &contactId);

} // namespace ContactsService

} // namespace KMGraph

#endif // LIBKMGRAPH2_CONTACTSSERVICE_H
