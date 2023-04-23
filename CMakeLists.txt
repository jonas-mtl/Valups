cmake_minimum_required(VERSION 3.0.0)
project(opencvtest VERSION 0.1.0)

set(TIFF_INCLUDE_DIR "D:/Dev/Vcpkg/installed/x64-windows/lib/")
set(TIFF_LIBRARY "tiff.lib")

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(opencvtest 
	"src/main.cpp"
	"src/core/screen-parser.cpp"
)

target_link_libraries( opencvtest ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
