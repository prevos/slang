set(LIB_SOURCES
    # file
    libretro-common/file/config_file.c
    libretro-common/file/file_path.c
    libretro-common/file/archive_file_zlib.c
    libretro-common/file/nbio/nbio_intf.c
    libretro-common/file/nbio/nbio_stdio.c

    # streams
    libretro-common/streams/file_stream.c
    libretro-common/streams/interface_stream.c
    libretro-common/streams/memory_stream.c
    libretro-common/streams/trans_stream.c
    libretro-common/streams/trans_stream_pipe.c
    libretro-common/streams/trans_stream_zlib.c

    # vfs
    libretro-common/vfs/vfs_implementation.c

    # compat
    libretro-common/compat/compat_strcasestr.c

    # lists
    libretro-common/lists/string_list.c

    # formats
    libretro-common/formats/image_texture.c
    libretro-common/formats/image_transfer.c
    )

add_library(common STATIC ${LIB_SOURCES})
target_link_libraries(common z)
target_compile_definitions(common PRIVATE HAVE_ZLIB RARCH_INTERNAL)

option(HAVE_RPNG "Have PNG" ON)
if (HAVE_RPNG)
    target_compile_definitions(common PRIVATE HAVE_RPNG)
    target_sources(common PRIVATE libretro-common/formats/png/rpng.c)
endif()

target_include_directories(common PUBLIC libretro-common/include)