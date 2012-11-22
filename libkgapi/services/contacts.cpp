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

#include "contacts.h"
#include "accessmanager.h"
#include "objects/contact.h"
#include "objects/contactsgroup.h"

#include <kurl.h>

/* Qt::escape() */
#include <QTextDocument>

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace KGAPI
{

namespace Services
{

class ContactsPrivate
{

public:
    static QString stringFromXMLMap(const QVariantMap &map, const QString &key) {
	const QVariantMap t = map.value(key).toMap();
	return t.value(QLatin1String("$t")).toString();
    }

    static KGAPI::Object* JSONToContact(const QVariantMap &data);
    static KGAPI::Object* JSONToGroup(const QVariantMap &data);

    static QByteArray contactToXML(const KGAPI::Object *object);
    static QByteArray groupToXML(const KGAPI::Object *object);

    static KGAPI::Object* XMLToContact(const QDomDocument &data);
    static KGAPI::Object* XMLToGroup(const QDomDocument &data);
};

}

}

using namespace KGAPI;
using namespace KGAPI::Services;

QUrl Contacts::ScopeUrl(QLatin1String("https://www.google.com/m8/feeds/"));

static const QString serviceNameStr = QLatin1String("KGAPI::Services::Contacts");


const QString& Contacts::serviceName()
{
    if (QMetaType::type(serviceNameStr.toLatin1().constData()) == 0) {
        qRegisterMetaType< KGAPI::Services::Contacts >(serviceNameStr.toLatin1().constData());
    }

    return serviceNameStr;
}

QByteArray Contacts::objectToJSON(KGAPI::Object* object)
{
    /* Google allows writing only XML data */
    return QByteArray();

    Q_UNUSED(object)
}

QByteArray Contacts::objectToXML(KGAPI::Object* object)
{
    if (!object) {
        return QByteArray();
    }

    if (dynamic_cast< KGAPI::Objects::Contact* >(object)) {
        return ContactsPrivate::contactToXML(object);
    } else if (dynamic_cast< KGAPI::Objects::ContactsGroup* >(object)) {
        return ContactsPrivate::groupToXML(object);
    } else {
        return QByteArray();
    }
}


QList< KGAPI::Object* > Contacts::parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QList< KGAPI::Object* > output;
    QJson::Parser parser;

    const QVariantMap head = parser.parse(jsonFeed).toMap();
    const QVariantMap feed = head.value(QLatin1String("feed")).toMap();
    const QVariantList categories = feed.value(QLatin1String("category")).toList();
    Q_FOREACH(const QVariant &c, categories) {
        const QVariantMap category = c.toMap();
        bool groups = false;

	if (category.value(QLatin1String("term")).toString() == QLatin1String("http://schemas.google.com/contact/2008#group")) {
	    groups = true;
	}

        const QVariantList entries = feed.value(QLatin1String("entry")).toList();
        Q_FOREACH(const QVariant &e, entries) {
            if (groups) {
                output << ContactsPrivate::JSONToGroup(e.toMap());
	    } else {
                output << ContactsPrivate::JSONToContact(e.toMap());
	    }
        }
    }

    const QVariantList links = feed.value(QLatin1String("link")).toList();
    Q_FOREACH(const QVariant &l, links) {
        const QVariantMap link = l.toMap();
        if (link.value(QLatin1String("rel")).toString() == QLatin1String("next")) {
            feedData.nextPageUrl = KUrl(link.value(QLatin1String("href")).toString());
            break;
        }
    }

    feedData.totalResults = ContactsPrivate::stringFromXMLMap(feed, QLatin1String("openSearch$totalResults")).toInt();
    feedData.startIndex = ContactsPrivate::stringFromXMLMap(feed, QLatin1String("openSearch$startIndex")).toInt();
    feedData.itemsPerPage = ContactsPrivate::stringFromXMLMap(feed, QLatin1String("openSearch$itemsPerPage")).toInt();

    return output;
}

Object *Contacts::JSONToObject(const QByteArray &jsonData)
{
    QJson::Parser parser;

    const QVariantMap data = parser.parse(jsonData).toMap();
    const QVariantMap entry = data.value(QLatin1String("entry")).toMap();
    const QVariantList categories = entry.value(QLatin1String("category")).toList();

    Q_FOREACH(const QVariant &c, categories) {
        const QVariantMap category = c.toMap();

        if (category.value(QLatin1String("term")).toString() == QLatin1String("http://schemas.google.com/contact/2008#group")) {
            return ContactsPrivate::JSONToGroup(entry);
        }

        if (category.value(QLatin1String("term")).toString() == QLatin1String("http://schemas.google.com/contact/2008#contact")) {
            return ContactsPrivate::JSONToContact(entry);
        }
    }

    return 0;
}

KGAPI::Object* Contacts::XMLToObject(const QByteArray& xmlData)
{
    QByteArray xmlDoc;
    /* Document without <xml> header is not valid and Qt won't parse it */
    if (!xmlData.contains("<?xml")) {
        xmlDoc.append("<?xml version='1.0' encoding='UTF-8'?>");
    }
    xmlDoc.append(xmlData);

    QDomDocument doc;
    doc.setContent(xmlDoc);
    const QDomNodeList entry = doc.elementsByTagName(QLatin1String("entry"));
    QDomNodeList data;
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return 0;
    }

    bool isGroup = false;

    for (int i = 0; i < data.count(); i++) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (((e.tagName() == QLatin1String("category")) &&
	     (e.attribute(QLatin1String("term")) == QLatin1String("http://schemas.google.com/contact/2008#group"))) ||
            ((e.tagName() == QLatin1String("atom:category")) &&
	     (e.attribute(QLatin1String("term")) == QLatin1String("http://schemas.google.com/g/2005#group"))))
	{
            isGroup = true;
            break;
        }
    }

    if (isGroup) {
        return ContactsPrivate::XMLToGroup(doc);
    } else {
        return ContactsPrivate::XMLToContact(doc);
    }

}

QList< KGAPI::Object* > Contacts::parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object* >();
}

const QUrl& Contacts::scopeUrl() const
{
    return Contacts::ScopeUrl;
}

QUrl Contacts::fetchAllContactsUrl(const QString& user, const bool &showDeleted)
{
    KUrl url("https://www.google.com/m8/feeds/contacts/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));
    url.addQueryItem(QLatin1String("alt"), QLatin1String("json"));
    if (showDeleted) {
        url.addQueryItem(QLatin1String("showdeleted"), QLatin1String("true"));
    }

    return url;
}

QUrl Contacts::fetchContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains(QLatin1Char('/'))) {
        id = contactID.mid(contactID.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = contactID;
    }

    KUrl url("https://www.google.com/m8/feeds/contacts/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));
    url.addPath(id);
    url.addQueryItem(QLatin1String("alt"), QLatin1String("json"));

    return url;
}

QUrl Contacts::createContactUrl(const QString& user)
{
    KUrl url("https://www.google.com/m8/feeds/contacts/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));

    return url;
}

QUrl Contacts::updateContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains(QLatin1Char('/'))) {
        id = contactID.mid(contactID.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = contactID;
    }

    KUrl url("https://www.google.com/m8/feeds/contacts/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));
    url.addPath(id);

    return url;
}

QUrl Contacts::removeContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains(QLatin1Char('/'))) {
        id = contactID.mid(contactID.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = contactID;
    }

    KUrl url("https://www.google.com/m8/feeds/contacts/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));
    url.addPath(id);

    return url;
}

QUrl Contacts::fetchAllGroupsUrl(const QString &user)
{
    KUrl url("https://www.google.com/m8/feeds/groups/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));
    url.addQueryItem(QLatin1String("alt"), QLatin1String("json"));

    return url;
}

QUrl Contacts::fetchGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains(QLatin1Char('/'))) {
        id = groupId.mid(groupId.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = groupId;
    }

    KUrl url("https://www.google.com/m8/feeds/groups/");
    url.addPath(user);
    url.addPath(QLatin1String("base"));
    url.addPath(id);
    url.addQueryItem(QLatin1String("alt"), QLatin1String("json"));

    return url;
}

QUrl Contacts::createGroupUrl(const QString &user)
{
    KUrl url("https://www.google.com/m8/feeds/groups/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));

    return url;
}

QUrl Contacts::updateGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains(QLatin1Char('/'))) {
        id = groupId.mid(groupId.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = groupId;
    }

    KUrl url("https://www.google.com/m8/feeds/groups/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));
    url.addPath(id);

    return url;
}

QUrl Contacts::removeGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains(QLatin1Char('/'))) {
        id = groupId.mid(groupId.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = groupId;
    }

    KUrl url("https://www.google.com/m8/feeds/groups/");
    url.addPath(user);
    url.addPath(QLatin1String("full"));
    url.addPath(id);

    return url;
}

QUrl Contacts::photoUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains(QLatin1Char('/'))) {
        id = contactID.mid(contactID.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = contactID;
    }

    KUrl url("https://www.google.com/m8/feeds/photos/media/");
    url.addPath(user);
    url.addPath(id);

    return url;
}


QString Contacts::protocolVersion() const
{
    return QLatin1String("3.0");
}

bool Contacts::supportsJSONRead(QString* urlParam)
{
    if (urlParam)
        *urlParam = QLatin1String("json");

    return true;
}

bool Contacts::supportsJSONWrite(QString* urlParam)
{
    Q_UNUSED(urlParam)

    return false;
}



/*********************************** PRIVATE *************************************/

KGAPI::Object* ContactsPrivate::JSONToGroup(const QVariantMap &data)
{
    Objects::ContactsGroup *object = new Objects::ContactsGroup;

    object->setId(stringFromXMLMap(data, QLatin1String("id")));
    object->setEtag(data.value(QLatin1String("gd$etag")).toString());

    object->setTitle(stringFromXMLMap(data, QLatin1String("title")));
    object->setContent(stringFromXMLMap(data, QLatin1String("content")));

    object->setUpdated(AccessManager::RFC3339StringToDate(stringFromXMLMap(data, QLatin1String("updated"))).dateTime());

    if (data.contains(QLatin1String("gContact$systemGroup"))) {
        object->setIsSystemGroup(true);
    } else {
        object->setIsSystemGroup(false);
    }

    return object;
}


KGAPI::Object* ContactsPrivate::JSONToContact(const QVariantMap &data)
{
    Objects::Contact *object = new Objects::Contact();

    /* Google contact ID */
    object->setUid(stringFromXMLMap(data, QLatin1String("id")));

    /* Google ETAG. This can be used to identify if the item was changed remotly */
    object->setEtag(data.value(QLatin1String("gd$etag")).toString());

    /* Date and time when contact was updated on the remote server */
    object->setUpdated(AccessManager::RFC3339StringToDate(stringFromXMLMap(data, QLatin1String("updated"))));

    /* If the contact was deleted, we don't need more info about it.
     * Just store our own flag, which will be then parsed by the resource
     * itself. */
    object->setDeleted(data.value(QLatin1String("gd$deleted")).isValid());

    /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture
     * itself. */
    const QVariantList links = data.value(QLatin1String("link")).toList();
    Q_FOREACH(const QVariant &link, links) {
	const QVariantMap linkMap = link.toMap();
	if (linkMap.value(QLatin1String("rel")).toString() == QLatin1String("http://schemas.google.com/contacts/2008/rel#photo")) {
            object->setPhotoUrl(linkMap.value(QLatin1String("href")).toString());
        }
    }

    /* Name */
    if (data.contains(QLatin1String("title"))) {
        object->setName(stringFromXMLMap(data, QLatin1String("title")));
    }

    /* Formatted name */
    if (data.contains(QLatin1String("gd$name"))) {
        const QVariantMap name = data.value(QLatin1String("gd$name")).toMap();

        if (name.contains(QLatin1String("gd$fullName"))) {
            object->setFormattedName(stringFromXMLMap(name, QLatin1String("gd$fullName")));
	}
        if (name.contains(QLatin1String("gd$givenName"))) {
            object->setGivenName(stringFromXMLMap(name, QLatin1String("gd$givenName")));
	}
        if (name.contains(QLatin1String("gd$familyName"))) {
            object->setFamilyName(stringFromXMLMap(name, QLatin1String("gd$familyName")));
	}
        if (name.contains(QLatin1String("gd$additionalName"))) {
            object->setAdditionalName(stringFromXMLMap(name, QLatin1String("gd$additionalName")));
	}
        if (name.contains(QLatin1String("gd$namePrefix"))) {
            object->setPrefix(stringFromXMLMap(name, QLatin1String("gd$namePrefix")));
	}
        if (name.contains(QLatin1String("gd$nameSuffix"))) {
            object->setSuffix(stringFromXMLMap(name, QLatin1String("gd$nameSuffix")));
	}
    }

    /* Note */
    if (data.contains(QLatin1String("content"))) {
	object->setNote(ContactsPrivate::stringFromXMLMap(data, QLatin1String("content")));
    }

    /* Organization (work) - KABC supports only one organization */
    if (data.contains(QLatin1String("gd$organization"))) {
	const QVariantList organizations = data.value(QLatin1String("gd$organization")).toList();
        const QVariantMap organization = organizations.first().toMap();

        if (organization.contains(QLatin1String("gd$orgName"))) {
	    object->setOrganization(stringFromXMLMap(organization, QLatin1String("gd$orgName")));
	}
        if (organization.contains(QLatin1String("gd$orgDepartment"))) {
	    object->setDepartment(stringFromXMLMap(organization, QLatin1String("gd$orgDepartment")));
	}
        if (organization.contains(QLatin1String("gd$orgTitle"))) {
            object->setTitle(stringFromXMLMap(organization, QLatin1String("gd$orgTitle")));
	}
        if (organization.contains(QLatin1String("gd$where"))) {
            object->setOffice(stringFromXMLMap(organization, QLatin1String("gd$where")));
	}
    }

    /* Nickname */
    if (data.contains(QLatin1String("gContact$nickname"))) {
	object->setNickName(stringFromXMLMap(data, QLatin1String("gContact$nickname")));
    }

    /* Occupation (= organization/title) */
    if (data.contains(QLatin1String("gContact$occupation"))) {
        object->setProfession(stringFromXMLMap(data, QLatin1String("gContact$occupation")));
    }

    /* Relationships */
    if (data.contains(QLatin1String("gContact$relation"))) {
	const QVariantList relations = data.value(QLatin1String("gContact$relation")).toList();
        Q_FOREACH(const QVariant &r, relations) {
            const QVariantMap relation = r.toMap();
            if (relation.value(QLatin1String("rel")).toString() == QLatin1String("spouse")) {
                object->setSpousesName(relation.value(QLatin1String("$t")).toString());
                continue;
            }

            if (relation.value(QLatin1String("rel")).toString() == QLatin1String("manager")) {
                object->setManagersName(relation.value(QLatin1String("$t")).toString());
                continue;
            }

            if (relation.value(QLatin1String("rel")).toString() == QLatin1String("assistant")) {
                object->setAssistantsName(relation.value(QLatin1String("$t")).toString());
                continue;
            }
        }
    }

    /* Anniversary */
    if (data.contains(QLatin1String("gContact$event"))) {
	const QVariantList events = data.value(QLatin1String("gContact$event")).toList();
        Q_FOREACH(const QVariant &e, events) {
            const QVariantMap event = e.toMap();

            if (event.value(QLatin1String("rel")).toString() == QLatin1String("anniversary")) {
                QVariantMap when = event.value(QLatin1String("gd$when")).toMap();
                object->setAnniversary(when.value(QLatin1String("startTime")).toString());
            }
        }
    }

    /* Websites */
    if (data.contains(QLatin1String("gContact$website"))) {
	const QVariantList websites = data.value(QLatin1String("gContact$website")).toList();
        Q_FOREACH(const QVariant &w, websites) {
            const QVariantMap web = w.toMap();

            if (web.value(QLatin1String("rel")).toString() == QLatin1String("home-page")) {
                object->setUrl(KUrl(web.value(QLatin1String("href")).toString()));
                continue;
            }

            if (web.value(QLatin1String("rel")).toString() == QLatin1String("blog")) {
                object->setBlogFeed(web.value(QLatin1String("href")).toString());
                continue;
            }
        }
    }

    /* Emails */
    const QVariantList emails = data.value(QLatin1String("gd$email")).toList();
    Q_FOREACH(const QVariant & em, emails) {
        const QVariantMap email = em.toMap();
        object->insertEmail(email.value(QLatin1String("address")).toString(),
			    email.value(QLatin1String("primary")).toBool());
    }

    /* IMs */
    const QVariantList ims = data.value(QLatin1String("gd$im")).toList();
    Q_FOREACH(const QVariant & i, ims) {
        const QVariantMap im = i.toMap();
	const QString protocol = Objects::Contact::IMSchemeToProtocolName(im.value(QLatin1String("protocol")).toString());
        object->insertCustom(QLatin1String("messaging/") + protocol,
			     QLatin1String("All"),
			     im.value(QLatin1String("address")).toString());
    }

    /* Phone numbers */
    const QVariantList phones = data.value(QLatin1String("gd$phoneNumber")).toList();
    Q_FOREACH(const QVariant & p, phones) {
        const QVariantMap phone = p.toMap();
        object->insertPhoneNumber(KABC::PhoneNumber(phone.value(QLatin1String("$t")).toString(),
                                  Objects::Contact::phoneSchemeToType(phone.value(QLatin1String("rel")).toString())));
    }

    /* Addresses */
    const QVariantList addresses = data.value(QLatin1String("gd$structuredPostalAddress")).toList();
    Q_FOREACH(const QVariant & a, addresses) {
        const QVariantMap address = a.toMap();
        KABC::Address addr;
        if (!address.contains(QLatin1String("gd$city")) &&
                !address.contains(QLatin1String("gd$country")) &&
                !address.contains(QLatin1String("gd$postcode")) &&
                !address.contains(QLatin1String("gd$region")) &&
                !address.contains(QLatin1String("gd$pobox")))
	{
            addr.setExtended(stringFromXMLMap(address, QLatin1String("gd$street")));
        } else {
            if (address.contains(QLatin1String("gd$street"))) {
                addr.setStreet(stringFromXMLMap(address, QLatin1String("gd$street")));
	    }
            if (address.contains(QLatin1String("gd$country"))) {
                addr.setCountry(stringFromXMLMap(address, QLatin1String("gd$country")));
	    }
            if (address.contains(QLatin1String("gd$city"))) {
                addr.setLocality(stringFromXMLMap(address, QLatin1String("gd$city")));
	    }
            if (address.contains(QLatin1String("gd$postcode"))) {
                addr.setPostalCode(stringFromXMLMap(address, QLatin1String("gd$postcode")));
	    }
            if (address.contains(QLatin1String("gdregion"))) {
                addr.setRegion(stringFromXMLMap(address, QLatin1String("gd$region")));
	    }
            if (address.contains(QLatin1String("gd$pobox"))) {
                addr.setPostOfficeBox(stringFromXMLMap(address, QLatin1String("gd$pobox")));
	    }
        }
        addr.setType(Objects::Contact::addressSchemeToType(address.value(QLatin1String("rel")).toString()));

        object->insertAddress(addr);
    }

    /* Birthday */
    const QVariantMap bDay = data.value(QLatin1String("gContact$birthday")).toMap();
    if (!bDay.isEmpty()) {
        object->setBirthday(QDateTime::fromString(bDay.value(QLatin1String("when")).toString(), QLatin1String("yyyy-MM-dd")));
    }

    /* User-defined fields */
    const QVariantList userDefined = data.value(QLatin1String("gContact$userDefinedField")).toList();
    Q_FOREACH(const QVariant & u, userDefined) {
        const QVariantMap field = u.toMap();
        object->insertCustom(QLatin1String("GCALENDAR"),
			     field.value(QLatin1String("key")).toString(),
			     field.value(QLatin1String("value")).toString());
    }

    /* Groups */
    const QVariantList groups = data.value(QLatin1String("gContact$groupMembershipInfo")).toList();
    QStringList groupsList;
    Q_FOREACH(const QVariant & g, groups) {
        const QVariantMap group = g.toMap();
	if (group.value(QLatin1String("deleted")).toBool() == false) {
            groupsList.append(group.value(QLatin1String("href")).toString());
	}
    }
    object->insertCustom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo"),
			 groupsList.join(QLatin1String(",")));

    return dynamic_cast< KGAPI::Object* >(object);
}


QByteArray ContactsPrivate::contactToXML(const KGAPI::Object* object)
{
    const Objects::Contact *contact = static_cast< const Objects::Contact* >(object);

    QByteArray output;

    QStringList parsedCustoms;

    /* Name */
    output.append("<gd:name>");
    if (!contact->givenName().isEmpty()) {
        output.append("<gd:givenName>").append(Qt::escape(contact->givenName()).toUtf8()).append("</gd:givenName>");
    }
    if (!contact->familyName().isEmpty()) {
        output.append("<gd:familyName>").append(Qt::escape(contact->familyName()).toUtf8()).append("</gd:familyName>");
    }
    if (!contact->assembledName().isEmpty()) {
        output.append("<gd:fullName>").append(Qt::escape(contact->formattedName()).toUtf8()).append("</gd:fullName>");
    }
    if (!contact->additionalName().isEmpty()) {
        output.append("<gd:additionalName>").append(Qt::escape(contact->additionalName()).toUtf8()).append("</gd:additionalName>");
    }
    if (!contact->prefix().isEmpty()) {
        output.append("<gd:namePrefix>").append(Qt::escape(contact->prefix()).toUtf8()).append("</gd:namePrefix>");
    }
    if (!contact->suffix().isEmpty()) {
        output.append("<gd:nameSuffix>").append(Qt::escape(contact->suffix()).toUtf8()).append("</gd:nameSuffix>");
    }
    output.append("</gd:name>");

    /* Notes */
    if (!contact->note().isEmpty()) {
        output.append("<atom:content type='text'>").append(Qt::escape(contact->note()).toUtf8()).append("</atom:content>");
    }

    /* Organization (work) */
    QByteArray org;
    const QString office = contact->office();
    if (!contact->organization().isEmpty()) {
        org.append("<gd:orgName>").append(Qt::escape(contact->organization()).toUtf8()).append("</gd:orgName>");
    }
    if (!contact->department().isEmpty()) {
        org.append("<gd:orgDepartment>").append(Qt::escape(contact->department()).toUtf8()).append("</gd:orgDepartment>");
    }
    if (!contact->title().isEmpty()) {
        org.append("<gd:orgTitle>").append(Qt::escape(contact->title()).toUtf8()).append("</gd:orgTitle>");
    }
    if (!office.isEmpty()) {
        org.append("<gd:where>").append(Qt::escape(office).toUtf8()).append("</gd:where>");
        parsedCustoms << QLatin1String("KADDRESSBOOK-X-Office");
    }
    if (!org.isEmpty()) {
        output.append("<gd:organization rel=\"http://schemas.google.com/g/2005#work\">").append(org).append("</gd:organization>");
    }

    /* Nickname */
    if (!contact->nickName().isEmpty()) {
        output.append("<gContact:nickname>").append(Qt::escape(contact->nickName()).toUtf8()).append("</gContact:nickname>");
    }

    /* Occupation */
    if (!contact->profession().isEmpty()) {
        output.append("<gContact:occupation>").append(Qt::escape(contact->profession()).toUtf8()).append("</gContact:occupation>");
        parsedCustoms << QLatin1String("KADDRESSBOOK-X-Profession");
    }

    /* Spouse */
    const QString spouse = contact->spousesName();
    if (!spouse.isEmpty()) {
        output.append("<gContact:relation rel=\"spouse\">").append(Qt::escape(spouse).toUtf8()).append("</gContact:relation>");
        parsedCustoms << QLatin1String("KADDRESSBOOK-X-SpousesName");
    }

    /* Manager */
    const QString manager = contact->managersName();
    if (!manager.isEmpty()) {
        output.append("<gContact:relation rel=\"manager\">").append(Qt::escape(manager).toUtf8()).append("</gContact:relation>");
        parsedCustoms << QLatin1String("KADDRESSBOOK-X-ManagersName");
    }

    /* Assistant */
    const QString assistant = contact->assistantsName();
    if (!assistant.isEmpty()) {
        output.append("<gContact:relation rel=\"assistant\">").append(Qt::escape(assistant).toUtf8()).append("</gContact:relation>");
        parsedCustoms << QLatin1String("KADDRESSBOOK-X-AssistantsName");
    }

    /* Anniversary */
    const QString anniversary = contact->anniversary();
    if (!anniversary.isEmpty()) {
        output.append("<gContact:event rel=\"anniversary\"><gd:when startTime=\"").append(Qt::escape(anniversary).toUtf8()).append("\" /></gContact:event>");
        parsedCustoms << QLatin1String("KADDRESSBOOK-X-Anniversary");
    }

    /* Homepage */
    if (!contact->url().isEmpty()) {
        output.append("<gContact:website rel=\"home-page\" href=\"").append(Qt::escape(contact->url().prettyUrl()).toUtf8()).append("\" />");
    }

    /* Blog */
    const QString blog = contact->blogFeed();
    if (!blog.isEmpty()) {
        output.append("<gContact:website rel=\"blog\" href=\"").append(Qt::escape(blog).toUtf8()).append("\" />");
        parsedCustoms << QLatin1String("KADDRESSBOOK-BlogFeed");
    }

    /* Emails */
    Q_FOREACH(const QString &email, contact->emails()) {
        output.append("<gd:email rel='http://schemas.google.com/g/2005#home' address='").append(Qt::escape(email).toUtf8()).append("' />");
    }

    /* IMs */
    const QString im_str = QLatin1String("<gd:im address=\"%1\" protocol=\"%2\" rel=\"http://schemas.google.com/g/2005#other\" primary=\"%3\"/>");
    Q_FOREACH(const QString &im, contact->customs()) {
        if (im.startsWith(QLatin1String("messaging/"))) {
            QString key = im.left(im.indexOf(QLatin1Char(':')));
            QString value = im.mid(im.indexOf(QLatin1Char(':')) + 1);
            QString proto = key.mid(10);
            proto.chop(4);
            bool primary = (contact->custom(QLatin1String("KADDRESSBOOK"), QLatin1String("X-IMAddress")) == value);
            output.append(im_str.arg(value, Objects::Contact::IMProtocolNameToScheme(proto),
				     (primary ? QLatin1String("true") : QLatin1String("false"))).toUtf8());
            parsedCustoms << key;
        }
    }
    parsedCustoms << QLatin1String("KADDRESSBOOK-X-IMAddress");

    /* Phone numbers */
    const QString phone_str = QLatin1String("<gd:phoneNumber rel=\"%1\">%2</gd:phoneNumber>");
    Q_FOREACH(const KABC::PhoneNumber &number, contact->phoneNumbers()) {
        output.append(phone_str.arg(Objects::Contact::phoneTypeToScheme(number.type()), number.number()).toUtf8());
    }

    /* Address */
    Q_FOREACH(const KABC::Address &address, contact->addresses()) {
        output.append("<gd:structuredPostalAddress rel='")
        .append(Objects::Contact::addressTypeToScheme(address.type()).toUtf8())
        .append("'>");

        if (!address.locality().isEmpty())
            output.append("<gd:city>").append(Qt::escape(address.locality()).toUtf8()).append("</gd:city>");
        if (!address.street().isEmpty())
            output.append("<gd:street>").append(Qt::escape(address.street()).toUtf8()).append("</gd:street>");
        if (!address.region().isEmpty())
            output.append("<gd:region>").append(Qt::escape(address.region()).toUtf8()).append("</gd:region>");
        if (!address.postalCode().isEmpty())
            output.append("<gd:postcode>").append(Qt::escape(address.postalCode()).toUtf8()).append("</gd:postcode>");
        if (!address.country().isEmpty())
            output.append("<gd:country>").append(Qt::escape(address.country()).toUtf8()).append("</gd:country>");
        if (!address.formattedAddress().isEmpty())
            output.append("<gd:formattedAddress>").append(Qt::escape(address.formattedAddress()).toUtf8()).append("</gd:formattedAddress>");
        output.append("</gd:structuredPostalAddress>");
    }

    /* Birthday */
    const QDate birthday = contact->birthday().date();
    if (birthday.isValid()) {
        const QString birthdayStr = birthday.toString(QLatin1String("yyyy-MM-dd"));
        output.append("<gContact:birthday when='").append(birthdayStr.toUtf8()).append("'/>");
    }

    const QStringList groups = contact->custom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo")).split(QLatin1Char(','));
    if ((groups.length() > 0) && !groups.at(0).isEmpty()) {
        Q_FOREACH(const QString & group, groups) {
            bool removed = contact->groupIsDeleted(group);
            if (!removed)
                output.append(QString(QLatin1String("<gContact:groupMembershipInfo deleted=\"false\" href=\"%2\" />")).arg(group).toUtf8());
        }
    }
    parsedCustoms << QLatin1String("GCALENDAR-groupMembershipInfo");

    /* User-defined fields */
    const QStringList customs = contact->customs();
    const QString defined_str = QLatin1String("<gContact:userDefinedField key=\"%1\" value=\"%2\" />");
    Q_FOREACH(const QString &customStr, customs) {
        const QString key = customStr.left(customStr.indexOf(QLatin1Char(':')));
        if (!parsedCustoms.contains(key)) {
            const QString value = customStr.mid(customStr.indexOf(QLatin1Char(':')) + 1);
            output.append(defined_str.arg(Qt::escape(key), Qt::escape(value)).toUtf8());
        }
    }

    return output;
}

QByteArray ContactsPrivate::groupToXML(const KGAPI::Object *object)
{
    const KGAPI::Objects::ContactsGroup *group = static_cast< const KGAPI::Objects::ContactsGroup* >(object);

    QByteArray output;

    output.append("<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
                  "term=\"http://schemas.google.com/g/2005#group\"/>");
    output.append("<atom:title type=\"text\">").append(Qt::escape(group->title()).toUtf8()).append("</atom:title>");
    output.append("<atom:content type=\"text\">").append(Qt::escape(group->content()).toUtf8()).append("</atom:content>");

    return output;
}

KGAPI::Object* ContactsPrivate::XMLToContact(const QDomDocument &doc)
{
    Objects::Contact *contact;
    QStringList groups;

    const QDomNodeList entry = doc.elementsByTagName(QLatin1String("entry"));
    QDomNodeList data;
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return 0;
    }

    contact = new Objects::Contact();

    for (int i = 0; i < data.count(); i++) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (e.tagName() == QLatin1String("id")) {
            contact->setUid(e.text());
            continue;
        }

        /* ETag */
        if (e.tagName() == QLatin1String("etag")) {
            contact->setEtag(e.text());
            continue;
        }

        if (e.tagName() == QLatin1String("gd:name")) {
            QDomNodeList l = e.childNodes();
            for (uint i = 0; i < l.length(); i++) {
                const QDomElement el = l.at(i).toElement();

                if (el.tagName() == QLatin1String("gd:fullname")) {
                    contact->setFormattedName(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:givenName")) {
                    contact->setGivenName(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:familyName")) {
                    contact->setFamilyName(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:additionalName")) {
                    contact->setAdditionalName(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:namePrefix")) {
                    contact->setPrefix(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:nameSuffix")) {
                    contact->setSuffix(el.text());
                    continue;
                }

            }
            continue;
        }

        /* If the contact was deleted, we don't need more info about it.
        * Just store our own flag, which will be then parsed by the resource
        * itself. */
        contact->setDeleted(e.tagName() == QLatin1String("gd:deleted"));

        /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture
        * itself. */
        if ((e.tagName() == QLatin1String("link")) &&
	    (e.attribute(QLatin1String("rel")) == QLatin1String("http://schemas.google.com/contacts/2008/rel#photo"))) {
            contact->setPhotoUrl(e.attribute(QLatin1String("href")));    /* URL */
            continue;
        }

        /* Name */
        if (e.tagName() == QLatin1String("title")) {
            contact->setName(e.text());
            continue;
        }

        /* Note */
        if (e.tagName() == QLatin1String("content")) {
            contact->setNote(e.text());
            continue;
        }

        /* Organization (work) - KABC supports only organization */
        if (e.tagName() == QLatin1String("gd:organization")) {
            const QDomNodeList l = e.childNodes();
            for (uint i = 0; i < l.length(); i++) {
                const QDomElement el = l.at(i).toElement();

                if (el.tagName() == QLatin1String("gd:orgName")) {
                    contact->setOrganization(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:orgDepartment")) {
                    contact->setDepartment(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:orgTitle")) {
                    contact->setTitle(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:where")) {
                    contact->setOffice(el.text());
                    continue;
                }
            }
            continue;
        }

        /* Nickname */
        if (e.tagName() == QLatin1String("gContact:nickname")) {
            contact->setNickName(e.text());
            continue;
        }

        /* Occupation (= organization/title) */
        if (e.tagName() == QLatin1String("gContact:occupation")) {
            contact->setProfession(e.text());
            continue;
        }

        /* Relationships */
        if (e.tagName() == QLatin1String("gContact:relation")) {
            if (e.attribute(QLatin1String("rel"), QString()) == QLatin1String("spouse")) {
                contact->setSpousesName(e.text());
                continue;
            }

            if (e.attribute(QLatin1String("rel"), QString()) == QLatin1String("manager")) {
                contact->setManagersName(e.text());
                continue;
            }

            if (e.attribute(QLatin1String("rel"), QString()) == QLatin1String("assistant")) {
                contact->setAssistantsName(e.text());
                continue;
            }

            continue;
        }

        /* Anniversary */
        if (e.tagName() == QLatin1String("gContact:event")) {
            if (e.attribute(QLatin1String("rel"), QString()) == QLatin1String("anniversary")) {
                QDomElement w = e.firstChildElement(QLatin1String("gd:when"));
                contact->setAnniversary(w.attribute(QLatin1String("startTime"), QString()));
            }

            continue;
        }

        /* Websites */
        if (e.tagName() == QLatin1String("gContact:website")) {
            if (e.attribute(QLatin1String("rel"), QString()) == QLatin1String("home-page")) {
                contact->setUrl(KUrl(e.attribute(QLatin1String("href"), QString())));
                continue;
            }

            if (e.attribute(QLatin1String("rel"), QString()) == QLatin1String("blog")) {
                contact->setBlogFeed(e.attribute(QLatin1String("href"), QString()));
                continue;
            }

            continue;
        }

        /* Emails */
        if (e.tagName() == QLatin1String("gd:email")) {
            contact->insertEmail(e.attribute(QLatin1String("address")),
				 (e.attribute(QLatin1String("primary")).toLower() == QLatin1String("true")));
            continue;
        }

        /* IMs */
        if (e.tagName() == QLatin1String("gd:im")) {
            contact->insertCustom(QLatin1String("messaging/") + Objects::Contact::IMSchemeToProtocolName(e.attribute(QLatin1String("protocol"))),
				  QLatin1String("All"),
				  e.attribute(QLatin1String("address")));
            continue;
        }

        /* Phone numbers */
        if (e.tagName() == QLatin1String("gd:phoneNumber")) {
            contact->insertPhoneNumber(KABC::PhoneNumber(e.text(), Objects::Contact::phoneSchemeToType(e.attribute(QLatin1String("rel")))));
            continue;
        }

        /* Addresses */
        if (e.tagName() == QLatin1String("gd:structuredPostalAddress")) {
            KABC::Address address;
            const QDomNodeList l = e.childNodes();
            for (uint i = 0; i < l.length(); i++) {
                const QDomElement el = l.at(i).toElement();

                if (el.tagName() == QLatin1String("gd:street")) {
                    address.setStreet(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:country")) {
                    address.setCountry(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:city")) {
                    address.setLocality(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:postcode")) {
                    address.setPostalCode(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:region")) {
                    address.setRegion(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:pobox")) {
                    address.setPostOfficeBox(el.text());
                    continue;
                }
            }

            address.setType(Objects::Contact::addressSchemeToType(e.attribute(QLatin1String("rel")),
								  (e.attribute(QLatin1String("primary")) == QLatin1String("true"))));
            contact->insertAddress(address);
            continue;
        }

        /* Birthday */
        if (e.tagName() == QLatin1String("gContact:birthday")) {
            contact->setBirthday(QDateTime::fromString(e.attribute(QLatin1String("when")), QLatin1String("yyyy-MM-dd")));
            continue;
        }

        /* User-defined tags */
        if (e.tagName() == QLatin1String("gContact:userDefinedField")) {
            contact->insertCustom(QLatin1String("GCALENDAR"),
				  e.attribute(QLatin1String("key"), QString()),
				  e.attribute(QLatin1String("value"), QString()));
            continue;
        }

        if (e.tagName() == QLatin1String("gContact:groupMembershipInfo")) {
            if (e.hasAttribute(QLatin1String("deleted")) || e.attribute(QLatin1String("deleted")).toInt() == false) {
                groups.append(e.attribute(QLatin1String("href")));
            }
        }
    }

    contact->insertCustom(QLatin1String("GCALENDAR"), QLatin1String("groupMembershipInfo"), groups.join(QLatin1String(",")));

    return dynamic_cast< KGAPI::Object* >(contact);
}

Object* ContactsPrivate::XMLToGroup(const QDomDocument &doc)
{
    Objects::ContactsGroup *group;
    QStringList groups;

    const QDomNodeList entry = doc.elementsByTagName(QLatin1String("entry"));
    QDomNodeList data;
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return 0;
    }

    group = new Objects::ContactsGroup();

    for (int i = 0; i < data.count(); i++) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (e.tagName() == QLatin1String("id")) {
            group->setId(e.text());
            continue;
        }

        if (e.tagName() == QLatin1String("updated")) {
            group->setUpdated(KGAPI::AccessManager::RFC3339StringToDate(e.text()).dateTime());
            continue;
        }

        if ((e.tagName() == QLatin1String("title")) ||
            (e.tagName() == QLatin1String("atom:title"))) {
            group->setTitle(e.text());
            continue;
        }

        if ((e.tagName() == QLatin1String("content")) ||
            (e.tagName() == QLatin1String("atom:content"))) {
            group->setContent(e.text());
            continue;
        }

        if (e.tagName() == QLatin1String("gContact:systemGroup")) {
            group->setIsSystemGroup(true);
            continue;
        }
    }

    return dynamic_cast< KGAPI::Object* >(group);
}
