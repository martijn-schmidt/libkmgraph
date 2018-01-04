ecm_create_qm_loader(QM_LOADER libkmgraph_qt)

set(kmgraphcore_SRCS
    accountinfo/accountinfo.cpp
    accountinfo/accountinfofetchjob.cpp
    private/newtokensfetchjob.cpp
    account.cpp
    createjob.cpp
    deletejob.cpp
    fetchjob.cpp
    job.cpp
    modifyjob.cpp
    object.cpp
    utils.cpp
    ${QM_LOADER}

    ../debug.cpp
)

ecm_generate_headers(kmgraphcore_base_CamelCase_HEADERS
    HEADER_NAMES
    Account
    CreateJob
    DeleteJob
    FetchJob
    Job
    ModifyJob
    Object
    Types
    Utils
    PREFIX KMGraph
    REQUIRED_HEADERS kmgraphcore_base_HEADERS
)

ecm_generate_headers(kmgraphcore_accountinfo_CamelCase_HEADERS
    HEADER_NAMES
    AccountInfo
    AccountInfoFetchJob
    REQUIRED_HEADERS kmgraphcore_accountinfo_HEADERS
    RELATIVE accountinfo
)

add_library(KPimMGraphCore
    ${kmgraphcore_SRCS}
)

generate_export_header(KPimMGraphCore BASE_NAME kmgraphcore)

add_library(KPim::MGraphCore ALIAS KPimMGraphCore)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphCore ALIAS KPimMGraphCore)
target_include_directories(KPimMGraphCore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph;${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph>")

target_link_libraries(KPimMGraphCore
PRIVATE
    KF5::KIOWidgets
    KF5::WindowSystem
    Qt5::WebEngineWidgets
PUBLIC
    Qt5::Widgets
)

set_target_properties(KPimMGraphCore PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphCore
)

install(TARGETS
    KPimMGraphCore
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphcore_base_CamelCase_HEADERS}
    ${kmgraphcore_accountinfo_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph"
    COMPONENT Devel
)

install(FILES
    ${kmgraphcore_base_HEADERS}
    ${kmgraphcore_accountinfo_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphcore_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphCore
    LIB_NAME KPimMGraphCore
    DEPS "Qt5::Widgets"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
