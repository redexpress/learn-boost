#include "lib/AES.h"

#include <openssl/evp.h>
#include <stdexcept>
#include <vector>

namespace dev
{
namespace
{
constexpr int AES_BLOCK_SIZE = 16;

void checkKeySize(bytesConstRef _key)
{
    if (_key.size() != 16 && _key.size() != 24 && _key.size() != 32)
        throw std::runtime_error("AES key must be 16, 24, or 32 bytes");
}

bytes aesCBC(bytesConstRef input, bytesConstRef key, bool encrypt)
{
    checkKeySize(key);
    bytesConstRef ivRef = key.cropped(0, AES_BLOCK_SIZE);
    bytes iv = ivRef.toBytes();

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        throw std::runtime_error("EVP_CIPHER_CTX_new failed");

    const EVP_CIPHER* cipher = nullptr;
    if (key.size() == 16)
        cipher = EVP_aes_128_cbc();
    else if (key.size() == 24)
        cipher = EVP_aes_192_cbc();
    else
        cipher = EVP_aes_256_cbc();

    bytes output(input.size() + AES_BLOCK_SIZE);

    int outLen = 0;
    int finalLen = 0;

    if (encrypt)
    {
        EVP_EncryptInit_ex(ctx, cipher, nullptr, key.data(), iv.data());
        EVP_EncryptUpdate(ctx, output.data(), &outLen, input.data(), input.size());
        EVP_EncryptFinal_ex(ctx, output.data() + outLen, &finalLen);
    }
    else
    {
        EVP_DecryptInit_ex(ctx, cipher, nullptr, key.data(), iv.data());
        EVP_DecryptUpdate(ctx, output.data(), &outLen, input.data(), input.size());
        if (EVP_DecryptFinal_ex(ctx, output.data() + outLen, &finalLen) != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("AES CBC decrypt failed: bad padding or wrong key");
        }
    }

    EVP_CIPHER_CTX_free(ctx);

    output.resize(outLen + finalLen);
    return output;
}
}  // namespace

bytes aesCBCEncrypt(bytesConstRef _plainData, bytesConstRef _key)
{
    return aesCBC(_plainData, _key, true);
}

bytes aesCBCDecrypt(bytesConstRef _cypherData, bytesConstRef _key)
{
    return aesCBC(_cypherData, _key, false);
}

std::string aesCBCEncrypt(const std::string& _plainData, const std::string& _key)
{
    bytesConstRef plainRef(
        reinterpret_cast<byte const*>(_plainData.data()), _plainData.size());
    bytesConstRef keyRef(reinterpret_cast<byte const*>(_key.data()), _key.size());
    bytes result = aesCBCEncrypt(plainRef, keyRef);
    return std::string(result.begin(), result.end());
}

std::string aesCBCDecrypt(const std::string& _cypherData, const std::string& _key)
{
    bytesConstRef cypherRef(
        reinterpret_cast<byte const*>(_cypherData.data()), _cypherData.size());
    bytesConstRef keyRef(reinterpret_cast<byte const*>(_key.data()), _key.size());
    bytes result = aesCBCDecrypt(cypherRef, keyRef);
    return std::string(result.begin(), result.end());
}
}  // namespace dev
