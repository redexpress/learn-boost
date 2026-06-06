#define BOOST_TEST_MODULE learn_boost_tests
#include <boost/test/unit_test.hpp>

#include <boost/json.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "devcore.h"
#include "lib/Common.h"
#include "lib/Base64.h"
#include "lib/AES.h"
#include "lib/Hash.h"
#include "json.h"
#include "sort.h"
#include "net.h"

#include <sstream>
#include <string>

BOOST_AUTO_TEST_CASE(test_base64_roundtrip)
{
    std::string text = "Hello, FISCO-BCOS!";
    dev::bytes input(text.begin(), text.end());

    std::string encoded = dev::toBase64(dev::bytesConstRef(input.data(), input.size()));
    BOOST_CHECK(!encoded.empty());

    dev::bytes decoded = dev::fromBase64(encoded);
    std::string result(decoded.begin(), decoded.end());
    BOOST_CHECK_EQUAL(result, text);
}

BOOST_AUTO_TEST_CASE(test_base64_known_value)
{
    dev::bytes input = {'M', 'a', 'n'};
    std::string encoded = dev::toBase64(dev::bytesConstRef(input.data(), input.size()));
    BOOST_CHECK_EQUAL(encoded, "TWFu");
}

BOOST_AUTO_TEST_CASE(test_uuid_generation)
{
    boost::uuids::random_generator gen;
    boost::uuids::uuid id1 = gen();
    boost::uuids::uuid id2 = gen();
    BOOST_CHECK(id1 != id2);
    BOOST_CHECK(!id1.is_nil());
}

BOOST_AUTO_TEST_CASE(test_aes_cbc_roundtrip)
{
    std::string plain = "Hello, FISCO-BCOS AES!";
    std::string key(32, '\0');
    for (size_t i = 0; i < key.size(); ++i)
        key[i] = static_cast<char>(i);

    std::string cipher = dev::aesCBCEncrypt(plain, key);
    BOOST_CHECK_EQUAL(cipher.size() % 16, 0u);
    BOOST_CHECK_GE(cipher.size(), plain.size());

    std::string decrypted = dev::aesCBCDecrypt(cipher, key);
    BOOST_CHECK_EQUAL(decrypted, plain);
}

BOOST_AUTO_TEST_CASE(test_sha3_known_value)
{
    std::string empty = "";
    std::string emptyHash = dev::sha3(empty);
    BOOST_CHECK_EQUAL(emptyHash,
        "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a");
}

BOOST_AUTO_TEST_CASE(test_sha256_known_value)
{
    std::string text = "abc";
    std::string hash = dev::sha256(text);
    BOOST_CHECK_EQUAL(hash,
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
}

BOOST_AUTO_TEST_CASE(test_ripemd160_known_value)
{
    std::string text = "";
    std::string hash = dev::ripemd160(text);
    BOOST_CHECK_EQUAL(hash, "9c1185a5c5e9fc54612808977ee8f548b2258d31");
}

BOOST_AUTO_TEST_CASE(test_json_serialize_parse)
{
    namespace json = boost::json;
    json::object data;
    data["name"] = "Alice";
    data["age"] = 30;
    data["active"] = true;

    std::string jsonStr = json::serialize(data);
    BOOST_CHECK(jsonStr.find("Alice") != std::string::npos);

    json::value parsed = json::parse(jsonStr);
    BOOST_CHECK_EQUAL(parsed.at("name").as_string().c_str(), "Alice");
    BOOST_CHECK_EQUAL(parsed.at("age").as_int64(), 30);
    BOOST_CHECK_EQUAL(parsed.at("active").as_bool(), true);
}

BOOST_AUTO_TEST_CASE(test_ini_parse)
{
    std::string iniData = R"(
[server]
host=127.0.0.1
port=8080
enable=true

[database]
name=fisco
)";

    std::stringstream ss(iniData);
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(ss, pt);

    BOOST_CHECK_EQUAL(pt.get<std::string>("server.host"), "127.0.0.1");
    BOOST_CHECK_EQUAL(pt.get<int>("server.port"), 8080);
    BOOST_CHECK_EQUAL(pt.get<bool>("server.enable"), true);
    BOOST_CHECK_EQUAL(pt.get<std::string>("database.name"), "fisco");
}

BOOST_AUTO_TEST_CASE(test_use_functions)
{
    BOOST_CHECK_NO_THROW(use_base64());
    BOOST_CHECK_NO_THROW(use_uuid());
    BOOST_CHECK_NO_THROW(use_aes());
    BOOST_CHECK_NO_THROW(use_hash());
    BOOST_CHECK_NO_THROW(use_ini());
    BOOST_CHECK_NO_THROW(use_json());
    BOOST_CHECK_NO_THROW(use_sort());
}