set(kmgraphcalendar_SRCS
    calendar.cpp
    calendarcreatejob.cpp
    calendardeletejob.cpp
    calendarfetchjob.cpp
    calendarmodifyjob.cpp
    calendarservice.cpp
    event.cpp
    eventcreatejob.cpp
    eventdeletejob.cpp
    eventfetchjob.cpp
    eventmodifyjob.cpp
    eventmovejob.cpp
    reminder.cpp
    freebusyqueryjob.cpp
    ../debug.cpp
)

ecm_generate_headers(kmgraphcalendar_CamelCase_HEADERS
    HEADER_NAMES
    Calendar
    CalendarCreateJob
    CalendarDeleteJob
    CalendarFetchJob
    CalendarModifyJob
    Event
    EventCreateJob
    EventDeleteJob
    EventFetchJob
    EventModifyJob
    EventMoveJob
    Reminder
    FreeBusyQueryJob
    PREFIX KMGraph/Calendar
    REQUIRED_HEADERS kmgraphcalendar_HEADERS
)

add_library(KPimMGraphCalendar
    ${kmgraphcalendar_SRCS}
)

generate_export_header(KPimMGraphCalendar BASE_NAME kmgraphcalendar)

add_library(KPim::MGraphCalendar ALIAS KPimMGraphCalendar)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphCalendar ALIAS KPimMGraphCalendar)
target_include_directories(KPimMGraphCalendar INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph>")

target_link_libraries(KPimMGraphCalendar
PUBLIC
    KPim::MGraphCore
    KF5::CalendarCore
PRIVATE
    Qt5::Network
)

set_target_properties(KPimMGraphCalendar PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphCalendar
)

install(TARGETS
    KPimMGraphCalendar
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphcalendar_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph/Calendar"
    COMPONENT Devel
)

install(FILES
    ${kmgraphcalendar_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphcalendar_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph/calendar"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphCalendar
    LIB_NAME KPimMGraphCalendar
    DEPS "MGraphCore CalendarCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
