project(Math VERSION ${THOR_VERSION} LANGUAGES CXX)

add_library(Math
		decompose.c
		MatrixDecompose.cpp
		ThMathReflection.cpp)

target_include_directories(Math PUBLIC {$THOR_INCLUDE_DIR})

target_link_libraries(Math Core)

install(TARGETS Math
    		ARCHIVE  DESTINATION lib/Thor
    		LIBRARY  DESTINATION lib/Thor
    		RUNTIME  DESTINATION bin/Thor)