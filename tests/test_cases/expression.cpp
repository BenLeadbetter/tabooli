#include <utility/test_data.hpp>

#include <tabooli/detail/expression.hpp>

#include <catch2/catch.hpp>

#include <boost/range/combine.hpp>

#include <sstream>
#include <variant>
#include <initializer_list>

namespace tests {

SCENARIO("expression")
{
    using expression = tabooli::detail::expression<test_data>;
    using node = tabooli::detail::expression_node<test_data>;
    using node_data_type = std::variant<test_data, tabooli::detail::operator_code>;
    struct comparison
    {
        bool operator()(tabooli::detail::operator_code op)
        {
            return n->op() == op;
        }
        bool operator()(const test_data& d)
        {
            return n->data() && *n->data() == d;
        }
        const node* n{nullptr};
        bool operator()(const node& d, const node_data_type& t)
        {
            n = &d;
            return std::visit(*this, t);
        }
    };
    class visitor : tabooli::detail::expression_visitor<test_data> 
    {
    public:
        void push_operator(tabooli::detail::operator_code c) override
        {
            switch (c)
            {
                using namespace tabooli::detail;
                case operator_code::logical_and:
                    ss << "and(";
                    break;
                case operator_code::logical_or:
                    ss << "or(";
                    break;
                case operator_code::logical_not:
                    ss << "not(";
                    break;
                default:
                    assert(false);
            }
        }
        void pop_operator(tabooli::detail::operator_code) override { ss << ")"; }
        void with_data(const test_data& d) override { ss << d; }
        std::string str() const { return ss.str(); }
    private:
        std::stringstream ss;
    };
    
    GIVEN("raw data expression")
    {
        auto ex = expression(test_data{0});
        
        WHEN("visitor")
        {
            visitor v;
            ex.visit(v);
            
            THEN("traverses just that single node")
            {
                CHECK(v.str() == "0");
            }
        }
    }
    
    GIVEN("a chained binary expression")
    {
        using namespace tabooli::detail;
        auto ex = expression(test_data{0}).operate(
            operator_code::logical_and,
            expression(test_data{1}).operate(
                operator_code::logical_and,
                expression(test_data{2}).operate(
                    operator_code::logical_and,
                    expression(test_data{3}).operate(
                        operator_code::logical_and,
                        expression(test_data{4})
                    )
                )
            )
        );
        
        THEN("expression has size 10")
        {
            CHECK(ex.size() == 9);
        }
        
        THEN("elements are correct")
        {
            
            std::initializer_list<node_data_type> elems = {
                tabooli::detail::operator_code::logical_and,
                test_data{0},
                tabooli::detail::operator_code::logical_and,
                test_data{1},
                tabooli::detail::operator_code::logical_and,
                test_data{2},
                tabooli::detail::operator_code::logical_and,
                test_data{3},
                test_data{4},
            };
            
            for (const auto i : boost::combine(ex, elems))
            {
                CHECK(comparison{}(i.get<0>(), i.get<1>()));
            }
        }
        
        WHEN("visit")
        {
            visitor v;
            ex.visit(v);
            
            THEN("traverses the nested tree")
            {
                CHECK(v.str() == "and(0and(1and(2and(34))))");
            }
        }
    }

    GIVEN("a composed expression")
    {
        auto ex = expression(test_data{1}).operate(
            tabooli::detail::operator_code::logical_or,
            expression(test_data{2}).operate(
                tabooli::detail::operator_code::logical_not
            ).operate(
                tabooli::detail::operator_code::logical_and,
                expression(test_data{3})
            )
        );
        
        THEN("expression has size 6")
        {
            CHECK(ex.size() == 6);
        }
        
        THEN("the elements are correct")
        {
            std::initializer_list<node_data_type> elems = {
                tabooli::detail::operator_code::logical_or,
                test_data{1},
                tabooli::detail::operator_code::logical_and,
                tabooli::detail::operator_code::logical_not,
                test_data{2},
                test_data{3}
            };
            
            for (const auto i : boost::combine(ex, elems))
            {
                CHECK(comparison{}(i.get<0>(), i.get<1>()));
            }
        }
        
        WHEN("visit expression")
        {
            visitor v;
            ex.visit(v);
            
            THEN("traverses expression correctly")
            {
                CHECK(v.str() == "or(1and(not(2)3))");
            }
            
        }
    }
}

}