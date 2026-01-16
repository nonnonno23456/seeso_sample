//
// Created by David on 2024-07-29.
//

#include "eyedid/gaze_tracker.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "eyedid/framework/c_api.h"
#include "eyedid/framework/global_init.h"

static inline constexpr EyedidTracker* cast_tracker(void * ptr) {
  return static_cast<EyedidTracker*>(ptr);
}

static inline constexpr const EyedidTracker* cast_tracker(const void * ptr) {
  return static_cast<const EyedidTracker *>(ptr);
}

namespace eyedid {

std::string getVersionStr() {
  return EyedidVersionString();
}

int32_t getVersionInt() {
  return EyedidVersionInteger();
}

GazeTracker::~GazeTracker() {
  EyedidTrackerDelete(cast_tracker(tracker_object));
  tracker_object = nullptr;
}

GazeTracker::GazeTracker(const CoordConverterV2<float>& coord_converter)
    : coord_converter_(coord_converter) {
  callback.setConverter(&coord_converter_);
}

int GazeTracker::initialize(const std::string& license_key,
               const EyedidTrackerOptions& options) {
  if (tracker_object == nullptr) {
    tracker_object = EyedidTrackerCreate(license_key.c_str(), static_cast<uint32_t>(license_key.size()));
  }

  auto internal_code = EyedidTrackerGetAuthorizationResult(cast_tracker(tracker_object));
  if (internal_code != 0) {
    return internal_code + 2;
  }
  EyedidTrackerInit(cast_tracker(tracker_object), &options);


  using dispatcher = internal::CallbackDispatcher<CoreCallback>;

  EyedidTrackerSetMetricsCallback(cast_tracker(tracker_object),
                                 &dispatcher::dispatchOnMetrics,
                                 &dispatcher::dispatchOnDrop);

  EyedidTrackerSetCalibrationCallback(cast_tracker(tracker_object),
                                     &dispatcher::dispatchOnCalibrationNextPoint,
                                     &dispatcher::dispatchOnCalibrationProgress,
                                     &dispatcher::dispatchOnCalibrationFinished,
                                     &dispatcher::dispatchOnCalibrationCanceled);
  EyedidTrackerSetCallbackUserData(cast_tracker(tracker_object), std::addressof(callback));
  return 0;
}

void GazeTracker::deinitialize() {
  EyedidTrackerRemoveCallbackInterface(cast_tracker(tracker_object));
  EyedidTrackerDeInit(cast_tracker(tracker_object));
}

bool GazeTracker::isTrackerInitialized() const {
  return EyedidTrackerInitialized(cast_tracker(tracker_object)) == kEyedidTrue;
}

void GazeTracker::setTrackingFps(int fps) {
  EyedidTrackerSetFPS(cast_tracker(tracker_object), fps);
}

void GazeTracker::setFaceDistance(int cm) {
  face_distance_mm = cm * 10;
  EyedidTrackerSetFaceDistance(cast_tracker(tracker_object), face_distance_mm);
}

int GazeTracker::getFaceDistance() const {
  return face_distance_mm / 10;
}

void GazeTracker::setCameraFOV(float fov) {
  EyedidTrackerSetCameraFOV(cast_tracker(tracker_object), fov);
}

float GazeTracker::getCameraFOV() {
  return EyedidTrackerGetCameraFOV(cast_tracker(tracker_object));
}

void GazeTracker::setTargetBoundRegion(float left, float top, float right, float bottom) {
  auto tl = coord_converter_.revert({left, top});
  auto br = coord_converter_.revert({right, bottom});
  EyedidTrackerSetTargetBoundRegion(cast_tracker(tracker_object), tl[0], tl[1], br[0], br[1]);
}

void GazeTracker::startCalibration(EyedidCalibrationPointNum num, EyedidCalibrationAccuracy criteria,
                                   float left, float top, float right, float bottom, bool use_previous_calibration) {
  auto tl = coord_converter_.revert({left, top});
  auto br = coord_converter_.revert({right, bottom});
  auto kUsePrevCalibration = use_previous_calibration ? kEyedidTrue : kEyedidFalse;
  EyedidTrackerStartCalibration(
      cast_tracker(tracker_object), num, criteria, tl[0], tl[1], br[0], br[1], kUsePrevCalibration);
}

void GazeTracker::startCollectSamples() {
  EyedidTrackerStartCollectSamples(cast_tracker(tracker_object));
}

void GazeTracker::stopCalibration() {
  EyedidTrackerStopCalibration(cast_tracker(tracker_object));
}

void GazeTracker::setCalibrationData(const std::vector<float>& serial_data) {
  EyedidTrackerSetCalibrationData(cast_tracker(tracker_object),
                                  serial_data.data(),
                                 static_cast<uint32_t>(serial_data.size()));
}

bool GazeTracker::addFrame(int64_t timestamp, uint8_t* buffer, int width, int height) {
  return EyedidTrackerAddFrame(cast_tracker(tracker_object), timestamp, buffer, width, height) == kEyedidTrue;
}

void GazeTracker::setTrackingCallback(eyedid::ITrackingCallback* listener) {
  callback.setTrackingCallback(listener);
}

void GazeTracker::setCalibrationCallback(eyedid::ICalibrationCallback* listener) {
  callback.setCalibrationCallback(listener);
}

void GazeTracker::removeTrackingCallback() {
  callback.setTrackingCallback(nullptr);
}

void GazeTracker::removeCalibrationCallback() {
  callback.setCalibrationCallback(nullptr);
}

void GazeTracker::setAttentionRegion(float left, float top, float right, float bottom) {
  const auto tl = coord_converter_.revert({left, top});
  const auto br = coord_converter_.revert({right, bottom});
  EyedidTrackerSetAttentionRegion(cast_tracker(tracker_object), tl[0], tl[1], br[0], br[1]);
}

std::vector<float> GazeTracker::getAttentionRegion() const {
  float roi[4];
  const auto success = EyedidTrackerGetAttentionRegion(cast_tracker(tracker_object), roi);
  if (!success)
    return {};

  const auto tl = coord_converter_.convert({roi[0], roi[1]});
  const auto br = coord_converter_.convert({roi[2], roi[3]});
  return {tl[0], tl[1], br[0], br[1]};
}

void GazeTracker::removeAttentionRegion() {
  EyedidTrackerRemoveAttentionRegion(cast_tracker(tracker_object));
}

} // namespace eyedid
