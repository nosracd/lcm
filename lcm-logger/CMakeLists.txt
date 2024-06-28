add_executable(lcm-logger lcm_logger.c glib_util.c)
target_include_directories(lcm-logger PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(lcm-logger lcm-static ${GLIB_LDFLAGS})

add_executable(lcm-logplayer lcm_logplayer.c)
target_link_libraries(lcm-logplayer lcm-static ${GLIB_LDFLAGS})

install(TARGETS
  lcm-logger
  lcm-logplayer
  DESTINATION bin
)

install(FILES
  lcm-logger.1
  lcm-logplayer.1
  DESTINATION share/man/man1
)
