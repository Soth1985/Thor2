##########################################################################
# PhysFS                                                   	     	     #
##########################################################################
add_library(PhysFS STATIC IMPORTED)
set_property(TARGET PhysFS PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${THOR_INSTALL_DIR}/include)
set_property(TARGET PhysFS PROPERTY IMPORTED_LOCATION ${THOR_INSTALL_DIR}/lib/libphysfs.a)
