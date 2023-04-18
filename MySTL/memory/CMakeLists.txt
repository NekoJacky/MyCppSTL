aux_source_directory(. MemorySRC)

add_library(memory ${MemorySRC} alloc_demo.cpp alloc_demo.h)
