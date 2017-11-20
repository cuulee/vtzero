#ifndef TEST_HPP
#define TEST_HPP

#include <catch.hpp>

#ifdef VTZERO_TEST_WITH_VARIANT
#include <boost/variant.hpp>
#include <unordered_map>
#endif

#include <stdexcept>

// Define vtzero_assert() to throw this error. This allows the tests to
// check that the assert fails.
struct assert_error : public std::runtime_error {
    explicit assert_error(const char* what_arg) : std::runtime_error(what_arg) {
    }
};

#define vtzero_assert(x) if (!(x)) { throw assert_error{#x}; }

#define vtzero_assert_in_noexcept_function(x) if (!(x)) { got_an_assert = true; }

extern bool got_an_assert;

#define REQUIRE_ASSERT(x) x; REQUIRE(got_an_assert); got_an_assert = false;

#include <vtzero/output.hpp>

std::string load_test_tile();

struct mypoint {
    int64_t p1;
    int64_t p2;
};

inline vtzero::point<2> create_vtzero_point(mypoint p) noexcept {
    return {static_cast<int32_t>(p.p1),
            static_cast<int32_t>(p.p2)};
}

#ifdef VTZERO_TEST_WITH_VARIANT
typedef boost::make_recursive_variant<
        std::string,
        float,
        double,
        int64_t,
        uint64_t,
        bool,
        std::vector< boost::recursive_variant_ >,
        std::unordered_map<std::string, boost::recursive_variant_>
    >::type variant_type;

class prop_visitor : public boost::static_visitor<> {
    std::string expected;

public:
    
    prop_visitor(std::string const& str) : expected(str) {}

    template <typename T>
    void operator() (T const&) const {
        REQUIRE(false);
    }

    void operator() (std::string const& str) const {
        REQUIRE(str == expected);
    }
};
#endif

#endif // TEST_HPP
