# CompileProtobuf.cmake

function(define_proto_generation_target TARGET_NAME PROTO_SRC_DIR OUT_DIR)
    file(GLOB_RECURSE PROTO_FILES "${PROTO_SRC_DIR}/*.proto")
    file(MAKE_DIRECTORY ${OUT_DIR})

    set(GENERATED_SRCS "")
    set(GENERATED_HDRS "")

    foreach(proto_file ${PROTO_FILES})
        get_filename_component(proto_name ${proto_file} NAME_WE)

        set(proto_src "${OUT_DIR}/${proto_name}.pb.cc")
        set(proto_hdr "${OUT_DIR}/${proto_name}.pb.h")

        add_custom_command(
            OUTPUT ${proto_src} ${proto_hdr}
            COMMAND ${Protobuf_PROTOC_EXECUTABLE}
            ARGS --proto_path=${PROTO_SRC_DIR}
                 --cpp_out=${OUT_DIR}
                 ${proto_file}
            DEPENDS ${proto_file}
            COMMENT "Generating ${proto_src} and ${proto_hdr}"
            VERBATIM
        )

        list(APPEND GENERATED_SRCS ${proto_src})
        list(APPEND GENERATED_HDRS ${proto_hdr})
    endforeach()

    # Mark generated sources so CMake does not expect them to exist at configure time
    set_source_files_properties(${GENERATED_SRCS} ${GENERATED_HDRS}
        PROPERTIES GENERATED TRUE)

    add_custom_target(${TARGET_NAME} DEPENDS ${GENERATED_SRCS})
    
    # Export file lists back to parent
    set(${TARGET_NAME}_SRCS ${GENERATED_SRCS} PARENT_SCOPE)
    set(${TARGET_NAME}_HDRS ${GENERATED_HDRS} PARENT_SCOPE)
endfunction()
