message(STATUS "Building with Apple Metal support")

enable_language(OBJC)
enable_language(OBJCXX)

file(GLOB_RECURSE HEADER_FILES *.h*)
file(GLOB_RECURSE SOURCE_FILES *.mm)

luminous_add_backend(metal SOURCES ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(luminous-backend-metal PRIVATE
        "-framework Foundation"
        "-framework Metal"
        "-framework MetalKit"
        "-framework MetalPerformanceShaders")
