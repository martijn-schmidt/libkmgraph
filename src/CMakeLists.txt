# Disable KGAPI libraries which we don't need for KMGraph to interface with OneDrive
#add_subdirectory(blogger)
#add_subdirectory(calendar)
#add_subdirectory(contacts)
add_subdirectory(core)
add_subdirectory(onedrive)
#add_subdirectory(latitude)
#add_subdirectory(staticmaps)
#add_subdirectory(tasks)
install( FILES libkmgraph.categories DESTINATION ${KDE_INSTALL_CONFDIR} )

