#include "database.h"

namespace db {

SQLTransaction::SQLTransaction(const std::string&) {}

bool SQLTransaction::Begin() {
    return true;
}

bool SQLTransaction::Update(const std::string&, const std::string&) {
    return true;
}

bool SQLTransaction::Commit() {
    return true;
}

bool SQLTransaction::RollBack() {
    return true;
}

} // namespace db
