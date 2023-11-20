#include <string>

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

using namespace std::string_literals;

BOOST_AUTO_TEST_CASE(BasicAssertions) {
    BOOST_CHECK_NE("hello"s, "world"s);
    BOOST_CHECK_EQUAL(7 * 6, 42);
}
