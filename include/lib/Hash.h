#pragma once

#include "Common.h"
#include <string>

namespace dev
{
bool sha3(bytesConstRef _input, bytesRef o_output);
bytes sha3(bytesConstRef _input);
std::string sha3(std::string const& _input);

bytes sha256(bytesConstRef _input);
std::string sha256(std::string const& _input);

bytes ripemd160(bytesConstRef _input);
std::string ripemd160(std::string const& _input);
}  // namespace dev