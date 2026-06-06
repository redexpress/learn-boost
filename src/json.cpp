#include <boost/json.hpp>
#include <iostream>

namespace json = boost::json;

void use_json() {
    json::object data;

    data["name"] = "Alice";
    data["age"] = 30;
    data["score"] = 98.6;
    data["active"] = true;

    json::array tags = {"cpp", "boost", "json"};
    data["tags"] = tags;

    json::object address;
    address["city"] = "New York";
    address["zip"] = "10001";
    data["address"] = address;

    std::string jsonStr = json::serialize(data);
    std::cout << "Serialized JSON: " << jsonStr << std::endl;

    std::string input = R"({"product":"laptop","price":1200.50})";
    json::value parsed = json::parse(input);
    std::cout << "Parsed product: " << parsed.at("product") << std::endl;

    if (parsed.as_object().contains("price")) {
        double price = parsed.at("price").as_double();
        std::cout << "Price: " << price << std::endl;
    }
}