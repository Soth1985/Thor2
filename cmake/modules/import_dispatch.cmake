if (MSVC)
    add_library(dispatch SHARED IMPORTED)
    set_property(TARGET dispatch PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/import/dispatch/include)
	
	set_property(TARGET dispatch PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/import/dispatch/bin/dispatch.dll)
	set_property(TARGET dispatch PROPERTY IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/import/dispatch/lib/dispatch.lib)
endif()