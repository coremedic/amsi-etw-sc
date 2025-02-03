cmake_minimum_required(VERSION 3.30)
set(CMAKE_OSX_ARCHITECTURES x86_64)
project(asmi_etw_sc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

add_compile_options(
        -Os -fno-asynchronous-unwind-tables -nostdlib
        -fno-ident -fpack-struct=8 -falign-functions=1
        -s -ffunction-sections -falign-jumps=1 -w
        -falign-labels=1 -fPIC -Iinclude -masm=intel -fpermissive
)

add_link_options(
        -Wl,-T${CMAKE_SOURCE_DIR}/scripts/Linker.ld
        -Wl,-s,--no-seh,--enable-stdcall-fixup
        -Wl,--as-needed,--no-whole-archive
)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()


set(CMAKE_CXX_STANDARD 17)

add_executable(asmi_etw_sc
        src/main.cpp
        src/PreMain.cpp
        include/Common.h
)

target_link_libraries(asmi_etw_sc -nostdlib -nodefaultlibs)

set(ASM_SOURCE_FILES

)

foreach(ASM_SRC IN LISTS ASM_SOURCE_FILES)
    get_filename_component(ASM_OBJ ${ASM_SRC} NAME_WE)
    set(ASM_OBJ ${CMAKE_CURRENT_BINARY_DIR}/${ASM_OBJ}.obj)
    add_custom_command(OUTPUT ${ASM_OBJ}
            COMMAND nasm -f win64 -coff ${CMAKE_CURRENT_SOURCE_DIR}/${ASM_SRC} ${ASM_OBJ}
            DEPENDS ${ASM_SRC}
            COMMENT "Assembling ${ASM_SRC}"
    )
    target_sources(asmi_etw_sc PRIVATE ${ASM_OBJ})
endforeach()

add_custom_command(TARGET asmi_etw_sc POST_BUILD
        COMMAND python3 scripts/extract.py -f $<TARGET_FILE:asmi_etw_sc> -o ${CMAKE_CURRENT_SOURCE_DIR}/asmi_etw_sc.x64.bin
        COMMENT "Building shellcode: asmi_etw_sc.x64.bin"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
