#include "lib/Common.h"
#include "lib/Base64.h"
#include "lib/AES.h"
#include "lib/Hash.h"
#include "lib/Log.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/outcome.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <expected>

void use_base64() {
    std::string text = "Hello, FISCO-BCOS!";
    dev::bytes input(text.begin(), text.end());

    std::string encoded = dev::toBase64(dev::bytesConstRef(&input[0], input.size()));
    std::cout << "Base64 encoded: " << encoded << "\n";

    dev::bytes decoded = dev::fromBase64(encoded);
    std::string result(decoded.begin(), decoded.end());
    std::cout << "Base64 decoded: " << result << "\n";
}

void use_uuid() {
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    std::cout << "UUID: " << id << "\n";
}

void use_aes() {
    std::string plain = "Hello, FISCO-BCOS AES!";
    std::string key(32, '\0');
    for (size_t i = 0; i < key.size(); ++i)
        key[i] = static_cast<char>(i);

    std::string cipher = dev::aesCBCEncrypt(plain, key);
    std::cout << "AES encrypted size: " << cipher.size() << "\n";

    std::string decrypted = dev::aesCBCDecrypt(cipher, key);
    std::cout << "AES decrypted: " << decrypted << "\n";
}

void use_hash() {
    std::string text = "Hello, FISCO-BCOS!";
    std::cout << "SHA3-256: " << dev::sha3(text) << "\n";
    std::cout << "SHA-256:  " << dev::sha256(text) << "\n";
    std::cout << "RIPEMD-160: " << dev::ripemd160(text) << "\n";
}

void use_ini() {
    std::string iniData = R"(
[server]
host=127.0.0.1
port=8080
enable=true

[database]
name=fisco
user=admin
)";

    std::stringstream ss(iniData);
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(ss, pt);

    std::cout << "server.host = " << pt.get<std::string>("server.host") << "\n";
    std::cout << "server.port = " << pt.get<int>("server.port") << "\n";
    std::cout << "server.enable = " << pt.get<bool>("server.enable") << "\n";
    std::cout << "database.name = " << pt.get<std::string>("database.name") << "\n";
    std::cout << "database.user = " << pt.get<std::string>("database.user") << "\n";
}

void use_log() {
    LOG(INFO) << LOG_BADGE("test") << LOG_DESC("hello log") << LOG_KV("key", 42);
    LOG(WARNING) << LOG_BADGE("warn") << LOG_DESC("this is warning");
    LOG(ERROR) << LOG_BADGE("err") << LOG_DESC("error message");
}

void open_file();
void log(std::string_view, const std::source_location& loc = std::source_location::current());
std::expected<int, std::string>divide(int a, int b);
namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;
outcome::result<int> divide2(int a, int b);

void use_exception() {
    try {
        open_file();
    } catch (const std::exception& e) {
        log(std::string("exception: ") + e.what());
    }
    auto r = divide(10, 2);
    if (r) {
        std::cout << *r << "\n";
    } else {
        LOG(ERROR) << r.error();
    }
    r = divide(10, 0);
    if (r) {
        std::cout << *r << "\n";
    } else {
        LOG(ERROR) << r.error();
    }
    auto r2 = divide2(10, 0);
    if (r2) {
        std::cout << r2.value() << "\n";
    } else {
        LOG(ERROR) << r2.error().message() << "\n";
    }
}

void log(std::string_view msg, const std::source_location& loc){
    LOG(ERROR) << loc.file_name() << ":" << loc.line() << " " << msg << '\n';
}

void open_file() {
    throw std::runtime_error("Failed to open file");
}

std::expected<int, std::string> divide(int a, int b) {
    if (b == 0)
        return std::unexpected("Division by zero");
    return a / b;
}

outcome::result<int> divide2(int a, int b){
    if (b == 0)
        return std::make_error_code(std::errc::invalid_argument);
    return a / b;
}