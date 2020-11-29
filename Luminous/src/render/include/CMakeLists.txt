file(GLOB HEADER_FILES *.h*)
file(GLOB SOURCE_FILES *.c*)

add_library(luminous-render SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(luminous-render PUBLIC luminous-core luminous-compute assimp_config)