#pragma once

#include "Common.h"
#include <string>

namespace dev
{
bytes aesCBCEncrypt(bytesConstRef _plainData, bytesConstRef _key);
bytes aesCBCDecrypt(bytesConstRef _cypherData, bytesConstRef _key);
std::string aesCBCEncrypt(const std::string& _plainData, const std::string& _key);
std::string aesCBCDecrypt(const std::string& _cypherData, const std::string& _key);
}  // namespace dev