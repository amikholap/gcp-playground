set(TBB_ROOT_DIR "${CONTRIB_DIR}/tbb")
set(TBB_INCLUDE_DIR "${TBB_ROOT_DIR}/include")

include(${TBB_ROOT_DIR}/cmake/TBBBuild.cmake)
tbb_build(TBB_ROOT ${TBB_ROOT_DIR} MAKE_ARGS extra_inc=big_iron.inc CONFIG_DIR TBB_DIR)
set(TBB_LIBRARY ${PROJECT_BINARY_DIR}/src/tbb_cmake_build/tbb_cmake_build_subdir_release/libtbb.a)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        TBB
        DEFAULT_MSG
        TBB_ROOT_DIR
        TBB_INCLUDE_DIR
        TBB_LIBRARY
)

mark_as_advanced(
        TBB_ROOT_DIR
        TBB_INCLUDE_DIR
        TBB_LIBRARY
)
