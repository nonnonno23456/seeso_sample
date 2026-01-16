//
// Created by David on 2024-07-29.
//
#ifndef EYEDID_GAZE_TRACKER_H_
#define EYEDID_GAZE_TRACKER_H_

#include <vector>
#include <string>
#include <cstddef>

#include "eyedid/callback/icalibration_callback.h"
#include "eyedid/callback/itracking_callback.h"
#include "eyedid/framework/c_def.h"
#include "eyedid/framework/core_callback.h"
#include "eyedid/util/point.h"
#include "eyedid/util/coord_converter_v2.h"

namespace eyedid {

// This function must be called before any function or EyeTracker object is created.
// Only requires on Windows.
void global_init(const char* file = "eyedid_core.dll");

std::string getVersionStr();

int32_t getVersionInt();

class GazeTracker {
 public:
  using converter_type = CoordConverterV2<float>;

  /**
   * Create GazeTracker
   * CoordConverter must be given, which converts [millimeters in camera-coordinate] to [pixels in display coordinate]
   *
   * You can use makeCameraToDisplayConverter() or makeDefaultCameraToDisplayConverter().
   * If you want to use raw-coordinate[millimeters in camera-coordinate], use makeNoOpConverter()
   *
   * You can change them later using GazeTracker::converter()
   *
   * @param coord_converter
   */
  explicit GazeTracker(const converter_type& coord_converter = makeNoOpConverter<float>());

  ~GazeTracker();

  /**
   * The construction of GazeTracker requires authentication. This example will pass EyedidTrackerOptions to configure GazeTracker.
   * After initialization, the authentication result will be logged with a status code.
   *
   * @param license_key Key generated from Eyedid SDK Console
   * @param options These are the options related to configuring GazeTracker.
   * @return Returns 0 if initialization is successful. Otherwise, returns an error code indicating the type of failure.
   */
  int initialize(const std::string& license_key,
                 const EyedidTrackerOptions& options = EyedidTrackerOptions());

  /**
   * Deinitialize the internal object. Calling other member functions (except for initialize) after calling deinitialize() results in undefined behavior.
   */
  void deinitialize();

  /**
   * Checks if the tracker has been successfully initialized.
   *
   * This function verifies whether the GazeTracker has been properly
   * initialized and is ready for use. It is useful for ensuring that
   * all necessary setup steps have been completed before attempting
   * to use tracker functionalities.
   *
   * @return true if initialized, false otherwise.
   */
  bool isTrackerInitialized() const;
  /**
   * Set face distance from the camera in cm. To get better GazeTracking performance, accurate face distance should be set by this function.
   * Default value is 60 (cm).
   * @param cm Distance between user face and camera in cm.
   */
  void setFaceDistance(int cm);

  /**
   * Gets the current face-to-camera distance in centimeters.
   *
   * @return Distance between user face and camera in cm.
   */
  int getFaceDistance() const;

  /**
   * If you know the horizontal field of view (FOV) of the webcam, you can set it using this function.
   * This can help improve accuracy compared to using the setFaceDistance function.
   *
   * @param fov The camera's horizontal FOV (field of view) is specified as a floating-point value in radians.
   */
  void setCameraFOV(float fov);
  /**
   * Returns the current horizontal field of view (FOV) value of the camera.
   *
   * @return horizontal field of view (FOV).
   */
  float getCameraFOV();

  /**
   * This function sets the callback for gaze tracking metrics. Note that GazeTracker does not own the instance.
   *
   * @param listener Address of gaze tracking metrics listener instance
   */
  void setTrackingCallback(eyedid::ITrackingCallback* listener);

  /**
   * Set Calibration callback for GazeTracking.
   * Note that GazeTracker does not own the instance.
   *
   * @param listener Address of calibration listener instance.
   */
  void setCalibrationCallback(eyedid::ICalibrationCallback* listener);

  /**
   * Remove tracking callback interface for GazeTracking.
   */
  void removeTrackingCallback();
  /**
   * Remove calibration callback interface for GazeTracking.
   */
  void removeCalibrationCallback();

  /**
   * Set the maximum Input Frames-per-second (FPS) for GazeTracking. FPS can be lower than the set value due to device specifications.
   * It limits the calculation by omitting the given frame internally.
   * The default value is set to 30 FPS.
   *
   * @param fps Maximum FPS(frames per seconds) for gaze tracking
   */
  void setTrackingFps(int fps);

  /**
   * Add image frame into GazeTracker for GazeTracking. addFrame should be called continuously, and add image frame from the camera stream.
   * The image should be in RGB color format.
   *
   * @Note that some frames can be ignored inside Eyedid SDK, thus not calculated
   *
   * @param timestamp Timestamp of the given frame
   * @param buffer Image data buffer in RGB format. It is safe to release buffer after passing to GazeTracker since the buffer is deep-copied.
   * @param width Width of the image
   * @param height Height of the image
   * @return 	Returns false if the input frame is omitted, true otherwise.
   */
  bool addFrame(int64_t timestamp, uint8_t* buffer, int width, int height);

  /**
   * This function sets the gaze tracking area to a specific region. By default, the entire screen of the main monitor is used.
   *
   * @param left The left boundary of the tracking area.
   * @param top The top boundary of the tracking area.
   * @param right The right boundary of the tracking area.
   * @param bottom The bottom boundary of the tracking area.
   */
  void setTargetBoundRegion(float left, float top, float right, float bottom);

  /**
   * Start calibration process with CalibrationMode and Criteria.
   * calibrationMode is number of calibration points. criteria is option that manage calibration process and accuracy.
   *
   * @param num Number of points used for calibration
   * @param criteria Accuracy option of the calibration (Note: Currently, only the default implementation is available.)
   * @param left Distance to left edge of the calibration ROI
   * @param top Distance to top edge of the calibration ROI
   * @param right Distance to right edge of the calibration ROI
   * @param bottom Distance to bottom edge of the calibration ROI
   * @param use_previous_calibration Whether to use data from a previous calibration. If true, previous data will be reused; otherwise, calibration starts fresh.
   */
  void startCalibration(EyedidCalibrationPointNum num, EyedidCalibrationAccuracy criteria,
                        float left, float top, float right, float bottom, bool use_previous_calibration = false);
  /**
   * Stop the current calibration process.
   */
  void stopCalibration();

  /**
   * Start collect next calibration point data.
   * This should be called in onCalibrationNextPoint when need to collect new calibration point data.
   */
  void startCollectSamples();
  /**
   * Set calibration data without calibrating manually.
   *
   * @param calibration_data Stored calibration data
   */
  void setCalibrationData(const std::vector<float>& calibration_data);

  /**
   * Set Attention ROI(Region of Interest).
   *
   * @param left Set distance to the left edge of the ROI rectangle
   * @param top Set distance to the top edge of the ROI rectangle
   * @param right Set distance to the right edge of the ROI rectangle
   * @param bottom Set distance to the bottom edge of the ROI rectangle
   */
  void setAttentionRegion(float left, float top, float right, float bottom);

  /**
   * Get Attention ROI(Region of Interest).
   *
   * @return contains the value from setAttentionRegion if set, empty std::vector otherwise.
   */
  std::vector<float> getAttentionRegion() const;
  /**
   * Remove Attention ROI(Region of Interest).
   */
  void removeAttentionRegion();

  /**
   * Accesses GazeTracker's coordinate converter for custom transformations.
   *
   * This function provides access to the current coordinate converter instance,
   * allowing modification or customization of coordinate transformations used by GazeTracker.
   *
   * @return A reference to the current CoordConverter instance.
   */
  converter_type& converter() { return coord_converter_; }

  /**
   * Accesses GazeTracker's coordinate converter for read-only purposes.
   *
   * This function provides read-only access to the current coordinate converter instance,
   * allowing inspection of coordinate transformations used by GazeTracker without modification.
   *
   * @return A const reference to the current CoordConverter instance.
   */
  const converter_type& converter() const { return coord_converter_; }

 private:
  CoreCallback callback;
  converter_type coord_converter_;
  int face_distance_mm = 600; // mm

  // Use void pointer to hide C API when including this header
  void* tracker_object = nullptr;
};

} // namespace eyedid

#endif // EYEDID_GAZE_TRACKER_H_
