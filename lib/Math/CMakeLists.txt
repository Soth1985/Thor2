project(Math VERSION ${THOR_VERSION} LANGUAGES CXX)

set(Math_SRC
    decompose.c
    MatrixDecompose.cpp
    ThMathReflection.cpp)

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
    ${THOR_INCLUDE_DIR}/Thor/Math/ThMathReflection.h
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

add_library(Math ${Math_SRC} ${Math_H})

target_compile_options(Math PUBLIC -msse4.2)
target_include_directories(Math PUBLIC {$THOR_INCLUDE_DIR})
target_link_libraries(Math Core)
export(TARGETS Math FILE Math.cmake)

install(TARGETS Math
    		ARCHIVE  DESTINATION lib/Thor
    		LIBRARY  DESTINATION lib/Thor
    		RUNTIME  DESTINATION bin/Thor)