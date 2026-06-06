#include "lib/Hash.h"

#include <openssl/evp.h>
#include <cstring>
#include <stdexcept>

namespace dev
{
namespace
{
bytes doHash(const EVP_MD* md, bytesConstRef _input, size_t outLen)
{
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx)
        throw std::runtime_error("EVP_MD_CTX_new failed");

    bytes output(outLen);
    unsigned int outLen2 = 0;

    if (EVP_DigestInit_ex(ctx, md, nullptr) != 1 ||
        EVP_DigestUpdate(ctx, _input.data(), _input.size()) != 1 ||
        EVP_DigestFinal_ex(ctx, output.data(), &outLen2) != 1)
    {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP digest failed");
    }

    EVP_MD_CTX_free(ctx);
    output.resize(outLen2);
    return output;
}

std::string toHex(bytes const& data)
{
    static const char hex[] = "0123456789abcdef";
    std::string ret;
    ret.reserve(data.size() * 2);
    for (auto b : data)
    {
        ret.push_back(hex[b >> 4]);
        ret.push_back(hex[b & 0x0f]);
    }
    return ret;
}
}  // namespace

bool sha3(bytesConstRef _input, bytesRef o_output)
{
    if (o_output.size() != 32)
        return false;
    auto result = doHash(EVP_sha3_256(), _input, 32);
    std::memcpy(o_output.data(), result.data(), 32);
    return true;
}

bytes sha3(bytesConstRef _input) { return doHash(EVP_sha3_256(), _input, 32); }

std::string sha3(std::string const& _input)
{
    bytesConstRef ref(reinterpret_cast<byte const*>(_input.data()), _input.size());
    return toHex(sha3(ref));
}

bytes sha256(bytesConstRef _input) { return doHash(EVP_sha256(), _input, 32); }

std::string sha256(std::string const& _input)
{
    bytesConstRef ref(reinterpret_cast<byte const*>(_input.data()), _input.size());
    return toHex(sha256(ref));
}

bytes ripemd160(bytesConstRef _input) { return doHash(EVP_ripemd160(), _input, 20); }

std::string ripemd160(std::string const& _input)
{
    bytesConstRef ref(reinterpret_cast<byte const*>(_input.data()), _input.size());
    return toHex(ripemd160(ref));
}
}  // namespace dev
