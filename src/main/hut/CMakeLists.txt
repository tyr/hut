#
# Submodules
#

# DB

set(${PROJECT_NAME}_DB_OBJECTS

    db/hut_db.c

)

# CLI

set(${PROJECT_NAME}_CLI_OBJECTS

    cli/hut_cli.c

)

#
# Set build options.
#

option(BUILD_SHARED "whether or not to build ${PROJECT_NAME} as a shared library" ON)
option(BUILD_CLI "whether or not to build ${PROJECT_NAME} CLI" ON)

#
# Set shared linker flags.
#

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")


#
# Use tcmalloc
#

#option(USE_TCMALLOC "Use Google Tcmalloc library" ON)
#if(USE_TCMALLOC)
#  find_package(Tcmalloc)
#endif(USE_TCMALLOC)

#
# Create libraries
#

# Static library

add_library(${PROJECT_NAME}_static STATIC ${${PROJECT_NAME}_DB_OBJECTS})
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_static tinycthread)
#target_link_libraries(${PROJECT_NAME}_static ${Tcmalloc_LIBRARIES})

# Shared library

if(BUILD_SHARED)
  add_library(${PROJECT_NAME}_shared SHARED ${${PROJECT_NAME}_DB_OBJECTS})
  set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
  target_link_libraries(${PROJECT_NAME}_shared tinycthread)
#  target_link_libraries(${PROJECT_NAME}_static ${Tcmalloc_LIBRARIES})
endif(BUILD_SHARED)

# CLI

if(BUILD_CLI)
  add_executable(${PROJECT_NAME}_cli ${${PROJECT_NAME}_CLI_OBJECTS})
  set_target_properties(${PROJECT_NAME}_cli PROPERTIES OUTPUT_NAME ${PROJECT_NAME}cli)
  target_link_libraries(${PROJECT_NAME}_cli tinycthread)
  target_link_libraries(${PROJECT_NAME}_cli ${PROJECT_NAME}_static)
#  target_link_libraries(${PROJECT_NAME}_static ${Tcmalloc_LIBRARIES})
endif(BUILD_CLI)
