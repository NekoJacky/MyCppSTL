file(GLOB SRC "*.hpp")

aux_source_directory(. SRC)

add_library(src ${SRC})

include_directories(memory)
