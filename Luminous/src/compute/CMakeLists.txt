file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

add_library(luminous-compute SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(luminous-compute PUBLIC luminous-core luminous-ext)