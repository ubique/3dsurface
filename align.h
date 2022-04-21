#pragma once

#include <cstdint>

#define IS_ALIGNED(x, a)                                                       \
  (((reinterpret_cast<uintptr_t>(x)) & (static_cast<uintptr_t>((a)-1))) == 0)
