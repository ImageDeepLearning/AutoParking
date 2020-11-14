add_library(AP-dection SHARED
	#src/System.cc
)

target_link_libraries( AP-dection
    ${THIRD_PARTY_LIBS}
#    AF-Com AF-Mcon AF-util
)
