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

#ifndef LIBKMGRAPH2_ONEDRIVESERVICE_H
#define LIBKMGRAPH2_ONEDRIVESERVICE_H

#include "types.h"
#include "kmgraphonedrive_export.h"

#include <QUrl>

namespace KMGraph2
{

namespace OneDriveService
{

    /**
     * @brief Returns URL for retrieving About data
     *
     * @param includeSubscribed
     * @param maxChangeIdCount
     * @param startChangeId
     */
    KMGRAPHONEDRIVE_EXPORT QUrl fetchAboutUrl(bool includeSubscribed,
                                        qlonglong maxChangeIdCount,
                                        qlonglong startChangeId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchAppUrl(const QString &appId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchAppsUrl();

    KMGRAPHONEDRIVE_EXPORT QUrl fetchChildReference(const QString &folderId,
                                              const QString &referenceId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchChildReferences(const QString &folderId);

    KMGRAPHONEDRIVE_EXPORT QUrl createChildReference(const QString &folderId);

    KMGRAPHONEDRIVE_EXPORT QUrl deleteChildReference(const QString &folderId,
                                               const QString &referenceId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchChangeUrl(const QString &changeId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchChangesUrl();

    KMGRAPHONEDRIVE_EXPORT QUrl copyFileUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl deleteFileUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchFileUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchFilesUrl();

    KMGRAPHONEDRIVE_EXPORT QUrl touchFileUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl trashFileUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl untrashFileUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl uploadMetadataFileUrl(const QString &fileId = QString());

    KMGRAPHONEDRIVE_EXPORT QUrl uploadMediaFileUrl(const QString &fileId = QString());

    KMGRAPHONEDRIVE_EXPORT QUrl uploadMultipartFileUrl(const QString &fileId = QString());

    KMGRAPHONEDRIVE_EXPORT QUrl fetchParentReferenceUrl(const QString &fileId,
                                                  const QString &referenceId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchParentReferencesUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl createParentReferenceUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl deleteParentReferenceUrl(const QString &fileId,
                                                   const QString &referenceId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchPermissionsUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchPermissionUrl(const QString &fileId,
                                             const QString &permissionId);

    KMGRAPHONEDRIVE_EXPORT QUrl createPermissionUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl deletePermissionUrl(const QString &fileId,
                                              const QString &permissionId);
    KMGRAPHONEDRIVE_EXPORT QUrl modifyPermissionUrl(const QString &fileId,
                                              const QString &permissionId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchRevisionUrl(const QString &fileId,
                                           const QString &revisionId);

    KMGRAPHONEDRIVE_EXPORT QUrl fetchRevisionsUrl(const QString &fileId);

    KMGRAPHONEDRIVE_EXPORT QUrl deleteRevisionUrl(const QString &fileId,
                                            const QString &revisionId);

    KMGRAPHONEDRIVE_EXPORT QUrl modifyRevisionUrl(const QString &fileId,
                                            const QString &revisionId);

} // namespace OneDriveService

} // namespace KMGraph2

#endif // KMGRAPH2_ONEDRIVESERVICE_H
