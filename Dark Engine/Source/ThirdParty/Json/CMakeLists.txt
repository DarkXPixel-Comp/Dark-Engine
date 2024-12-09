add_library(JSON INTERFACE)

CPMAddPackage("gh:nlohmann/json@3.10.5")


target_link_libraries(JSON INTERFACE nlohmann_json::nlohmann_json)
