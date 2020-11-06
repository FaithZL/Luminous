file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)


message(${HEADER_FILES})

add_library(luminous-util SHARED ${HEADER_FILES} ${SOURCE_FILES})