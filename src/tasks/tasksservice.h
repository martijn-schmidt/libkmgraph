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

#ifndef LIBKMGRAPH2_TASKSSERVICE_H
#define LIBKMGRAPH2_TASKSSERVICE_H

#include "kmgraphtasks_export.h"
#include "types.h"

namespace KMGraph2
{

/**
 * @brief Additional methods for implementing support for Google Tasks service
 *
 * You should never need to use these methods, unless implementing your own Job
 */
namespace TasksService
{

    /**
     * @brief Parses JSON data into a Task object
     *
     * @param jsonData
     */
    KMGRAPHTASKS_EXPORT TaskPtr JSONToTask(const QByteArray& jsonData);

    /**
     * @brief Serializes a Task object into JSON
     *
     * @param task
     */
    KMGRAPHTASKS_EXPORT QByteArray taskToJSON(const TaskPtr& task);

    /**
     * @brief Parses JSON data into a TaskList object
     *
     * @param jsonData
     */
    KMGRAPHTASKS_EXPORT TaskListPtr JSONToTaskList(const QByteArray& jsonData);

    /**
     * @brief Serializes a TaskList into JSON data
     *
     * @param taskList
     */
    KMGRAPHTASKS_EXPORT QByteArray taskListToJSON(const TaskListPtr& taskList);

    /**
     * @brief Parses JSON feed into list of Tasks or TaskLists
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional information
     *                 about the feed
     */
    KMGRAPHTASKS_EXPORT ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Returns URL to create a single task
     *
     * @param tasklistID ID of parent task list
     */
    KMGRAPHTASKS_EXPORT QUrl createTaskUrl(const QString &tasklistID);

    /**
     * @brief Returns URL to fetch all tasks from a single tasklist
     *
     * @param tasklistID ID of parent task list
     */
    KMGRAPHTASKS_EXPORT QUrl fetchAllTasksUrl(const QString &tasklistID);

    /**
     * @brief Returns URL for fetching a single task
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to fetch
     */
    KMGRAPHTASKS_EXPORT QUrl fetchTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * @brief Returns URL for updating a task
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to update
     */
    KMGRAPHTASKS_EXPORT QUrl updateTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * @brief Returns URL for removing a task
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to remove
     */
    KMGRAPHTASKS_EXPORT QUrl removeTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * @brief Returns URL to reparent task.
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to move
     * @param newParent UID of new parent item
     */
    KMGRAPHTASKS_EXPORT QUrl moveTaskUrl(const QString &tasklistID, const QString &taskID, const QString &newParent);

    /**
     * @brief Returns URL for fetching all tasklists
     */
    KMGRAPHTASKS_EXPORT QUrl fetchTaskListsUrl();

    /**
     * @brief Returns URL for creating a new tasklist
     */
    KMGRAPHTASKS_EXPORT QUrl createTaskListUrl();

    /**
     * @brief Returns URL for modifying a tasklist
     *
     * @param tasklistID ID of task list to update
     */
    KMGRAPHTASKS_EXPORT QUrl updateTaskListUrl(const QString &tasklistID);

    /**
     * @brief Returns URL for deleteing a tasklist
     *
     * @param tasklistID ID of task list to remove
     */
    KMGRAPHTASKS_EXPORT QUrl removeTaskListUrl(const QString &tasklistID);

} /* namespace TasksServices */

} /* namespace KMGraph2 */

#endif // LIBKMGRAPH2_TASKSSERVICE_H
