#pragma once

#include <cstdint>
#include <vector>
#include "vector_ref.h"

namespace dev
{
using byte = uint8_t;
using bytes = std::vector<byte>;
using bytesConstRef = vector_ref<byte const>;
using bytesRef = vector_ref<byte>;
}  // namespace dev