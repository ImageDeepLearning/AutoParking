add_executable(main main.cpp)
add_executable(birdview BirdView.cpp)
target_link_libraries(birdview
        ${AP_LIBS}
        ${THIRD_PARTY_LIBS}
        )

target_link_libraries(main
        ${THIRD_PARTY_LIBS}
        )