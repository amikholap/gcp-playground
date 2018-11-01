set(PROTOBUF_ROOT_DIR "${CONTRIB_DIR}/protobuf")
set(PROTOBUF_INCLUDE_DIR "${PROTOBUF_ROOT_DIR}/src")
set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIR})
set(PROTOBUF_LIBRARIES libprotobuf)
set(PROTOBUF_PROTOC_LIBRARIES libprotoc)
set(PROTOBUF_PROTOC_EXECUTABLE protoc)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        Protobuf
        DEFAULT_MSG
        PROTOBUF_ROOT_DIR
        PROTOBUF_INCLUDE_DIR
)

mark_as_advanced(
        PROTOBUF_ROOT_DIR
        PROTOBUF_INCLUDE_DIR
)
