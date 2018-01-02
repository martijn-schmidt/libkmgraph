set(kmgraphblogger_SRCS
    blog.cpp
    blogfetchjob.cpp
    bloggerservice.cpp
    comment.cpp
    commentapprovejob.cpp
    commentdeletecontentjob.cpp
    commentdeletejob.cpp
    commentfetchjob.cpp
    page.cpp
    pagecreatejob.cpp
    pagedeletejob.cpp
    pagefetchjob.cpp
    pagemodifyjob.cpp
    post.cpp
    postcreatejob.cpp
    postdeletejob.cpp
    postfetchjob.cpp
    postmodifyjob.cpp
    postpublishjob.cpp
    postsearchjob.cpp

    ../debug.cpp
)

ecm_generate_headers(kmgraphblogger_CamelCase_HEADERS
    HEADER_NAMES
    Blog
    BlogFetchJob
    Comment
    CommentApproveJob
    CommentDeleteContentJob
    CommentDeleteJob
    CommentFetchJob
    Page
    PageCreateJob
    PageDeleteJob
    PageFetchJob
    PageModifyJob
    Post
    PostCreateJob
    PostDeleteJob
    PostFetchJob
    PostModifyJob
    PostPublishJob
    PostSearchJob
    PREFIX KMGraph/Blogger
    REQUIRED_HEADERS kmgraphblogger_HEADERS
)

add_library(KPimMGraphBlogger
    ${kmgraphblogger_SRCS}
)

generate_export_header(KPimMGraphBlogger BASE_NAME kmgraphblogger)

add_library(KPim::MGraphBlogger ALIAS KPimMGraphBlogger)
# TODO: Backwards compatibility, remove in next major release
add_library(KF5::MGraphBlogger ALIAS KPimMGraphBlogger)
target_include_directories(KPimMGraphBlogger INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph>")

target_link_libraries(KPimMGraphBlogger
PUBLIC
    KPim::MGraphCore
PRIVATE
    Qt5::Network
)

set_target_properties(KPimMGraphBlogger PROPERTIES
    VERSION ${KMGRAPH_VERSION_STRING}
    SOVERSION ${KMGRAPH_SOVERSION}
    EXPORT_NAME MGraphBlogger
)

install(TARGETS
    KPimMGraphBlogger
    EXPORT KPimMGraphTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${kmgraphblogger_CamelCase_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/KMGraph/Blogger"
    COMPONENT Devel
)

install(FILES
    ${kmgraphblogger_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/kmgraphblogger_export.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph/kmgraph/blogger"
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KMGraphBlogger
    LIB_NAME KPimMGraphBlogger
    DEPS "MGraphCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR "${KDE_INSTALL_INCLUDEDIR}/KPim/KMGraph"
)

install(FILES
    "${PRI_FILENAME}"
    DESTINATION "${ECM_MKSPECS_INSTALL_DIR}"
)
