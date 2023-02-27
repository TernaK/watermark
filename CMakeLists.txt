cmake_minimum_required(VERSION 3.24)
project(watermark)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(open_cv_libs 
    opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
set(asset_dir
    ${PROJECT_SOURCE_DIR}/assets)

add_executable(watermark main.cpp watermark.cpp watermark.h)
target_link_directories(watermark PRIVATE /usr/local/lib)
target_include_directories(watermark PRIVATE /usr/local/include)
target_link_libraries(watermark PRIVATE ${open_cv_libs})
target_compile_definitions(watermark PRIVATE -DASSETS_DIR=\"${asset_dir}\")
