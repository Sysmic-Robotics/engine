include(ExternalProject)

# Define expected output files
set(PROTOBUF_BUILT_FILE "${CMAKE_BINARY_DIR}/protobuf-install/bin/protoc${CMAKE_EXECUTABLE_SUFFIX}")
set(PROTOBUF_SUBPATH "${CMAKE_INSTALL_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(PROTOC_SUBPATH "bin/protoc${CMAKE_EXECUTABLE_SUFFIX}")

# Only build if protoc doesn't exist yet
if(NOT EXISTS "${PROTOBUF_BUILT_FILE}")
    message(STATUS "Protobuf not built yet. Building from source...")

    ExternalProject_Add(project_protobuf
        URL https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.6.1.tar.gz
        URL_HASH SHA256=3d4e589d81b2006ca603c1ab712c9715a76227293032d05b26fca603f90b3f5b
        SOURCE_SUBDIR "cmake"
        DOWNLOAD_NO_PROGRESS true
        # PATCH_COMMAND cp ${CMAKE_CURRENT_LIST_DIR}/protobuf.CMakeLists.txt CMakeLists.txt
        DOWNLOAD_DIR "${DEPENDENCY_DOWNLOADS}"
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_MAKE_PROGRAM:PATH=${CMAKE_MAKE_PROGRAM}
            -DCMAKE_INSTALL_MESSAGE:STRING=NEVER
            -DCMAKE_BUILD_TYPE:STRING=Release
            "-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS} -std=gnu++11 -w"
            -Dprotobuf_BUILD_TESTS:BOOL=OFF
        STEP_TARGETS install
    )

    ExternalProject_Get_Property(project_protobuf install_dir)
    file(MAKE_DIRECTORY "${install_dir}/include")

    # Define library and compiler paths
    set(PROTOBUF_FOUND true)
    set(PROTOBUF_VERSION "3.6.1")
    set(PROTOBUF_INCLUDE_DIR "${install_dir}/include")
    set(PROTOBUF_INCLUDE_DIRS "${PROTOBUF_INCLUDE_DIR}")
    set(PROTOBUF_LIBRARY "${install_dir}/${PROTOBUF_SUBPATH}")
    set(PROTOBUF_LIBRARIES "${PROTOBUF_LIBRARY}")
    set(PROTOBUF_PROTOC_EXECUTABLE "${install_dir}/${PROTOC_SUBPATH}")
    set(Protobuf_PROTOC_EXECUTABLE "${install_dir}/${PROTOC_SUBPATH}")

    # Create imported targets
    if(NOT TARGET protobuf::protoc)
        add_executable(protobuf::protoc IMPORTED)
    endif()
    set_target_properties(protobuf::protoc PROPERTIES
        IMPORTED_LOCATION "${Protobuf_PROTOC_EXECUTABLE}"
    )

    if(NOT TARGET protobuf::libprotobuf)
        add_library(protobuf::libprotobuf STATIC IMPORTED)
    endif()
    set_target_properties(protobuf::libprotobuf PROPERTIES
        IMPORTED_LOCATION "${PROTOBUF_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_DIR}"
    )

    add_custom_target(protobuf_ready DEPENDS project_protobuf)

else()
    message(STATUS "Protobuf already built. Using cached build...")

    set(PROTOBUF_FOUND true)
    set(PROTOBUF_VERSION "3.6.1")
    set(PROTOBUF_INCLUDE_DIR "${CMAKE_BINARY_DIR}/protobuf-install/include")
    set(PROTOBUF_INCLUDE_DIRS "${PROTOBUF_INCLUDE_DIR}")
    set(PROTOBUF_LIBRARY "${CMAKE_BINARY_DIR}/protobuf-install/${PROTOBUF_SUBPATH}")
    set(PROTOBUF_LIBRARIES "${PROTOBUF_LIBRARY}")
    set(PROTOBUF_PROTOC_EXECUTABLE "${PROTOBUF_BUILT_FILE}")
    set(Protobuf_PROTOC_EXECUTABLE "${PROTOBUF_BUILT_FILE}")

    # Register imported targets
    if(NOT TARGET protobuf::protoc)
        add_executable(protobuf::protoc IMPORTED)
        set_target_properties(protobuf::protoc PROPERTIES
            IMPORTED_LOCATION "${PROTOBUF_PROTOC_EXECUTABLE}"
        )
    endif()

    if(NOT TARGET protobuf::libprotobuf)
        add_library(protobuf::libprotobuf STATIC IMPORTED)
        set_target_properties(protobuf::libprotobuf PROPERTIES
            IMPORTED_LOCATION "${PROTOBUF_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_DIR}"
        )
    endif()

    # Mark it as ready
    add_custom_target(protobuf_ready)
endif()
