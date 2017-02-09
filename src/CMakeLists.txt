#
# Include subdirectories.
#

add_subdirectory(main/hut)

if(BUILD_TESTS)
  add_subdirectory(test/hut)
endif(BUILD_TESTS)
