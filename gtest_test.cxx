#include <iostream>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "config.h"

using ::testing::AtMost;
using ::testing::Contains;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::IsSupersetOf;
using ::testing::Return;
using ::testing::UnorderedElementsAreArray;
using namespace std::string_literals;

// 1. Basics

TEST(ExampleTest, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}

TEST(ExampleTest, TrueFalseAssertions) {
    EXPECT_TRUE(true);
    EXPECT_FALSE("hello"s.empty());
}

TEST(ExampleTest, CompareAssertions) {
    EXPECT_GE(3, 3);
    EXPECT_GT(4, 3);
    EXPECT_NEAR(3.14, 3.145, 0.01);
    EXPECT_STREQ("hello", "hello");
}

TEST(ExampleTest, ThrowAssertions) {
    auto throwingInt = [](){ throw 1; };
    EXPECT_THROW(throwingInt(), int);
    auto throwingException = [](){ throw std::runtime_error("error!"); };
    EXPECT_THROW(throwingException(), std::exception);
    auto notThrowing = [](){};
    EXPECT_NO_THROW(notThrowing());
}

namespace cfg {

bool operator!=(const Config& lhs, const Config& rhs) {
    return lhs.name != rhs.name || lhs.count != rhs.count;
}

bool operator==(const Config& lhs, const Config& rhs) {
    return !(lhs != rhs);
}

std::ostream& operator<<(std::ostream& os, const Config& cfg) {
    os << "{ name: " << cfg.name << ", count: " << cfg.count << " }";
    return os;
}

} // namespace cfg

TEST(ExampleTest, CustimTypesCompareAssertions) {
    cfg::Config cfg1 = { "foo", 11 };
    cfg::Config cfg2 = { "foo", 12 };
    EXPECT_NE(cfg1, cfg2);
}

TEST(ExampleTest, MatchAssertions) {
    std::vector v = {1, 5, 2, 3, 4, 5};
    EXPECT_THAT(v, ElementsAreArray({1, 5, 2, 3, 4, 5}));
    EXPECT_THAT(v, UnorderedElementsAreArray({1, 2, 3, 4, 5, 5}));
    EXPECT_THAT(v, IsSupersetOf({1, 3, 5, 5}));
    EXPECT_THAT(v, Contains(5).Times(2));
}

#if 0
TEST(ExampleTest, AssertVsExpect) {
    // soft assertions
    EXPECT_EQ(1, 2);
    EXPECT_EQ(1, 3);
    EXPECT_EQ(1, 4);
    // hard assertion
    ASSERT_EQ(1, 5);
    // soft assertion again
    EXPECT_EQ(1, 6);
}
#endif

// 2. Fixtures

struct ParserTest : ::testing::Test {
    std::stringstream ss;
    cfg::Config cfg;
    void SetUp() override {
        // std::cout << __PRETTY_FUNCTION__ << '\n';
    }
    void TearDown() override {
        // std::cout << __PRETTY_FUNCTION__ << '\n';
    }
};

TEST_F(ParserTest, EmptyInput) {
    cfg::Parser parser(ss);
    EXPECT_FALSE(parser(cfg));
    EXPECT_EQ(cfg, cfg::Config());
}

TEST_F(ParserTest, InvalidJson) {
    // arrange
    ss << "not a json";
    cfg::Parser parser(ss);
    // act
    const bool result = parser(cfg);
    // assert
    EXPECT_FALSE(result);
    EXPECT_EQ(cfg, cfg::Config());
}

TEST_F(ParserTest, EmptyJson) {
    ss << "{}";
    cfg::Parser parser(ss);
    EXPECT_FALSE(parser(cfg));
    EXPECT_EQ(cfg, cfg::Config());
}

struct BadParserTest : ParserTest, ::testing::WithParamInterface<std::string> {};

TEST_P(BadParserTest, InvalidConfig) {
    ss << GetParam();
    cfg::Parser parser(ss);
    EXPECT_FALSE(parser(cfg));
    EXPECT_EQ(cfg, cfg::Config());
}

INSTANTIATE_TEST_SUITE_P(
    InvalidConfigs,
    BadParserTest,
    testing::Values(
        "",
        "not a json",
        "{}",
        "{ \"count\": 32 }",
        "{ \"name\": \"foo\"}",
        "{ \"name\": \"foo\", \"count\": {} }",
        "{ \"name\": \"foo\", \"count\": \"str\" }",
        "{ \"name\": \"foo\", \"count\": [] }",
        "{ \"name\": {}, \"count\": 1 }",
        "{ \"name\": [], \"count\": 1 }",
        "{ \"name\": 1, \"count\": 1 }",
        "{ \"name\": -1, \"count\": 1 }",
        "{ \"name\": 1.1, \"count\": 1 }"
    )
);

TEST_F(ParserTest, SuccessfulParse) {
    ss << "{ \"name\": \"foo\", \"count\": 1 }";
    cfg::Parser parser(ss);
    ASSERT_TRUE(parser(cfg));
    EXPECT_EQ(cfg, cfg::Config({"foo", 1u}));
}

// 3. Mock classes

class MockTransaction : public db::ITransaction {
public:
    MOCK_METHOD(bool, Begin, (), (override));
    MOCK_METHOD(bool, Update, (const std::string&, const std::string&), (override));
    MOCK_METHOD(bool, Commit, (), (override));
    MOCK_METHOD(bool, RollBack, (), (override));
};

struct TransactionParserTest : ParserTest {
    ::testing::StrictMock<MockTransaction> transaction;
};

TEST_F(TransactionParserTest, EmptyJson) {
    EXPECT_CALL(transaction, Begin()).Times(AtMost(1));
    EXPECT_CALL(transaction, RollBack()).Times(AtMost(1));
    cfg::Parser parser(ss);
    EXPECT_FALSE(parser(transaction));
}

TEST_F(TransactionParserTest, InvalidCountType) {
    ss << "{ \"name\": \"foo\", \"count\": {} }";
    EXPECT_CALL(transaction, Begin()).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(transaction, RollBack()).Times(1);
    EXPECT_CALL(transaction, Update(Eq("name"), Eq("foo"))).Times(1);
    cfg::Parser parser(ss);
    EXPECT_FALSE(parser(transaction));
}

TEST_F(TransactionParserTest, FailedToBegin) {
    ss << "{ \"name\": \"foo\", \"count\": 1 }";
    EXPECT_CALL(transaction, Begin()).WillRepeatedly(Return(false));
    cfg::Parser parser(ss);
    EXPECT_FALSE(parser(transaction));
}
