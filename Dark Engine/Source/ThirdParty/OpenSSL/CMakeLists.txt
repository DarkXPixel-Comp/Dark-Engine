add_library(OpenSSL INTERFACE)

find_package(OpenSSL REQUIRED)

target_link_libraries(OpenSSL INTERFACE OpenSSL::SSL)
