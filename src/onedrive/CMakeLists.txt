set(kmgraphonedrive_SRCS
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
    onedriveservice.cpp
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

ecm_generate_headers(kmgraphonedrive_CamelCase_HEADERS
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
    PREFIX KMGraph/OneDrive
    REQUIRED_HEADERS kmgraphonedrive_HEADERS
)

add_library(KPimMGraphOneDrive
    ${kmgraphonedrive_SRCS}
)

generate_export_header(KPimMGraphOneDrive BASE_NAME kmgraphonedrive)

add_library(KPim::MGraphOneDrive ALIAS KPimMGraphOneDrive)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphOneDrive ALIAS KPimMGraphOneDrive)
target_include_directories(KPimMGraphOneDrive INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph>")

target_link_libraries(KPimMGraphOneDrive
PUBLIC
    KPim::MGraphCore
PRIVATE
    Qt5::Network
)

set_target_properties(KPimMGraphOneDrive PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphOneDrive
)

install(TARGETS
    KPimMGraphOneDrive
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphonedrive_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph/OneDrive"
    COMPONENT Devel
)

install(FILES
    ${kmgraphonedrive_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphonedrive_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph/onedrive"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphOneDrive
    LIB_NAME KPimMGraphOneDrive
    DEPS "MGraphCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
