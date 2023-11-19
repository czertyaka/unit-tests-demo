#include <iostream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

#include "config.h"

using json = nlohmann::json;

namespace cfg {

Parser::Parser(std::istream& is) : is_(is) {}

bool Parser::operator()(db::ITransaction& transaction) {
    transaction.Begin();
    try {
        json data = json::parse(is_);
        if (!transaction.Update("name", data["name"]) ||
            !transaction.Update("count", std::to_string(data["count"].get<std::size_t>())) ||
            !transaction.Commit())
        {
            throw std::runtime_error("transaction error");
        }
        return true;
    }
    catch (const std::exception& err) {
        transaction.RollBack();
        std::cerr << "failed to parse config: " << err.what() << '\n';
    }
    return false;
}

bool Parser::operator()(Config& config) {
    const Config backup = config;
    try {
        json data = json::parse(is_);
        config.name = data["name"];
        config.count = data["count"].get<std::size_t>();
        return true;
    }
    catch (std::exception& err) {
        config = backup;
        std::cerr << "failed to parse config: " << err.what() << '\n';
    }
    return false;
}

} // namespace cfg
