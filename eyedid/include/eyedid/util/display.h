//
// Created by David on 2024/07/29.
//

#ifndef EYEDID_UTIL_DISPLAY_H_
#define EYEDID_UTIL_DISPLAY_H_

#include <cstdint>

#include <string>
#include <vector>
#include <tuple>

#include "eyedid/util/point.h"

namespace eyedid {

struct DisplayInfo {
  std::string displayName;
  std::string displayString;
  int displayStateFlag;
  std::string displayId;
  uint32_t displayId_num;
  std::string displayKey;
  float widthMm;
  float heightMm;
  int widthPx;
  int heightPx;
};

std::vector<DisplayInfo> getDisplayLists();

eyedid::Point<long> getWindowPosition(const std::string& windowName);

struct Rect {
  double x = 0, y = 0;
  double width = 0, height = 0;
};

Rect getWindowRect(const std::string& name);


} // namespace eyedid

#endif // EYEDID_UTIL_DISPLAY_H_
