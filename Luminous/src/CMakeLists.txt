add_subdirectory(ext/spdlog)
add_library(spdlog_config INTERFACE)
target_link_libraries(spdlog_config INTERFACE spdlog::spdlog)