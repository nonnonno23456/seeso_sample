//
// Created by YongGyu Lee on 2021/10/07.
//

#ifndef EYEDID_CORE_C_DEF_H_ // NOLINT(build/header_guard)
#define EYEDID_CORE_C_DEF_H_

#if __has_include(<cmath>)
#include <cmath>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/*
 * Eyedid Structs
 */
typedef enum EyedidBoolean {
  kEyedidFalse = 0,
  kEyedidTrue = 1,
} EyedidBoolean;

typedef struct EyedidTrackerOptions {
#ifdef IS_IOS
  EyedidBoolean use_blink;
  EyedidBoolean use_user_status;
  EyedidBoolean use_gaze_filter;
  EyedidBoolean stream_mode;
  float camera_fov;
  int max_concurrency;
#else
  EyedidBoolean use_blink = kEyedidFalse;
  EyedidBoolean use_user_status = kEyedidFalse;
  EyedidBoolean use_gaze_filter = kEyedidTrue;
  EyedidBoolean stream_mode = kEyedidTrue;
  float camera_fov = static_cast<float>(M_PI) / 4.f;
  int max_concurrency = 0;
#endif
} EyedidTrackerOptions;

// Available calibration point nums
typedef enum EyedidCalibrationPointNum {
  kEyedidCalibrationPointOne = 1,
  kEyedidCalibrationPointFive = 5,
} EyedidCalibrationPointNum;


// Calibration accuracy
typedef enum EyedidCalibrationAccuracy {
  kEyedidCalibrationAccuracyDefault = 0,
  kEyedidCalibrationAccuracyLow,
  kEyedidCalibrationAccuracyHigh,
} EyedidCalibrationAccuracy;

typedef enum EyedidTrackingState {
  kEyedidTrackingSuccess = 0,
  kEyedidTrackingFaceMissing,
  kEyedidTrackingGazeNotFound,
} EyedidTrackingState;

typedef enum EyedidEyeMovementState {
  kEyedidEyeMovementFixation = 0,
  kEyedidEyeMovementSaccade = 2,
  kEyedidEyeMovementUnknown = 3,
} EyedidEyeMovementState;

typedef struct EyedidGazeData {
  float x;
  float y;
  float fixation_x;
  float fixation_y;
  EyedidTrackingState tracking_state;
  EyedidEyeMovementState movement_state;
} EyedidGazeData;

typedef struct EyedidFaceData {
  float score;
  float left;
  float top;
  float right;
  float bottom;
  float yaw;
  float pitch;
  float roll;
  float center_x;
  float center_y;
  float center_z;
} EyedidFaceData;

typedef struct EyedidBlinkData {
  EyedidBoolean is_blink;
  EyedidBoolean is_blink_left;
  EyedidBoolean is_blink_right;
  float left_openness;
  float right_openness;
} EyedidBlinkData;

typedef struct EyedidUserStatusData {
  EyedidBoolean is_drowsy;
  float drowsiness_intensity;
  float attention_score;
} EyedidUserStatusData;

typedef struct EyedidData {
  EyedidGazeData gaze;
  EyedidFaceData face;
  EyedidBlinkData blink;
  EyedidUserStatusData user_status;
} EyedidData;

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // EYEDID_CORE_C_DEF_H_
