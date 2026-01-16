#include "eyedid/framework/core_callback.h"

#include <vector>

#include "eyedid/framework/c_def.h"

namespace eyedid {

namespace {

struct : public ITrackingCallback {
  void OnMetrics(uint64_t timestamp,
                 const EyedidGazeData& gaze_data,
                 const EyedidFaceData& face_data,
                 const EyedidBlinkData& blink_data,
                 const EyedidUserStatusData& user_status_data) override {}
  void OnDrop(uint64_t timestamp) override {}
} NullTrackingCallback;

struct : public ICalibrationCallback {
  void OnCalibrationProgress(float progress) override {}
  void OnCalibrationNextPoint(float x, float y) override {}
  void OnCalibrationFinish(const std::vector<float>& data) override {}
  void OnCalibrationCancel(const std::vector<float>& data) override {}
} NullCalibrationCallback;

const CoordConverterV2<float> default_coord_converter;

} // anonymous namespace


CoreCallback::CoreCallback()
: tracking_callback(&NullTrackingCallback),
  calibration_callback(&NullCalibrationCallback),
  coord_converter_(&default_coord_converter) {}

void CoreCallback::OnMetrics(uint64_t timestamp, const EyedidData* data) {
  EyedidGazeData gaze_data;
  gaze_data.movement_state = data->gaze.movement_state;
  gaze_data.tracking_state = data->gaze.tracking_state;
  float x = data->gaze.x;
  float y = data->gaze.y;
  if (x != -1001.f && y != -1001.f) {
    const auto p = coord_converter_->convert({x, y});
    x = p[0];
    y = p[1];
  }
  gaze_data.x = x;
  gaze_data.y = y;

  float fixation_x = data->gaze.fixation_x;
  float fixation_y = data->gaze.fixation_y;

  if(fixation_x != -1001.f && fixation_y != -1001.f) {
    const auto p = coord_converter_->convert({fixation_x, fixation_y});
    fixation_x = p[0];
    fixation_y = p[1];
  }

  gaze_data.fixation_x = fixation_x;
  gaze_data.fixation_y = fixation_y;

  tracking_callback->OnMetrics(timestamp, gaze_data, data->face, data->blink, data->user_status);
}

void CoreCallback::OnDrop(uint64_t timestamp) {
  tracking_callback->OnDrop(timestamp);
}

void CoreCallback::OnCalibrationProgress(float progress) {
  calibration_callback->OnCalibrationProgress(progress);
}

void CoreCallback::OnCalibrationNextPoint(float next_point_x, float next_point_y) {
  const auto px = coord_converter_->convert({next_point_x, next_point_y});
  calibration_callback->OnCalibrationNextPoint(px[0], px[1]);
}

void CoreCallback::OnCalibrationFinished(std::vector<float> calib_data) {
  calibration_callback->OnCalibrationFinish(calib_data);
}

void CoreCallback::OnCalibrationCanceled(std::vector<float> calib_data) {
  calibration_callback->OnCalibrationCancel(calib_data);
}

void CoreCallback::setTrackingCallback(eyedid::ITrackingCallback* callback) {
  tracking_callback = callback == nullptr ? &NullTrackingCallback : callback;
}

void CoreCallback::setCalibrationCallback(eyedid::ICalibrationCallback* callback) {
  calibration_callback = callback == nullptr ? &NullCalibrationCallback : callback;
}

void CoreCallback::setConverter(CoordConverterV2<float> *cc) {
  coord_converter_ = cc == nullptr ? &default_coord_converter : cc;
}

} // namespace eyedid
