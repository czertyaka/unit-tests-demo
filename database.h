#pragma once

#include <string>

namespace db {

class ITransaction {
public:
    virtual ~ITransaction() = default;
    virtual bool Begin() = 0;
    virtual bool Update(const std::string& key, const std::string& value) = 0;
    virtual bool Commit() = 0;
    virtual bool RollBack() = 0;
};

class SQLTransaction : public ITransaction {
public:
    SQLTransaction(const std::string& url);
    bool Begin() override;
    bool Update(const std::string& key, const std::string& value) override;
    bool Commit() override;
    bool RollBack() override;
};

} // namespace db
