project(Math VERSION ${THOR_VERSION} LANGUAGES CXX)

add_library(Math
		src/decompose.c
		src/MatrixDecompose.cpp
		src/ThMathReflection.cpp)

target_include_directories (Math PUBLIC  
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    		$<INSTALL_INTERFACE:include>
    		PRIVATE src)

target_link_libraries(Math Core)

install(TARGETS Math
    		ARCHIVE  DESTINATION lib/Thor
    		LIBRARY  DESTINATION lib/Thor
    		RUNTIME  DESTINATION bin/Thor)

install(DIRECTORY include/ DESTINATION include/Thor)