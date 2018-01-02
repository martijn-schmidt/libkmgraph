set(kmgraphtasks_SRCS
    task.cpp
    taskcreatejob.cpp
    taskdeletejob.cpp
    taskfetchjob.cpp
    tasklist.cpp
    tasklistcreatejob.cpp
    tasklistdeletejob.cpp
    tasklistfetchjob.cpp
    tasklistmodifyjob.cpp
    taskmodifyjob.cpp
    taskmovejob.cpp
    tasksservice.cpp

    ../debug.cpp
)

ecm_generate_headers(kmgraphtasks_CamelCase_HEADERS
    HEADER_NAMES
    Task
    TaskCreateJob
    TaskDeleteJob
    TaskFetchJob
    TaskList
    TaskListCreateJob
    TaskListDeleteJob
    TaskListFetchJob
    TaskListModifyJob
    TaskModifyJob
    TaskMoveJob
    PREFIX KMGraph/Tasks
    REQUIRED_HEADERS kmgraphtasks_HEADERS
)

add_library(KPimMGraphTasks
    ${kmgraphtasks_SRCS}
)

generate_export_header(KPimMGraphTasks BASE_NAME kmgraphtasks)

add_library(KPim::MGraphTasks ALIAS KPimMGraphTasks)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphTasks ALIAS KPimMGraphTasks)
target_include_directories(KPimMGraphTasks INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph>")

target_link_libraries(KPimMGraphTasks
PUBLIC
    KPim::MGraphCore
    KF5::CalendarCore
PRIVATE
    Qt5::Network
)

set_target_properties(KPimMGraphTasks PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphTasks
)

install(TARGETS
    KPimMGraphTasks
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphtasks_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph/Tasks"
    COMPONENT Devel
)

install(FILES
    ${kmgraphtasks_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphtasks_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph/tasks"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphTasks
    LIB_NAME KPimMGraphTasks
    DEPS "MGraphCore CalendarCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
