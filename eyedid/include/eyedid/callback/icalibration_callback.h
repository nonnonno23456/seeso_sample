//
// Created by David on 2024-07-29.
//

#ifndef EYEDID_CALLBACK_ICALIBRATION_CALLBACK_H_
#define EYEDID_CALLBACK_ICALIBRATION_CALLBACK_H_

#include <vector>

namespace eyedid {

class ICalibrationCallback {
 protected:
  ICalibrationCallback(const ICalibrationCallback&) = default;
  ICalibrationCallback& operator=(const ICalibrationCallback&) = default;

 public:
  ICalibrationCallback() = default;
  virtual ~ICalibrationCallback() = default;

  /** Calibration Progress Callback
   * @brief called during each calibration point(s)
   *
   * @param progress    progress(0.0 ~ 1.0)
   */
  virtual void OnCalibrationProgress(float progress) = 0;

  /** Next Point Callback
   * @brief called when calibration point changes
   *
   * @param x    new point x
   * @param y    new point y
   */
  virtual void OnCalibrationNextPoint(float x, float y) = 0;

  /** Calibration Finished Callback
   * @brief called when calibration is finished
   *
   * @param data      calibration data.
   *
   * This value can be stored to set calibration data on other eyedid::EyeTracker object.
   * You don't need to pass data to the same eyedid::EyeTracker object.
   *
   * @code{.cpp}
   *
   * eyedid::EyeTracker e1;
   * auto cb = new Callback(); // inherited from eyedid::CallbackInterface
   * e1.setCallbackInterface(cb);
   * e1.startCalibration(...);
   *
   * // assume that calibration for e1 is done.
   * // assume that calibration data can be got from cb
   *
   * eyedid::EyeTracker e2;
   * e2.setCalibrationData(cb.getData()); // no need to calibrate again!
   *
   * @endcode
   */
  virtual void OnCalibrationFinish(const std::vector<float>& data) = 0;

  /**
   * @brief Called when the calibration process is stopped.
   *
   * This function is triggered if the calibration process is canceled. If there are any successfully
   * calibrated targets, they will be provided in the data parameter. To apply this data,
   * please call setCalibrationData. If no targets were successfully calibrated, data will be nullptr.
   *
   * @param data  Calibration data containing successfully calibrated targets, or nullptr if none exist.
   */
  virtual void OnCalibrationCancel(const std::vector<float>& data) = 0;
};

} // namespace eyedid

#endif // EYEDID_CALLBACK_ICALIBRATION_CALLBACK_H_
