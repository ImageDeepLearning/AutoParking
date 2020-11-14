cmake_minimum_required(VERSION 3.14)
project(AutoParking)

# Top-Level CmakeLists.txt

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

####### DEPENDENCIES #######
find_package(OpenCV 3 REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "OPENCV is enabled")
    message(STATUS "OpenCV_INCLUDE_DIRS=${OpenCV_INCLUDE_DIRS}")
    message(STATUS "OpenCV_LIBS=${OpenCV_LIBS}")
else()
    message(STATUS "OPENCV is disabled or not found, " ${TARGET_NAME} " skiped")
    return()
endif()
#find_package(InferenceEngine 2.0 REQUIRED)

#find_package(Eigen3 REQUIRED)
####### INCLUDE #######
include_directories(
        ${PROJECT_SOURCE_DIR}
        #${PROJECT_SOURCE_DIR}/Thirdparty/UART/include
        #${PROJECT_SOURCE_DIR}/Thirdparty/fast/include
)

include_directories(
        Script/include
        Detection/include
        BirdView/include
        PackingLine/include
        utilite/include
)

####### SOURCES (Projects) #######
add_subdirectory(Detection)

add_subdirectory(BirdView)

add_subdirectory(utilite)

####### LIBS #######
# thirdparty
set(THIRD_PARTY_LIBS
        ${OpenCV_LIBS}
        #${EIGEN3_LIBS}
        #${BLAS_LIBRARIES}
        #${LAPACK_LIBRARIES}
        #${GLOG_LIBRARY}
        #${EvoBinoSDK_LIBRARIES}
        #boost_thread
        #boost_system
        )
# AF LIBS #
set(AP_LIBS
        #AP-dection
        AP-birdview
        #AP-packingline
        #	AF-util
        )

####### BUILDING MIAN #######
add_subdirectory(test)
add_subdirectory(Script)