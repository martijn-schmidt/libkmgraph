set(kmgraphdrive_SRCS
    about.cpp
    aboutfetchjob.cpp
    app.cpp
    appfetchjob.cpp
    change.cpp
    changefetchjob.cpp
    childreference.cpp
    childreferencecreatejob.cpp
    childreferencedeletejob.cpp
    childreferencefetchjob.cpp
    driveservice.cpp
    file.cpp
    fileabstractdatajob.cpp
    fileabstractmodifyjob.cpp
    fileabstractuploadjob.cpp
    filecopyjob.cpp
    filecreatejob.cpp
    filedeletejob.cpp
    filefetchcontentjob.cpp
    filefetchjob.cpp
    filemodifyjob.cpp
    filesearchquery.cpp
    filetouchjob.cpp
    filetrashjob.cpp
    fileuntrashjob.cpp
    parentreference.cpp
    parentreferencecreatejob.cpp
    parentreferencedeletejob.cpp
    parentreferencefetchjob.cpp
    permission.cpp
    permissioncreatejob.cpp
    permissiondeletejob.cpp
    permissionfetchjob.cpp
    permissionmodifyjob.cpp
    revision.cpp
    revisiondeletejob.cpp
    revisionfetchjob.cpp
    revisionmodifyjob.cpp
    user.cpp

    ../debug.cpp
)

ecm_generate_headers(kmgraphdrive_CamelCase_HEADERS
    HEADER_NAMES
    About
    AboutFetchJob
    App
    AppFetchJob
    Change
    ChangeFetchJob
    ChildReference
    ChildReferenceCreateJob
    ChildReferenceDeleteJob
    ChildReferenceFetchJob
    File
    FileAbstractDataJob
    FileAbstractModifyJob
    FileAbstractUploadJob
    FileCopyJob
    FileCreateJob
    FileDeleteJob
    FileFetchContentJob
    FileFetchJob
    FileModifyJob
    FileSearchQuery
    FileTouchJob
    FileTrashJob
    FileUntrashJob
    ParentReference
    ParentReferenceCreateJob
    ParentReferenceDeleteJob
    ParentReferenceFetchJob
    Permission
    PermissionCreateJob
    PermissionDeleteJob
    PermissionFetchJob
    PermissionModifyJob
    Revision
    RevisionDeleteJob
    RevisionFetchJob
    RevisionModifyJob
    User
    PREFIX KMGraph/Drive
    REQUIRED_HEADERS kmgraphdrive_HEADERS
)

add_library(KPimMGraphDrive
    ${kmgraphdrive_SRCS}
)

generate_export_header(KPimMGraphDrive BASE_NAME kmgraphdrive)

add_library(KPim::MGraphDrive ALIAS KPimMGraphDrive)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphDrive ALIAS KPimMGraphDrive)
target_include_directories(KPimMGraphDrive INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph>")

target_link_libraries(KPimMGraphDrive
PUBLIC
    KPim::MGraphCore
PRIVATE
    Qt5::Network
)

set_target_properties(KPimMGraphDrive PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphDrive
)

install(TARGETS
    KPimMGraphDrive
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphdrive_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph/Drive"
    COMPONENT Devel
)

install(FILES
    ${kmgraphdrive_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphdrive_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph/drive"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphDrive
    LIB_NAME KPimMGraphDrive
    DEPS "MGraphCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
