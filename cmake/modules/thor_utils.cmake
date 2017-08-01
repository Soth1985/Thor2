function (thor_target_cpp_std target std)
    if(APPLE)
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD ${std})
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    else()
        target_compile_options(${target} PRIVATE -std=${std} -stdlib=libc++)
    endif()
endfunction()

function(thor_add_precompiled_header target_name pch_header pch_source)
	if(CMAKE_GENERATOR MATCHES "^Visual.*$")
		get_filename_component(pch_header_fname ${pch_header} NAME)
		set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "/Yu${pch_header_fname}")
		set_source_files_properties(${pch_source} PROPERTIES COMPILE_FLAGS "/Yc${pch_header_fname}")
	elseif (CMAKE_GENERATOR MATCHES Xcode)
		get_filename_component(full_path ${pch_header} ABSOLUTE)
		set_target_properties(${target_name} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${full_path}")
		set_target_properties(${target_name} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
	endif()
endfunction()

function (thor_target_install target_name)
	target_include_directories(${target_name} PUBLIC ${THOR_INCLUDE_DIR})
	target_include_directories(${target_name} PUBLIC ${THOR_SUBMODULE_DIR})
	export(TARGETS ${target_name} FILE "${target_name}.cmake")
	install(TARGETS ${target_name}
    		ARCHIVE  DESTINATION lib/Thor
    		LIBRARY  DESTINATION lib/Thor
			RUNTIME  DESTINATION bin/Thor
			BUNDLE DESTINATION bundle/Thor)
endfunction()

macro (thor_add_configuration config_in parent_in)
	set(config "")
	set(parent "")
	string(TOUPPER ${config_in} config)
	string(TOUPPER ${parent_in} parent)
	list(APPEND CMAKE_CONFIGURATION_TYPES ${config_in})
	list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CXX_FLAGS_${config} ${CMAKE_CXX_FLAGS_${parent}})
	set(CMAKE_C_FLAGS_${config} ${CMAKE_C_FLAGS_${parent}})
	set(CMAKE_EXE_LINKER_FLAGS_${config} ${CMAKE_EXE_LINKER_FLAGS_${parent}})
	set(CMAKE_SHARED_LINKER_FLAGS_${config} ${CMAKE_SHARED_LINKER_${parent}})
	#set(CMAKE_LIB_LINKER_FLAGS_${config} ${CMAKE_LIB_LINKER_FLAGS_${parent}})
endmacro()

function(thor_target_add_framework target fwname)
    find_library(FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS ${CMAKE_OSX_SYSROOT}/System/Library
        PATH_SUFFIXES Frameworks
        NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        message(ERROR ": Framework ${fwname} not found")
    else()
        target_link_libraries(${target} ${FRAMEWORK_${fwname}})
        message(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endfunction()

function(thor_set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
	set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endfunction()