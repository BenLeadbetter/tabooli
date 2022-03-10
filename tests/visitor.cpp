#include <catch2/catch.hpp>

#include <tabooli/combinator.hpp>
#include <tabooli/visitor.hpp>

#include <string>
#include <sstream>

namespace tests {
    
SCENARIO("visitor")
{
    using combinator = tabooli::combinator<std::string>;
    using intersect = tabooli::intersect<std::string>;
    using unite = tabooli::unite<std::string>;
    
    class visitor : public tabooli::visitor<std::string>
    {
    public:
        std::string str() const { return ss.str(); }
        void with_data(const std::string& s) override { ss << s; }
        void push_intersect() override { ss << "and("; }
        void pop_intersect() override { ss << ")"; }
        void push_unite() override { ss << "or("; }
        void pop_unite() override { ss << ")"; }
    private: 
        std::stringstream ss;
    };

    GIVEN("raw data")
    {
        combinator raw{"basic"};
        
        WHEN("visit")
        {
            visitor v;
            tabooli::visit(v, raw);
            
            THEN("data parsed correctly")
            {
                CHECK(v.str() == "basic");
            }
        }
    }

    GIVEN("composed raw data")
    {
        combinator composite = intersect{"one", "two"};

        WHEN("visit")
        { 
            visitor v;
            tabooli::visit(v, composite);
            
            THEN("data parsed correctly")
            {
                CHECK(v.str() == "and(onetwo)");
            }
        }
    }

    GIVEN("composed complex data")
    {
        combinator composite = unite{
            intersect{"one", "two"}, 
            "three", 
            unite{
                "four", 
                intersect{"five", "six", "seven"}
            }
        };

        WHEN("visit")
        {
            visitor v;
            tabooli::visit(v, composite);
            
            THEN("data parsed correctly")
            {
                CHECK(v.str() == "or(and(onetwo)threeor(fourand(fivesixseven)))");
            }
        }
    }
}

}