
function(DownloadPortAudio VERSION DESTINATION)
  if(NOT EXISTS ${DESTINATION}/portaudio)

    set(DOWNLOAD_FILE ${CMAKE_BINARY_DIR}/paudio.tgz)

    message(STATUS "DOWNLOAD URL https://files.portaudio.com/archives/pa_stable_v${VERSION}.tgz")
    file(DOWNLOAD https://files.portaudio.com/archives/pa_stable_v${VERSION}.tgz ${DOWNLOAD_FILE} SHOW_PROGRESS)

    file(ARCHIVE_EXTRACT INPUT ${DOWNLOAD_FILE} DESTINATION ${DESTINATION})

    file(REMOVE ${DOWNLOAD_FILE})
  endif()
endfunction()

function(DownloadDrLibs DESTINATION)
  if(NOT EXISTS ${DESTINATION}/drlibs)
    execute_process(
      COMMAND git clone --depth 1 https://github.com/mackron/dr_libs ${DESTINATION}/drlibs
    )
  endif()
endfunction()

# Get the version from the package.json
function(GetVersion)
  file(READ ${CMAKE_SOURCE_DIR}/package.json PACKAGE_JSON)
  string(JSON PACKAGE_VERSION GET ${PACKAGE_JSON} version)
  set(PACKAGE_VERSION ${PACKAGE_VERSION} PARENT_SCOPE)
endfunction()

# generate node.lib
function(GenerateNodeLIB)
  if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
    # Generate node.lib
    execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
  endif()
endfunction(GenerateNodeLIB)
