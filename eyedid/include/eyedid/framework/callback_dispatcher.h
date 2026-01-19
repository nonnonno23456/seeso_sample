//
// Created by David on 2024-07-29.
//

#ifndef EYEDID_FRAMEWORK_CALLBACK_DISPATCHER_H_
#define EYEDID_FRAMEWORK_CALLBACK_DISPATCHER_H_

#include <cstdint>
#include <vector>

#include "eyedid/framework/c_def.h"

namespace eyedid {
namespace internal {

/** Callback Object Dispatcher
 * @brief callback method dispatcher
 *
 * instead of CallbackInterface's virtual method, this class' static methods are passed to internal Eyedid object.
 * (since member function pointer cannot be expressed to C-style function pointers, and to avoid different vtable ABI)
 * Dispatcher function is called from internal Eyedid object,
 * and then the dispatcher functions calls CallbackInterface object's methods.
 *
 * @tparam Derived      Interface type
 */
template<typename Derived>
class CallbackDispatcher {
  static constexpr Derived* derived(void* ptr) { return static_cast<Derived*>(ptr); }
  static constexpr const Derived* derived(const void* ptr) { return static_cast<const Derived*>(ptr); }

 protected:
  ~CallbackDispatcher() = default;

 public:
  using derived_type = Derived;

  static void dispatchOnMetrics(void* obj, uint64_t timestamp, const EyedidData* data) {
    static auto OnMetrics = &derived_type::OnMetrics;
    (derived(obj)->*OnMetrics)(timestamp, data);
  }

  static void dispatchOnDrop(void* obj, uint64_t timestamp) {
    static auto OnDrop = &derived_type::OnDrop;
    (derived(obj)->*OnDrop)(timestamp);
  }

  static void dispatchOnCalibrationProgress(void* obj, float progress) {
    static auto OnCalibrationProgress = &derived_type::OnCalibrationProgress;
    (derived(obj)->*OnCalibrationProgress)(progress);
  }

  static void dispatchOnCalibrationNextPoint(void* obj, float next_point_x, float next_point_y) {
    static auto OnCalibrationNextPoint = &derived_type::OnCalibrationNextPoint;
    (derived(obj)->*OnCalibrationNextPoint)(next_point_x, next_point_y);
  }

  static void dispatchOnCalibrationFinished(void* obj, const float* data, uint32_t data_size) {
    static auto OnCalibrationFinished = &derived_type::OnCalibrationFinished;
    (derived(obj)->*OnCalibrationFinished)({data, data + data_size});
  }

  static void dispatchOnCalibrationCanceled(void* obj, const float* data, uint32_t data_size) {
    static auto OnCalibrationCanceled = &derived_type::OnCalibrationCanceled;
    (derived(obj)->*OnCalibrationCanceled)({data, data + data_size});
  }
};


} // namespace internal
} // namespace eyedid

#endif // EYEDID_FRAMEWORK_CALLBACK_DISPATCHER_H_
