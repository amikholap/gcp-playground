set(GRPC_ROOT_DIR "${CONTRIB_DIR}/grpc")
set(GRPC_INCLUDE_DIR "${GRPC_ROOT_DIR}/include/grpcpp")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        grpc
        DEFAULT_MSG
        GRPC_ROOT_DIR
        GRPC_INCLUDE_DIR
)

mark_as_advanced(
        GRPC_ROOT_DIR
        GRPC_INCLUDE_DIR
)
