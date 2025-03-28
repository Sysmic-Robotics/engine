set(LUA_VERSION "lua-5.4.7")
set(LUA_PREFIX "${CMAKE_BINARY_DIR}/${LUA_VERSION}")

include(ExternalProject)

ExternalProject_Add(lua_project
  PREFIX ${LUA_PREFIX}
  URL https://www.lua.org/ftp/${LUA_VERSION}.tar.gz
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} generic         # <-- changed from `make`
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE TRUE
  DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)

# Expose include and lib paths
ExternalProject_Get_Property(lua_project SOURCE_DIR)
set(LUA_INCLUDE_DIR "${SOURCE_DIR}/src" CACHE PATH "Lua include dir")
set(LUA_LIBRARY "${SOURCE_DIR}/src/liblua.a" CACHE FILEPATH "Lua static library")
