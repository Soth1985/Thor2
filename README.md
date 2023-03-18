# How to Build

## All Patforms

* Create a build folder named build_CHOSEN_PLATFORM for the rest of the document will be referred to as build
* open terminal at the repository root folder

## Windows Specific

## MacOS Specific

1. build folder is build_macos
2. Initialize conan dependencies
* conan install . --output-folder=build_macos --build=missing -s build_type=Debug --profile:build="conan/debug_profile_macos" --profile:host="conan/debug_profile_macos"
3. Generate Xcode project with cmake
*  cd build_macos
*  cmake .. -G Xcode

## iOS Specific
cmake .. -GXcode -DCMAKE_SYSTEM_NAME=iOS

## Building KTX libraries

1. To build for ios simulator, select any as build device in XCode and build
2. mac build: cmake -G Xcode -B build/mac -D KTX_FEATURE_STATIC_LIBRARY=ON -DCMAKE_BUILD_TYPE=Release
3. ios build: cmake -G Xcode -B build/ios -D CMAKE_SYSTEM_NAME=iOS -D KTX_FEATURE_STATIC_LIBRARY=ON