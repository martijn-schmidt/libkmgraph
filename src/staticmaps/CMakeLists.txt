set(kmgraphmaps_SRCS
    staticmapmarker.cpp
    staticmappath.cpp
    staticmaptilefetchjob.cpp
    staticmapurl.cpp

    ../debug.cpp
)

ecm_generate_headers(kmgraphmaps_CamelCase_HEADERS
    HEADER_NAMES
    StaticMapMarker
    StaticMapPath
    StaticMapTileFetchJob
    StaticMapUrl
    PREFIX KMGraph/Maps
    REQUIRED_HEADERS kmgraphmaps_HEADERS
)

add_library(KPimMGraphMaps
    ${kmgraphmaps_SRCS}
)

generate_export_header(KPimMGraphMaps BASE_NAME kmgraphmaps)

add_library(KPim::MGraphMaps ALIAS KPimMGraphMaps)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphMaps ALIAS KPimMGraphMaps)
target_include_directories(KPimMGraphMaps INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph>")

target_link_libraries(KPimMGraphMaps
PUBLIC
    KPim::MGraphCore
    KF5::Contacts
PRIVATE
    Qt5::Network
)

set_target_properties(KPimMGraphMaps PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphMaps
)

install(TARGETS
    KPimMGraphMaps
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphmaps_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph/Maps"
    COMPONENT Devel
)

install(FILES
    ${kmgraphmaps_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphmaps_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph/maps"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphMaps
    LIB_NAME KPimMGraphMaps
    DEPS "MGraphCore Contacts"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
