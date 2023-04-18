aux_source_directory(. SRC)

add_subdirectory(memory)

add_library(MySTL ${SRC})

target_link_libraries(MySTL memory)
