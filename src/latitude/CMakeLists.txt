set(kmgraphlatitude_SRCS
    latitudeservice.cpp
    latitude.h
    location.cpp
    locationcreatejob.cpp
    locationdeletejob.cpp
    locationfetchhistoryjob.cpp
    locationfetchjob.cpp

    ../debug.cpp
)

ecm_generate_headers(kmgraphlatitude_CamelCase_HEADERS
    HEADER_NAMES
    Location
    LocationCreateJob
    LocationDeleteJob
    LocationFetchHistoryJob
    LocationFetchJob
    Latitude
    PREFIX KMGraph/Latitude
    REQUIRED_HEADERS kmgraphlatitude_HEADERS
)

add_library(KPimMGraphLatitude
    ${kmgraphlatitude_SRCS}
)

generate_export_header(KPimMGraphLatitude BASE_NAME kmgraphlatitude)

add_library(KPim::MGraphLatitude ALIAS KPimMGraphLatitude)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphLatitude ALIAS KPimMGraphLatitude)
target_include_directories(KPimMGraphLatitude INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph>")

target_link_libraries(KPimMGraphLatitude
PUBLIC
    KPim::MGraphCore
    KF5::Contacts
PRIVATE
    Qt5::Network
)

set_target_properties(KPimMGraphLatitude PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphLatitude
)

install(TARGETS
    KPimMGraphLatitude
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphlatitude_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph/Latitude"
    COMPONENT Devel
)

install(FILES
    ${kmgraphlatitude_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphlatitude_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph/latitude"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphLatitude
    LIB_NAME KPimMGraphLatitude
    DEPS "MGraphCore Contacts"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
