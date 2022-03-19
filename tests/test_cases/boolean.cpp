#include <utility/test_data.hpp>

#include <tabooli/boolean.hpp>

#include <catch2/catch.hpp>

namespace tests {

SCENARIO("operator_expression")
{
    using boolean = tabooli::boolean<test_data>;

    test_data_ctor_listener listener;
    test_data_ctor_broadcaster::register_listener(&listener);

    GIVEN("a binary expression composed of lvalues")
    {
        auto lhs = boolean(test_data{0});
        auto rhs = boolean(test_data{0});
        auto ex = lhs && rhs;
        
        THEN("copy constructor is called twice")
        {
            CHECK(listener.copy_ctor_count() == 2);
        }
    }
    
    GIVEN("a binary expression composed of rvalues")
    {
        auto ex = boolean(test_data{0}) && boolean(test_data{1});
        
        THEN("copy constructor is not called")
        {
            CHECK(listener.copy_ctor_count() == 0);
        }
    }
    
    GIVEN("a ternary lvalue expression")
    {
        auto ex0 = boolean(test_data{0});
        auto ex1 = boolean(test_data{1});
        auto ex2 = boolean(test_data{2});
        auto ex = ex0 && ex1 && ex2;

        THEN("copy constructor is called three times")
        {
            CHECK(listener.copy_ctor_count() == 3);
        }
    }
    
    GIVEN("a ternary rvalue expression")
    {
        auto ex = boolean{} | test_data{0} && test_data{1} && test_data{2};

        THEN("copy constructor is not called")
        {
            CHECK(listener.copy_ctor_count() == 0);
        }
    }
    
    GIVEN("an expression with composed operations")
    {
        auto ex = (boolean{} | test_data{0} && test_data{1}) || !boolean{} | test_data{2};
    }
    
    GIVEN("composed raw data")
    {
        auto ex = boolean{} | test_data{1} && test_data{2};
        
        WHEN("evaluate")
        {
            bool result = ex.evaluate([](const auto& d){ return static_cast<bool>(d.val); });
            
            CHECK(result);
        }
    }
    
    GIVEN("composed of ints")
    {
        auto ex = tabooli::boolean<int>{} | 1 && 2 && 3;
    }

    GIVEN("composed of bools")
    {
        auto ex = tabooli::boolean<bool>{} | false && true && true; 

        WHEN("evaluate")
        {
            auto v = ex.evaluate();
            
            THEN("value equals false")
            {
                CHECK(!v);
            }
        }
    }
}

}