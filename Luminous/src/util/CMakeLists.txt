file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)


add_library(luminous-util SHARED ${HEADER_FILES} ${SOURCE_FILES})