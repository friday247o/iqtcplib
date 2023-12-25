find_path(LIBSNDFILE_INCLUDE_DIR sndfile.h)

set(LIBSNDFILE_NAMES ${LIBSNDFILE_NAMES} sndfile libsndfile)
find_library(LIBSNDFILE_LIBRARY NAMES ${LIBSNDFILE_NAMES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibSndFile DEFAULT_MSG LIBSNDFILE_LIBRARY LIBSNDFILE_INCLUDE_DIR)
