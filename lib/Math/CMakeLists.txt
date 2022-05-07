project(Math VERSION ${THOR_VERSION} LANGUAGES CXX)

set(Math_SRC
    decompose.c
    MatrixDecompose.cpp)

set(Math_H
    ${THOR_INCLUDE_DIR}/Thor/Math/Allocators.h
    ${THOR_INCLUDE_DIR}/Thor/Math/AxisAlignedBox.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Constants.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Decompose.h
    ${THOR_INCLUDE_DIR}/Thor/Math/EulerAngles.h
    ${THOR_INCLUDE_DIR}/Thor/Math/EulerAnglesFwd.h
    ${THOR_INCLUDE_DIR}/Thor/Math/ExpressionNodes.h
    ${THOR_INCLUDE_DIR}/Thor/Math/FixedMatrix.h
    ${THOR_INCLUDE_DIR}/Thor/Math/FixedMatrixFwd.h
    ${THOR_INCLUDE_DIR}/Thor/Math/FixedMatrixGeneric.inl
    ${THOR_INCLUDE_DIR}/Thor/Math/FixedVector.h
    ${THOR_INCLUDE_DIR}/Thor/Math/FixedVectorFwd.h
    ${THOR_INCLUDE_DIR}/Thor/Math/FixedVectorGeneric.inl
    ${THOR_INCLUDE_DIR}/Thor/Math/IBoundingVolume.h
    ${THOR_INCLUDE_DIR}/Thor/Math/IntersectionTests.h
    ${THOR_INCLUDE_DIR}/Thor/Math/LinearSystemSolver.h
    ${THOR_INCLUDE_DIR}/Thor/Math/ListConstructor.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Math.h
    ${THOR_INCLUDE_DIR}/Thor/Math/MathSSE.h
    ${THOR_INCLUDE_DIR}/Thor/Math/MathSSEasm.h
    ${THOR_INCLUDE_DIR}/Thor/Math/MatrixDecompose.h
    ${THOR_INCLUDE_DIR}/Thor/Math/OpTags.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Operators.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Plane.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Quaternion.h
    ${THOR_INCLUDE_DIR}/Thor/Math/QuaternionGeneric.inl
    ${THOR_INCLUDE_DIR}/Thor/Math/Ray.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Sphere.h
    ${THOR_INCLUDE_DIR}/Thor/Math/ThorMath.h
    ${THOR_INCLUDE_DIR}/Thor/Math/ThorMathExport.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Transform.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsCurve.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsCurve.inl
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsCurveTool.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsCurveTool.inl
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsExceptions.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsSurface.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsSurface.inl
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsSurfaceTool.h
    ${THOR_INCLUDE_DIR}/Thor/Math/Nurbs/NurbsSurfaceTool.inl)

list(SORT Math_SRC)
list(SORT Math_H)
add_library(Math ${Math_SRC} ${Math_H})
set_target_properties (Math PROPERTIES FOLDER Lib)
thor_add_precompiled_header(Math "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
thor_macos_target_setup_library(Math)
thor_ios_target_setup_library(Math)

target_compile_options(Math PUBLIC -msse4.2)

target_include_directories(Math PUBLIC {$THOR_INCLUDE_DIR})
target_link_libraries(Math Core)