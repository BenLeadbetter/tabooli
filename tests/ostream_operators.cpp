#include <tabooli/ostream_operators.hpp>

#include <catch2/catch.hpp>

#include <sstream>
#include <string>

TABOOLI_CUSTOM_PREFIX(tabooli::combinator<std::string>,"and","or","not"," ");

namespace tests {

SCENARIO("ostream operators")
{
    using combinator = tabooli::combinator<std::string>;
    using intersect = tabooli::intersect<std::string>;
    using unite = tabooli::unite<std::string>;

    GIVEN("a basic combinator")
    {
        combinator basic = "benga";
        
        WHEN("write to ostream")
        {
            std::stringstream os;
            os << basic;
            
            THEN("the raw data is written to the ostream")
            {
                CHECK(os.str() == "benga");
            }
        }
    }
    
    GIVEN("an intersect combinator")
    {
        combinator comb = intersect{"benga", "skream", "mala"};
        
        WHEN("write to ostream")
        {
            std::stringstream os;
            os << comb;
            
            THEN("the data is written as a bracketed list with intersect prefix")
            {
                CHECK(os.str() == "and{benga skream mala}");
            }
        }
    }

    GIVEN("a unite combinator")
    {
        combinator comb = unite{"joker", "pinch", "shackleton"};
        
        WHEN("write to ostream")
        {
            std::stringstream os;
            os << comb;
            
            THEN("the data is written as a bracketed list with unite prefix")
            {
                CHECK(os.str() == "or{joker pinch shackleton}");
            }
        }
    }
}

}