//
// Created by YongGyu Lee on 2021/04/29.
//

#ifndef EYEDID_CORE_C_API_H_ // NOLINT(build/header_guard)
#define EYEDID_CORE_C_API_H_

// C API for Eyedid SDK
//
// C API is experimental, so the following interfaces
// are likely to be changed until the stable version is released.

#include <stdarg.h>
#include <stdint.h>

#include "c_def.h" // NOLINT(build/include_subdir)

#ifdef EMSCRIPTEN
#   include <emscripten.h>
#   define EYEDID_EXPORT EMSCRIPTEN_KEEPALIVE
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   ifdef EYEDID_EXPORT_DLL
#     define EYEDID_EXPORT __declspec(dllexport)
#   else
#     define EYEDID_EXPORT __declspec(dllimport)
#   endif
#elif defined __GNUC__ && __GNUC__ >= 4 && defined(__APPLE__) && defined(__MACH__)
#   define EYEDID_EXPORT __attribute__ ((visibility ("default")))
# else
#   define EYEDID_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Global functions

// Get version as string.
// Same with EyedidVersionInteger() + string suffix (suffix may be empty)
// Use EyedidVersionInteger() to parse version more easily
EYEDID_EXPORT const char* EyedidVersionString();

// Get version as integer. 9-digit integer, 00 00 00000 (major, minor, patch)
EYEDID_EXPORT int32_t EyedidVersionInteger();

// EyedidTracker
typedef struct EyedidTracker EyedidTracker;

// TODO(?): Merge Create and Init
EYEDID_EXPORT EyedidTracker* EyedidTrackerCreate(const char* license_key, uint32_t key_length);

EYEDID_EXPORT void EyedidTrackerDelete(EyedidTracker* obj);

// TODO(?): handle invalid settings
EYEDID_EXPORT void EyedidTrackerInit(EyedidTracker* obj, const EyedidTrackerOptions* options);

EYEDID_EXPORT void EyedidTrackerSetCameraFOV(EyedidTracker* obj, float fov);

EYEDID_EXPORT float EyedidTrackerGetCameraFOV(EyedidTracker* obj);

EYEDID_EXPORT void EyedidTrackerDeInit(EyedidTracker* obj);

EYEDID_EXPORT int EyedidTrackerInitialized(const EyedidTracker* obj);

EYEDID_EXPORT void EyedidTrackerSetFPS(EyedidTracker* obj, int32_t fps);

EYEDID_EXPORT void EyedidTrackerSetFaceDistance(EyedidTracker* obj, int32_t distance_mm);

EYEDID_EXPORT int EyedidTrackerAddFrame(EyedidTracker* obj, int64_t timestamp, uint8_t* buffer,
                                      int32_t width, int32_t height);

EYEDID_EXPORT int EyedidTrackerGetAuthorizationResult(const EyedidTracker* obj);

EYEDID_EXPORT void EyedidTrackerSetTargetBoundRegion(EyedidTracker* obj, float left, float top,
                                                   float right, float bottom);

EYEDID_EXPORT void EyedidTrackerStartCalibration(EyedidTracker* tracker,
                                                 const EyedidCalibrationPointNum& points,
                                                 const EyedidCalibrationAccuracy& accuracy,
                                                 float left,
                                                 float top,
                                                 float right,
                                                 float bottom,
                                                 EyedidBoolean use_previous_calibration = kEyedidFalse);

EYEDID_EXPORT void EyedidTrackerStartCollectSamples(EyedidTracker* obj);

EYEDID_EXPORT void EyedidTrackerStopCalibration(EyedidTracker* obj);

EYEDID_EXPORT void EyedidTrackerSetCalibrationData(EyedidTracker* obj, const float* data, uint32_t size);

typedef void(*EyedidOnMetrics      )(void* user_data, uint64_t timestamp, const EyedidData* data);
typedef void(*EyedidOnDrop      )(void* user_data, uint64_t timestamp);

typedef void(*EyedidOnCalibrationNextPoint)(void* user_data, float x, float y);
typedef void(*EyedidOnCalibrationProgress )(void* user_data, float progress);
typedef void(*EyedidOnCalibrationFinish   )(void* user_data, const float* data, uint32_t data_size);
typedef void(*EyedidOnCalibrationCancel   )(void* user_data, const float* data, uint32_t data_size);

// Attach callbacks to the EyedidTracker.
EYEDID_EXPORT void EyedidTrackerSetMetricsCallback(EyedidTracker* tracker,
                                                   EyedidOnMetrics on_metrics,
                                                   EyedidOnDrop on_drop);

EYEDID_EXPORT void EyedidTrackerSetCalibrationCallback(EyedidTracker* tracker,
                                                       EyedidOnCalibrationNextPoint on_calib_next_point,
                                                       EyedidOnCalibrationProgress on_calib_progress,
                                                       EyedidOnCalibrationFinish on_calib_finish,
                                                       EyedidOnCalibrationCancel on_calibration_cancel);

// Set custom data for callback output
EYEDID_EXPORT void EyedidTrackerSetCallbackUserData(EyedidTracker* tracker, void* user_data);

EYEDID_EXPORT void EyedidTrackerRemoveCallbackInterface(EyedidTracker* obj);

EYEDID_EXPORT void EyedidTrackerSetAttentionRegion(EyedidTracker* obj,
                                                 float left, float top, float right, float bottom);

EYEDID_EXPORT int EyedidTrackerGetAttentionRegion(const EyedidTracker* obj, float* dst);

EYEDID_EXPORT void EyedidTrackerRemoveAttentionRegion(EyedidTracker* obj);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // EYEDID_CORE_C_API_H_
