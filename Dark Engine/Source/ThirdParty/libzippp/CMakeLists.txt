add_library(LibZip INTERFACE)

find_package(libzippp CONFIG REQUIRED)

target_link_libraries(LibZip INTERFACE libzippp::libzippp)
