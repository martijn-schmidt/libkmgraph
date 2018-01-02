set(kmgraphcontacts_SRCS
    contact.cpp
    contactcreatejob.cpp
    contactdeletejob.cpp
    contactfetchjob.cpp
    contactfetchphotojob.cpp
    contactmodifyjob.cpp
    contactsgroup.cpp
    contactsgroupcreatejob.cpp
    contactsgroupdeletejob.cpp
    contactsgroupfetchjob.cpp
    contactsgroupmodifyjob.cpp
    contactsservice.cpp

    ../debug.cpp
)

ecm_generate_headers(kmgraphcontacts_CamelCase_HEADERS
    HEADER_NAMES
    Contact
    ContactCreateJob
    ContactDeleteJob
    ContactFetchJob
    ContactFetchPhotoJob
    ContactModifyJob
    ContactsGroup
    ContactsGroupCreateJob
    ContactsGroupDeleteJob
    ContactsGroupFetchJob
    ContactsGroupModifyJob
    PREFIX KMGraph/Contacts
    REQUIRED_HEADERS kmgraphcontacts_HEADERS
)

add_library(KPimMGraphContacts
    ${kmgraphcontacts_SRCS}
)

generate_export_header(KPimMGraphContacts BASE_NAME kmgraphcontacts)

add_library(KPim::MGraphContacts ALIAS KPimMGraphContacts)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphContacts ALIAS KPimMGraphContacts)
target_include_directories(KPimMGraphContacts INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph>")

target_link_libraries(KPimMGraphContacts
PUBLIC
    KPim::MGraphCore
    KF5::Contacts
PRIVATE
    Qt5::Xml
    Qt5::Network
)

set_target_properties(KPimMGraphContacts PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphContacts
)

install(TARGETS
    KPimMGraphContacts
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphcontacts_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph/Contacts"
    COMPONENT Devel
)

install(FILES
    ${kmgraphcontacts_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphcontacts_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph/contacts"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphContacts
    LIB_NAME KPimMGraphContacts
    DEPS "KPim::MGraphCore KF5::Contacts Qt5::Xml"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
