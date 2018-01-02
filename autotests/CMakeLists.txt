include(ECMMarkAsTest)

find_package(Qt5Test CONFIG REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

MACRO(LIBKMGRAPH_TESTS)
       FOREACH(_testname ${ARGN})
               add_executable( ${_testname} ${_testname}.cpp )
               add_test(NAME ${_testname} COMMAND ${_testname} )
               ecm_mark_as_test(libkmgraph-${_testname})
               target_link_libraries(${_testname}
                                     Qt5::Test
                                     KF5::Contacts
                                     KF5::Mime
                                     KF5::CalendarCore
                                     KF5::KIOCore
                                     kmgraph)
       ENDFOREACH(_testname)
ENDMACRO(LIBKMGRAPH_TESTS)

macro(add_libkmgraph2_test _module _testname)
    add_executable( ${_module}-${_testname} ${_module}/${_testname}.cpp )
    add_test(NAME ${_module}-${_testname} COMMAND ${_module}-${_testname} )
    ecm_mark_as_test(libkmgraph2-${_module}-${_testname})
    target_link_libraries(${_module}-${_testname}
                          Qt5::Test
                          KF5::Contacts
                          KF5::CalendarCore
                          KPimMGraphDrive)
endmacro(add_libkmgraph2_test)

#LIBKMGRAPH_TESTS(
#    objectsaccountinfotests
#    objectscalendartests
#    objectscontacttests
#    objectscontactsgrouptests
#    objectseventtests
#    objectstasktests
#    objectstasklisttests
#    objectslocationtests
#    objectstaticmapurltests
#    servicesaccountinfotests
#    servicescalendartests
#    servicescontactstests
#    serviceslatitudetests
#    servicestaskstests
#)

add_libkmgraph2_test(drive filesearchquerytest)
