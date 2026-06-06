#pragma once

#include "Common.h"
#include <string>

namespace dev
{
std::string toBase64(bytesConstRef _in);
bytes fromBase64(std::string const& _in);
}  // namespace dev