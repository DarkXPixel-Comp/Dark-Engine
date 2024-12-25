#add_library(Boost INTERFACE)

CPMAddPackage( 
	NAME Boost
	URL "https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.tar.xz"
)


#target_link_libraries(Fmt INTERFACE fmt::fmt)
