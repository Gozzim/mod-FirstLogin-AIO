# Add script files
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/FirstLogin.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/FirstLoginSC.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/FirstLogin.h")

# Add scripts to script loader
AC_ADD_SCRIPT_LOADER("FirstLogin" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

# Add config file
AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/FirstLogin.conf.dist")