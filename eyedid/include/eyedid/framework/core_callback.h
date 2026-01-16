#ifndef EYEDID_FRAMEWORK_CORE_CALLBACK_H_
#define EYEDID_FRAMEWORK_CORE_CALLBACK_H_

#include <cstdint>

#include <memory>
#include <vector>

#include "eyedid/callback/icalibration_callback.h"
#include "eyedid/callback/itracking_callback.h"
#include "eyedid/framework/callback_dispatcher.h"
#include "eyedid/framework/c_def.h"
#include "eyedid/util/coord_converter_v2.h"

namespace eyedid {

class CoreCallback final : private internal::CallbackDispatcher<CoreCallback> {
 public:
  CoreCallback();

  void OnMetrics(uint64_t timestamp, const EyedidData* data);
  void OnDrop(uint64_t timestamp);

  void OnCalibrationProgress(float progress);
  void OnCalibrationNextPoint(float next_point_x, float next_point_y);
  void OnCalibrationFinished(std::vector<float> calib_data);
  void OnCalibrationCanceled(std::vector<float> calib_data);

  /**
   * Set callbacks
   * Note: these functions are not thread-safe
   * @param callback
   */

  void setTrackingCallback(ITrackingCallback* callback);
  void setCalibrationCallback(ICalibrationCallback* callback);

  /**
   * Set camera <-> display coordinate converter
   * Note: this function is not thread-safe
   * @param cc
   */
  void setConverter(CoordConverterV2<float>* cc);

 private:
  ITrackingCallback* tracking_callback;
  ICalibrationCallback* calibration_callback;
  const CoordConverterV2<float>* coord_converter_;
};

} // namespace eyedid

#endif // EYEDID_FRAMEWORK_CORE_CALLBACK_H_
