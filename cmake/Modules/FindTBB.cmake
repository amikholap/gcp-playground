set(TBB_ROOT_DIR "${CONTRIB_DIR}/tbb")
set(TBB_INCLUDE_DIR "${TBB_ROOT_DIR}/include")

include(${TBB_ROOT_DIR}/cmake/TBBBuild.cmake)
tbb_build(TBB_ROOT ${TBB_ROOT_DIR} CONFIG_DIR TBB_DIR)
include(${TBB_ROOT_DIR}/cmake/TBBConfig.cmake)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        TBB
        DEFAULT_MSG
        TBB_ROOT_DIR
        TBB_INCLUDE_DIR
)

mark_as_advanced(
        TBB_ROOT_DIR
        TBB_INCLUDE_DIR
)
