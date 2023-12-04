
function(DownloadPortAudio VERSION DESTINATION VAR_NAME)

  set(FINAL_DIR ${DESTINATION}/portaudio)

  if(NOT EXISTS ${FINAL_DIR})

    set(DOWNLOAD_FILE ${CMAKE_BINARY_DIR}/paudio.tgz)

    message(STATUS "DOWNLOAD URL https://files.portaudio.com/archives/pa_stable_v${VERSION}.tgz")
    file(DOWNLOAD https://files.portaudio.com/archives/pa_stable_v${VERSION}.tgz ${DOWNLOAD_FILE} SHOW_PROGRESS)

    file(ARCHIVE_EXTRACT INPUT ${DOWNLOAD_FILE} DESTINATION ${DESTINATION})

    file(REMOVE ${DOWNLOAD_FILE})
  endif()

  set(${VAR_NAME} ${FINAL_DIR} PARENT_SCOPE)
endfunction()

function(DownloadLibSndFile VERSION DESTINATION VAR_NAME)
  if(WIN32)
    set(FINAL_DIR ${DESTINATION}/libsndfile-${VERSION}-win64)
  else()
    set(FINAL_DIR ${DESTINATION}/libsndfile-${VERSION})
  endif()

  if(NOT EXISTS ${FINAL_DIR})
    set(DOWNLOAD_FILE ${CMAKE_BINARY_DIR}/libsndfile.tgz)
    
    if(WIN32)
      set(DOWNLOAD_URL https://github.com/libsndfile/libsndfile/releases/download/${VERSION}/libsndfile-${VERSION}-win64.zip)
    else()
      set(DOWNLOAD_URL https://github.com/libsndfile/libsndfile/releases/download/${VERSION}/libsndfile-${VERSION}.tar.xz)
    endif()

    message(STATUS "DOWNLOAD URL ${DOWNLOAD_URL}")
    file(DOWNLOAD ${DOWNLOAD_URL} ${DOWNLOAD_FILE} SHOW_PROGRESS)

    file(ARCHIVE_EXTRACT INPUT ${DOWNLOAD_FILE} DESTINATION ${DESTINATION})

    file(REMOVE ${DOWNLOAD_FILE})
  endif()

  set(${VAR_NAME} ${FINAL_DIR} PARENT_SCOPE)
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
