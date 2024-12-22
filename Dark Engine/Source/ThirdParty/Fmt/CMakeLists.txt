add_library(Fmt INTERFACE)

CPMAddPackage("gh:fmtlib/fmt#7.1.3")

target_link_libraries(Fmt INTERFACE fmt::fmt)
