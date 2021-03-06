if (WIN32)
    add_library(bass SHARED IMPORTED)
    set_property(TARGET bass PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/submodules/bass)
	
	if(CMAKE_CL_64)
		set_property(TARGET bass PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/submodules/bass/x64/bass.dll)
		set_property(TARGET bass PROPERTY IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/submodules/bass/x64/bass.lib)
	else ()
		set_property(TARGET bass PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/submodules/bass/bass.dll)
		set_property(TARGET bass PROPERTY IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/submodules/bass/bass.lib)
	endif()
endif()

if (IOS)
    add_library(bass STATIC IMPORTED)
    set_property(TARGET bass PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/submodules/bass)
    set_property(TARGET bass PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/submodules/bass/libbass.a)
endif()