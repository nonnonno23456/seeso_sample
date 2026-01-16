//
// Created by David on 2024/07/29.
//

#ifndef EYEDID_FRAMEWORK_GLOBAL_INIT_H_
#define EYEDID_FRAMEWORK_GLOBAL_INIT_H_


// This file must only be included in gaze_tracker.cc

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <windows.h>

#include <iostream>
#include <exception>

#include "eyedid/framework/dll_function.h"

#define EYEDID_SET_DLL_FUNCTION_IMPL(hinst, target, name)  \
do {                                                \
  target.setFuncPtr(GetProcAddress(hinst, name));   \
  if (target == nullptr) {                          \
    std::cerr                                       \
      << "Failed to find " name " from dll\n";      \
    throw std::runtime_error(                       \
      "Failed to find " name " from dll\n");        \
  }                                                 \
} while (false)

#define EYEDID_SET_DLL_FUNCTION(hinst, name) \
  EYEDID_SET_DLL_FUNCTION_IMPL(hinst, name, #name)

#define EYEDID_SET_DLL_FUNCTION_PREFIX(prefix, hinst, name) \
  EYEDID_SET_DLL_FUNCTION_IMPL(hinst, prefix##name, #prefix #name)

#define EYEDID_DECLARE_DLL_FUNCTION_LINKER(name) \
  DLLFunction<decltype(::name)> name

namespace eyedid {
namespace { // NOLINT(build/namespaces_headers)
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidVersionString);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidVersionInteger);

EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerCreate);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerDelete);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerInit);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerDeInit);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetCameraFOV);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerGetCameraFOV);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerInitialized);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetFPS);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetFaceDistance);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetTargetBoundRegion);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerAddFrame);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerGetAuthorizationResult);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerStartCalibration);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerStartCollectSamples);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerStopCalibration);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetCalibrationData);

EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetMetricsCallback);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetCalibrationCallback);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetCallbackUserData);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerRemoveCallbackInterface);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerSetAttentionRegion);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerGetAttentionRegion);
EYEDID_DECLARE_DLL_FUNCTION_LINKER(EyedidTrackerRemoveAttentionRegion);


} // anonymous namespace

void global_init(const char* file) {
  auto procIDDLL = LoadLibrary(file);
  // It will be NULL even if a *file* is found, but when other dlls that this program needs are missing
  if (procIDDLL == NULL) {
    std::cerr
      << "Failed to load 'eyedid_core.dll'\n"
      << "Check if 'eyedid_core.dll' in in the same directory with an executable file.\n"
      << "Even if 'eyedid_core.dll' exists, loading can fail if one or more of the following files are missing:\n"
      << "'libcrypto-1_1-x64.dll', 'libcurl.dll', 'libssl-1_1-x64.dll', 'opencv_world410.dll'\n";
    throw std::runtime_error("Failed to load a dll");
  }

  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidVersionString);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidVersionInteger);

  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerCreate);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerDelete);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerInit);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerDeInit);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetCameraFOV);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerGetCameraFOV);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerInitialized);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetFPS);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetFaceDistance);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetTargetBoundRegion);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerAddFrame);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerGetAuthorizationResult);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerStartCalibration);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerStartCollectSamples);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerStopCalibration);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetCalibrationData);

  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetMetricsCallback);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetCalibrationCallback);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetCallbackUserData);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerRemoveCallbackInterface);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerSetAttentionRegion);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerGetAttentionRegion);
  EYEDID_SET_DLL_FUNCTION(procIDDLL, EyedidTrackerRemoveAttentionRegion);
}

} // namespace eyedid

#else

namespace eyedid {

void global_init(const char*) {}

} // namespace eyedid
#endif

#endif // EYEDID_FRAMEWORK_GLOBAL_INIT_H_
