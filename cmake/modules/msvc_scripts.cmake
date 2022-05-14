# compile flags:
# Enhanced Instruction Set /arch:SSE2,AVX
# Multi-processor compilation /MP
# Enable parallel code generation /Qpar
# Enable function level linking /Gy

if(CMAKE_CL_64)
	set(THOR_MSVC_COMPILER_FLAGS_DEBUG /MP /ZI)
	set(THOR_MSVC_COMPILER_FLAGS_RELEASE /MP /Qpar /Gy)
else ()
	set(THOR_MSVC_COMPILER_FLAGS_DEBUG /arch:SSE2 /MP /ZI)
	set(THOR_MSVC_COMPILER_FLAGS_RELEASE /arch:SSE2 /MP /Qpar /Gy)
endif()

if (THOR_MSVC_STATIC_RUNTIME)
	list (APPEND THOR_MSVC_COMPILER_FLAGS_DEBUG /MTd)
	list (APPEND THOR_MSVC_COMPILER_FLAGS_RELEASE /MT)
endif()
# linker flags:
# Image Has Safe Exception Handlers /SAFESEH:YES,NO
# SubSystem /SUBSYSTEM:WINDOWS,CONSOLE
set(THOR_MSVC_LINKER_FLAGS_EXE_DEBUG "/SAFESEH:NO /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE")
set(THOR_MSVC_LINKER_FLAGS_EXE_RELEASE "/SAFESEH:NO /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE")

set(THOR_MSVC_LINKER_FLAGS_CONSOLE_EXE_DEBUG "/SAFESEH:NO /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE")
set(THOR_MSVC_LINKER_FLAGS_CONSOLE_EXE_RELEASE "/SAFESEH:NO /SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE")

function (thor_msvc_target_set_compiler_options target)
	if (MSVC)
		target_compile_options(${target} PUBLIC "$<$<CONFIG:DEBUG>:${THOR_MSVC_COMPILER_FLAGS_DEBUG}>")
		target_compile_options(${target} PUBLIC "$<$<CONFIG:RELEASE>:${THOR_MSVC_COMPILER_FLAGS_RELEASE}>")
		target_compile_options(${target} PUBLIC "$<$<CONFIG:MINSIZEREL>:${THOR_MSVC_COMPILER_FLAGS_RELEASE}>")
		target_compile_options(${target} PUBLIC "$<$<CONFIG:RELWITHDEBINFO>:${THOR_MSVC_COMPILER_FLAGS_RELEASE}>")
	endif()	
endfunction()

function (thor_msvc_target_setup_library target)
	thor_msvc_target_set_compiler_options(${target})
	thor_target_install(${target})
endfunction()

function (thor_msvc_target_link_libraries target)
	target_link_libraries(${target}
			Opengl32.lib
			Winmm.lib
			Iphlpapi.lib
			kernel32.lib
			user32.lib
			gdi32.lib
			winspool.lib
			Ws2_32.lib
			comdlg32.lib
			advapi32.lib
			shell32.lib
			ole32.lib
			oleaut32.lib
			uuid.lib
			odbc32.lib
			odbccp32.lib
			)
endfunction()

function (thor_msvc_target_setup_executable target)
	if (MSVC)
		thor_msvc_target_set_compiler_options(${target})
		set_target_properties(${target} PROPERTIES LINK_FLAGS_DEBUG ${THOR_MSVC_LINKER_FLAGS_EXE_DEBUG})
		set_target_properties(${target} PROPERTIES LINK_FLAGS_RELEASE ${THOR_MSVC_LINKER_FLAGS_EXE_RELEASE})
		set_target_properties(${target} PROPERTIES LINK_FLAGS_MINSIZEREL ${THOR_MSVC_LINKER_FLAGS_EXE_RELEASE})
		set_target_properties(${target} PROPERTIES LINK_FLAGS_RELWITHDEBINFO ${THOR_MSVC_LINKER_FLAGS_EXE_RELEASE})		
		thor_msvc_target_link_libraries(${target})
		thor_target_install(${target})
	endif()
endfunction()

function (thor_msvc_target_setup_console_executable target)
	if (MSVC)
		thor_msvc_target_set_compiler_options(${target})
		set_target_properties(${target} PROPERTIES LINK_FLAGS_DEBUG ${THOR_MSVC_LINKER_FLAGS_CONSOLE_EXE_DEBUG})
		set_target_properties(${target} PROPERTIES LINK_FLAGS_RELEASE ${THOR_MSVC_LINKER_FLAGS_CONSOLE_EXE_RELEASE})
		set_target_properties(${target} PROPERTIES LINK_FLAGS_MINSIZEREL ${THOR_MSVC_LINKER_FLAGS_CONSOLE_EXE_RELEASE})
		set_target_properties(${target} PROPERTIES LINK_FLAGS_RELWITHDEBINFO ${THOR_MSVC_LINKER_FLAGS_CONSOLE_EXE_RELEASE})		
		thor_msvc_target_link_libraries(${target})
		thor_target_install(${target})
	endif()
endfunction()