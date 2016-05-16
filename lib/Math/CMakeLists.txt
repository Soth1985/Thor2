add_library(Math
		src/Math/decompose.c
		src/Math/MatrixDecompose.cpp
		src/Math/ThMathReflection.cpp)

target_include_directories (Math PUBLIC  
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    		$<INSTALL_INTERFACE:include>
    		PRIVATE src)

install(TARGETS Math
    		ARCHIVE  DESTINATION Thor/lib
    		LIBRARY  DESTINATION Thor/lib
    		RUNTIME  DESTINATION Thor/bin)

install(DIRECTORY include/ DESTINATION Thor/include)