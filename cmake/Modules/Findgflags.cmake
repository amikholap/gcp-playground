set(GFLAGS_ROOT_DIR "${CONTRIB_DIR}/gflags")
set(GFLAGS_INCLUDE_DIR "${GFLAGS_ROOT_DIR}/src")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        gflags
        DEFAULT_MSG
        GFLAGS_ROOT_DIR
        GFLAGS_INCLUDE_DIR
)

mark_as_advanced(
        GFLAGS_ROOT_DIR
        GFLAGS_INCLUDE_DIR
)
