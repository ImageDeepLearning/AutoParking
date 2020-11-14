add_executable(calibration calibration.cpp)
target_link_libraries(calibration
        ${THIRD_PARTY_LIBS}
        )

add_executable(perspectivetomap perspectivetomap.cpp)
target_link_libraries(perspectivetomap
        ${THIRD_PARTY_LIBS}
        )
