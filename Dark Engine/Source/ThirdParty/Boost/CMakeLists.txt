add_library(Boost INTERFACE)



#target_compile_definitions(Boost INTERFACE BOOST_PYTHON_STATIC_LIB=1)
target_link_libraries(Boost INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/boost_python312-vc143-mt-x64-1_85.lib")
target_include_directories(Boost INTERFACE "include")
# target_sources(Boost PRIVATE "src/main.cpp")
