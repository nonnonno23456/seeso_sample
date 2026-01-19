//
// Created by David on 2024-07-29.
//

#ifndef EYEDID_FRAMEWORK_DLL_FUNCTION_H_
#define EYEDID_FRAMEWORK_DLL_FUNCTION_H_

#include <windows.h>

#include <cstddef>
#include <type_traits>

namespace eyedid {

template<typename T> struct DLLFunction;

/** @brief dll function wrapper
 *
 * @tparam R        return type
 * @tparam Args     argument types
 */
template<typename R, typename ...Args>
class DLLFunction<R(Args...)> {
 public:
  using return_type = R;
  using fptr_type = std::add_pointer_t<R WINAPI(Args...)>;

  return_type operator()(Args... args)       { return fptr(args...); }
  return_type operator()(Args... args) const { return fptr(args...); }

  template<typename ...Args2>
  return_type operator()(Args2... args) {
//    static_assert(false, "");
    return fptr(static_cast<Args>(args)...);
  }

  template<typename Dummy = void,
      std::enable_if_t<
          std::is_void<Dummy>::value &&
          !std::is_same<fptr_type, FARPROC>::value,
      int> = 0>
  void setFuncPtr(fptr_type ptr) { fptr = ptr;             }

  void setFuncPtr(FARPROC ptr)   { fptr = (fptr_type)ptr;  }

  bool operator == (std::nullptr_t) const { return fptr == nullptr; }
  bool operator != (std::nullptr_t) const { return fptr != nullptr; }

 private:
  fptr_type fptr = nullptr;
};

} // namespace eyedid

#endif // EYEDID_FRAMEWORK_DLL_FUNCTION_H_
