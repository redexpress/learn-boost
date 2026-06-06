#include "lib/Common.h"
#include "lib/Base64.h"
#include "lib/AES.h"
#include "lib/Hash.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <iostream>
#include <sstream>
#include <string>

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
