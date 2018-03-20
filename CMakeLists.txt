cmake_minimum_required(VERSION 3.0)
set(PIM_VERSION "5.8.40")

set(KMGRAPH_LIB_VERSION ${PIM_VERSION})

project(kmgraph VERSION ${KMGRAPH_LIB_VERSION})

# ECM setup
set(KF5_VERSION "5.44.0")

find_package(ECM ${KF5_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(CMakePackageConfigHelpers)
include(ECMPoQmTools)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)


ecm_setup_version(PROJECT
    VARIABLE_PREFIX KMGraph
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kmgraph_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPimMGraphConfigVersion.cmake"
    SOVERSION 5
)

############## Find Packages ##############
set(REQUIRED_QT_VERSION "5.8.0")
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS
    Core
    Network
    Widgets
    WebEngineWidgets
    Xml
)

find_package(KF5 ${KF5_VERSION} REQUIRED COMPONENTS
    KIO
    WindowSystem
)

add_definitions( -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT )
add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
# TODO FIX!
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x000000)

############## Targets ##############
add_subdirectory(src)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

############## CMake Config Files ##############
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPimMGraph")
set(KMGRAPH_KF5_COMPAT FALSE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimMGraphConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPimMGraphConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPimMGraphConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPimMGraphConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPimMGraphTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPimMGraphTargets.cmake
    NAMESPACE KPim::
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraph_version.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim"
    COMPONENT Devel
)


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
