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

#ifndef LIBKMGRAPH2_ONEDRIVEREVISION_H
#define LIBKMGRAPH2_ONEDRIVEREVISION_H

#include "types.h"
#include "object.h"
#include "kmgraphonedrive_export.h"

#include <QString>
#include <QUrl>

#include <QDateTime>

namespace KMGraph2
{

namespace OneDrive
{

/**
 * @brief Revision contains a revision of a file. NOTE: BETA FEATURE per OneDrive documentation.
 *
 * Getters and setters' documentation is based on Microsoft OneDrive's Graph API v1.0
 * @see <a href="https://docs.microsoft.com/en-us/onedrive/developer/rest-api/api/driveitem_list_versions">DriveItem List versions</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class KMGRAPHONEDRIVE_EXPORT Revision: public KMGraph2::Object
{

  public:
    explicit Revision();
    explicit Revision(const Revision &other);
    ~Revision() override;

    /**
     * @brief Returns the id of the revision.
     */
    QString id() const;

    /**
     * @brief Returns a link back to this revision.
     */
    QUrl selfLink() const;

    /**
     * @brief Returns the MIME type of the revision.
     */
    QString mimeType() const;

    /**
     * @brief Returns the last time this revision was modified.
     */
    QDateTime modifiedDate() const;

    /**
     * @brief Returns whether this revision is pinned to prevent automatic purging.
     *
     * This will only be populated and can only be modified on files with content
     * stored in OneDrive which are not Microsoft Documents. TODO: is this accurate for Graph?
     *
     * Revisions can also be pinned when they are created through the
     * drive.files.insert/update/copy by using the pinned query parameter.
     */
    bool pinned() const;

    /**
     * @brief Sets whether this revision is pinned to prevent automatic purging.
     *
     * This will only be populated and can only be modified on files with content
     * stored in OneDrive which are not Microsoft Documents. TODO: is this accurate for Graph?
     *
     * Revisions can also be pinned when they are created through the
     * drive.files.insert/update/copy by using the pinned query parameter.
     *
     * @param pinned
     */
    void setPinned(bool pinned);

    /**
     * @brief Returns whether this revision is published.
     *
     * This is only populated and can only be modified for Microsoft Documents. TODO: is this accurate for Graph?
     */
    bool published() const;

    /**
     * @brief Sets whether this revision is published.
     *
     * @param published
     */
    void setPublished(bool published);

    /**
     * @brief Returns a link to the published revision.
     */
    QUrl publishedLink() const;

    /**
     * @brief Returns whether subsequent revisions will be automatically republished.
     *
     * This is only populated and can only be modified for Microsoft Documents. TODO: is this accurate for Graph?
     */
    bool publishAuto() const;

    /**
     * @brief Sets whether subsequent revisions will be automatically republished.
     *
     * This is only populated and can only be modified for Microsoft Documents. TODO: is this accurate for Graph?
     *
     * @param publishedAuto
     */
    void setPublishAuto(bool publishAuto);

    /**
     * @brief Returns whether this revision is published outside the domain.
     *
     * This is only populated and can only be modified for Microsoft Documents. TODO: is this accurate for Graph?
     */
    bool publishedOutsideDomain() const;

    /**
     * @brief Sets whether this revision is published outside the domain.
     *
     * This is only populated and can only be modified for Microsoft Documents. TODO: is this accurate for Graph?
     *
     * @param publishedOutsideDomain
     */
    void setPublishedOutsideDomain(bool publishedOutsideDomain);

    /**
     * @brief Returns a short term download URL for the file.
     *
     * This will only be populated on files with content stored in OneDrive.
     */
    QUrl downloadUrl() const;

    /**
     * @brief Returns the links for exporting OneDrive documents to specific formats.
     *
     * This is a map from the export format to URL.
     */
    QMap < QString /* format */, QUrl /* url */ > exportLinks() const;

    /**
     * @brief Returns the name of the last user to modify this revision.
     */
    QString lastModifyingUserName() const;

    /**
     * @brief Returns object representing the last user to modify this revision
     */
    UserPtr lastModifyingUser() const;

    /**
     * @brief Returns the original filename when this revision was created.
     *
     * This will only be populated on files with content stored in OneDrive.
     */
    QString originalFilename() const;

    /**
     * @brief Returns an MD5 checksum for the content of this revision.
     *
     * This will only be populated on files with content stored in OneDrive
     */
    QString md5Checksum() const;

    /**
     * @brief Returns the size of the revision in bytes.
     *
     * This will only be populated on files with content stored in OneDrive.
     */
    qlonglong fileSize() const;

    static RevisionPtr fromJSON(const QByteArray &jsonData);
    static RevisionsList fromJSONFeed(const QByteArray &jsonData);
    static QByteArray toJSON(const RevisionPtr &revision);

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} /* namespace OneDrive */

} /* namespace KMGraph2 */

#endif // LIBKMGRAPH2_ONEDRIVEREVISION_H
