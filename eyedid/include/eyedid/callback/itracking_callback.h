//
// Created by David on 7/18/24.
//

#ifndef EYEDID_CALLBACK_ITRACKING_CALLBACK_H_
#define EYEDID_CALLBACK_ITRACKING_CALLBACK_H_

#include <cstdint>

#include "eyedid/framework/c_def.h"

namespace eyedid {

class ITrackingCallback {
 protected:
  ITrackingCallback(const ITrackingCallback&) = default;
  ITrackingCallback& operator=(const ITrackingCallback&) = default;

 public:
  ITrackingCallback() = default;
  virtual ~ITrackingCallback() = default;

  /**
   * This function is triggered when a new set of tracking metrics is available.
   *
   * @param timestamp           timestamp (passed by EyeTracker::AddFrame())
   * @param gaze_data           EyedidGazeData
   * @param blink_data          BlinkData
   * @param user_status_data    UserStatusData
   */
  virtual void OnMetrics(uint64_t timestamp,
                         const EyedidGazeData &gaze_data,
                         const EyedidFaceData &face_data,
                         const EyedidBlinkData &blink_data,
                         const EyedidUserStatusData &user_status_data) = 0;

 /**
  * Callback for handling frame drops due to performance issues.3
  *
  * This function is triggered when a frame is dropped instead of processed,
  * allowing tracking of lost data due to performance constraints.
  *
  * @param timestamp  timestamp (passed by EyeTracker::AddFrame())
  */
  virtual void OnDrop(uint64_t timestamp) = 0;
};

} // namespace eyedid

#endif // EYEDID_CALLBACK_ITRACKING_CALLBACK_H_
