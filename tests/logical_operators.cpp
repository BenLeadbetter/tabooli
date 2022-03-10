#include <tabooli/logical_operators.hpp>
#include <tabooli/combinator.hpp>
#include <tabooli/ostream_operators.hpp>

#include <catch2/catch.hpp>

#include <string>

namespace tests {

SCENARIO("logical operators")
{
    using combinator  = tabooli::combinator<std::string>;
    using intersect  = typename combinator::intersect;
    using unite  = typename combinator::unite;
    using negate  = typename combinator::negate;

    GIVEN("a raw data combinator")
    {
        combinator data = "noise";
        
        WHEN("combine raw data with pipe operator")
        {
            auto combined = data | "kate mcaffrey";
            
            THEN("return type is an intersect")
            {
                CHECK(combined == intersect{"noise", "kat mcaffrey"});
            }
        }
    }
}

}