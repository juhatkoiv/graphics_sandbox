# Install script for directory: C:/Profiling/tracy-0.9

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Profiling/tracy-0.9/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Profiling/tracy-0.9/out/build/x64-Debug/TracyClient.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tracy" TYPE FILE FILES
    "C:/Profiling/tracy-0.9/public/tracy/TracyC.h"
    "C:/Profiling/tracy-0.9/public/tracy/Tracy.hpp"
    "C:/Profiling/tracy-0.9/public/tracy/TracyD3D11.hpp"
    "C:/Profiling/tracy-0.9/public/tracy/TracyD3D12.hpp"
    "C:/Profiling/tracy-0.9/public/tracy/TracyLua.hpp"
    "C:/Profiling/tracy-0.9/public/tracy/TracyOpenCL.hpp"
    "C:/Profiling/tracy-0.9/public/tracy/TracyOpenGL.hpp"
    "C:/Profiling/tracy-0.9/public/tracy/TracyVulkan.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/client" TYPE FILE FILES
    "C:/Profiling/tracy-0.9/public/client/tracy_concurrentqueue.h"
    "C:/Profiling/tracy-0.9/public/client/tracy_rpmalloc.hpp"
    "C:/Profiling/tracy-0.9/public/client/tracy_SPSCQueue.h"
    "C:/Profiling/tracy-0.9/public/client/TracyArmCpuTable.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyCallstack.h"
    "C:/Profiling/tracy-0.9/public/client/TracyCallstack.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyDebug.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyDxt1.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyFastVector.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyLock.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyProfiler.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyRingBuffer.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyScoped.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyStringHelpers.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracySysTime.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracySysTrace.hpp"
    "C:/Profiling/tracy-0.9/public/client/TracyThread.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/common" TYPE FILE FILES
    "C:/Profiling/tracy-0.9/public/common/tracy_lz4.hpp"
    "C:/Profiling/tracy-0.9/public/common/tracy_lz4hc.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyAlign.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyAlloc.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyApi.h"
    "C:/Profiling/tracy-0.9/public/common/TracyColor.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyForceInline.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyMutex.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyProtocol.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyQueue.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracySocket.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyStackFrames.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracySystem.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyUwp.hpp"
    "C:/Profiling/tracy-0.9/public/common/TracyYield.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyConfig.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyConfig.cmake"
         "C:/Profiling/tracy-0.9/out/build/x64-Debug/CMakeFiles/Export/share/Tracy/TracyConfig.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyConfig-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyConfig.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Tracy" TYPE FILE FILES "C:/Profiling/tracy-0.9/out/build/x64-Debug/CMakeFiles/Export/share/Tracy/TracyConfig.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Tracy" TYPE FILE FILES "C:/Profiling/tracy-0.9/out/build/x64-Debug/CMakeFiles/Export/share/Tracy/TracyConfig-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Profiling/tracy-0.9/out/build/x64-Debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
