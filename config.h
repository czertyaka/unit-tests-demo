#pragma once

#include <cstddef>
#include <istream>
#include <string>

#include "database.h"

namespace cfg {

struct Config {
    std::string name = "";
    std::size_t count = 0;
};

class Parser {
public:
    Parser(std::istream& is);
    bool operator()(db::ITransaction& transaction);
    bool operator()(Config& config);
private:
    std::istream& is_;
};

} // namespace cfg
