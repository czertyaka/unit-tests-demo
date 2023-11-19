#include <iostream>
#include <fstream>

#include "database.h"
#include "config.h"

int main() {
    std::ifstream ifs("config.json");
    if (ifs && ifs.good()) {
        db::SQLTransaction transaction("jdbc:mysql://localhost:3306/mysql");
        cfg::Parser parser(ifs);
        if (parser(transaction)) {
            return EXIT_SUCCESS;
        }
        std::cerr << "failed to parse 'config.json'\n";
        return EXIT_FAILURE;
    }
    std::cerr << "failed to open 'config.json'\n";
    return EXIT_FAILURE;
}
