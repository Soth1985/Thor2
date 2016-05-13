##########################################################################
# ThorMath		                                                      	 #
##########################################################################
file(GLOB ThorMath_CPP "*.cpp")
file(GLOB ThorMath_C "*.c")
file(GLOB ThorMath_INL "*.inl")
file(GLOB ThorMath_H "*.h")

file(GLOB ThorNurbs_CPP "Nurbs/*.cpp")
file(GLOB ThorNurbs_INL "Nurbs/*.inl")
file(GLOB ThorNurbs_H "Nurbs/*.h")

set(ThorMathCore_Src ${ThorMath_CPP} ${ThorMath_C} ${ThorMath_H} ${ThorMath_INL})
set(ThorNurbs_Src ${ThorNurbs_CPP} ${ThorNurbs_H} ${ThorNurbs_INL})
set(ThorMath_Src ${ThorMathCore_Src} ${ThorNurbs_Src})

source_group( ThorMath FILES ${ThorMathCore_Src} )
source_group( ThorNurbs FILES ${ThorNurbs_Src} )

if(THOR_BUILD_SHARED)
	add_definitions(-DTHOR_MATH_EXPORT)
	add_library(ThorMath SHARED ${ThorMath_Src})
	set_target_properties(ThorMath PROPERTIES LINKER_LANGUAGE CXX)
	set(ThorMathInstallTargets ${ThorMathInstallTargets} ThorMath)
elseif(THOR_BUILD_STATIC)
	add_library(LibThorMath STATIC ${ThorMath_Src})
	set_target_properties(ThorMath PROPERTIES LINKER_LANGUAGE CXX)
	set(ThorMathInstallTargets ${ThorMathInstallTargets} LibThorMath)
endif()

thor_install( ThorMathInstallTargets )