add_library(AP-birdview SHARED
	src/BirdView.cpp
)

target_link_libraries( AP-birdview
    ${THIRD_PARTY_LIBS}
#    AF-Com AF-Mcon AF-util
)
