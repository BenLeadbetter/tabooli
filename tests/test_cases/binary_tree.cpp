#include <utility/test_data.hpp>

#include <tabooli/detail/binary_tree.hpp>

#include <catch2/catch.hpp>

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include <sstream>

namespace tests {

SCENARIO("binary_tree")
{
    using binary_tree = tabooli::detail::binary_tree<test_data>;

    BOOST_CONCEPT_ASSERT((boost::InputIterator<binary_tree::const_iterator>));
    BOOST_CONCEPT_ASSERT((boost::Container<binary_tree>));

    GIVEN("a binary_tree constructed from raw data")
    {
        auto bt = binary_tree(test_data{0});
        
        THEN("binary tree can be compared with itself")
        {
            CHECK(bt == bt);
        }
    }

    GIVEN("a binary_tree constructed from rvalue data")
    {
        test_data_ctor_listener listener;
        test_data_ctor_broadcaster::register_listener(&listener);
        auto bt = binary_tree(test_data{0});
        
        THEN("data copy constructor is not called")
        {
            CHECK(listener.copy_ctor_count() == 0);
        }
    }

    GIVEN("a binary tree constructed from another rvalue tree")
    {
        test_data_ctor_listener listener;
        test_data_ctor_broadcaster::register_listener(&listener);

        const auto bt = binary_tree(binary_tree(test_data{0}));
        
        THEN("data copy constructor is not called")
        {
            CHECK(listener.copy_ctor_count() == 0);
        }
    }

    GIVEN("a binary tree constructed from another lvalue tree")
    {
        test_data_ctor_listener listener;
        test_data_ctor_broadcaster::register_listener(&listener);

        const auto other = binary_tree(test_data{0});
        const auto bt = binary_tree(other);
        
        THEN("data copy constructor is called")
        {
            CHECK(listener.copy_ctor_count() == 1);
        }
    }
    
    GIVEN("a default constructed binary tree")
    {
        test_data_ctor_listener listener;
        test_data_ctor_broadcaster::register_listener(&listener);
        binary_tree tree{};
        
        WHEN("compose with rvalue raw data")
        {
            tree.compose(test_data{0});
            
            THEN("copy constructor not called")
            {
                CHECK(listener.copy_ctor_count() == 0);
            }
        }

        WHEN("compose with lvalue raw data")
        {
            auto data = test_data{0};
            tree.compose(data);
            
            THEN("copy constructor called")
            {
                CHECK(listener.copy_ctor_count() == 1);
            }
        }
    }

    GIVEN("a raw data binary tree")
    {
        auto tree = binary_tree(test_data{0});

        test_data_ctor_listener listener;
        test_data_ctor_broadcaster::register_listener(&listener);
        
        WHEN("compose with raw data")
        {
            tree.compose(test_data{1});
            
            THEN("data copy constructor is not called")
            {
                CHECK(listener.copy_ctor_count() == 0);
            }
        }
        
        WHEN("compose with lvalue tree")
        {
            auto other = binary_tree(test_data{2});
            tree.compose(test_data{1}, other);
            
            THEN("data copy constructor called")
            {
                CHECK(listener.copy_ctor_count() == 1);
            }
        }
        
        WHEN("compose with rvalue tree")
        {
            tree.compose(
                test_data{1},
                binary_tree(test_data{2}));
            
            THEN("data copy constructor not called")
            {
                CHECK(listener.copy_ctor_count() == 0);
            }
        }
        
        WHEN("set raw data rvalue child")
        {
            tree.set_child(test_data{1});

            THEN("data copy constructor not called")
            {
                CHECK(listener.copy_ctor_count() == 0);
            }
        }

        WHEN("set raw data lvalue child")
        {
            auto data = test_data{1};
            tree.set_child(data);

            THEN("data copy constructor called")
            {
                CHECK(listener.copy_ctor_count() == 1);
            }
        }

        WHEN("set tree rvalue child")
        {
            tree.set_child(binary_tree(test_data{1}));

            THEN("data copy constructor not called")
            {
                CHECK(listener.copy_ctor_count() == 0);
            }
        }

        WHEN("set tree lvalue child")
        {
            auto other = binary_tree(test_data{1});
            tree.set_child(other);

            THEN("data copy constructor called")
            {
                CHECK(listener.copy_ctor_count() == 1);
            }
        }
        
        WHEN("set rvalue children")
        {
            tree.set_children(test_data{1}, test_data{2});
            
            THEN("data copy constructor not called")
            {
                CHECK(listener.copy_ctor_count() == 0 );
            }
        }

        WHEN("set lvalue children")
        {
            auto f = test_data{1};
            auto s = test_data{2};
            tree.set_children(f, s);
            
            THEN("data copy constructor called twice")
            {
                CHECK(listener.copy_ctor_count() == 2 );
            }
        }
    }
    
    GIVEN("a default constructed tree")
    {
        binary_tree tree{};
        
        WHEN("copy the tree")
        {
            auto other = tree;
            
            THEN("copy is equal to original")
            {
                CHECK(other == tree);
            }
        }
        
        THEN("tree is equal to itself")
        {
            CHECK(tree == tree);
        }
        
        THEN("tree has size 0")
        {
            CHECK(tree.size() == 0);
        }
        
        THEN("tree is empty")
        {
            CHECK(tree.empty());
        }
        
        WHEN("compose with raw data")
        {
            tree.compose(test_data{0});

            THEN("tree has size 1")
            {
                CHECK(tree.size() == 1);
            }
            
            WHEN("compose again with two raw data nodes to form triangle")
            {
                tree.compose(test_data{1}, test_data{2});
                
                WHEN("compose with raw data again")
                {
                    tree.compose(test_data{3}, test_data{4});
                }
            }
            
            THEN("node contains composed data")
            {
                REQUIRE(!tree.empty());
                CHECK(*tree.begin() == test_data{0});
            }
        }
    }
    
    GIVEN("a raw data binary tree")
    {
        auto tree = binary_tree(test_data{0});
        
        THEN("tree is equal to itself")
        {
            CHECK(tree == tree);
        }
        
        WHEN("copy the tree")
        {
            auto other = tree;
            
            THEN("copy is equal to original")
            {
                CHECK(other == tree);
            }
        }
        
        WHEN("assign to new empy tree")
        {
            tree = binary_tree{};
            
            THEN("the tree is empty")
            {
                CHECK(tree.empty());
            }
        }
        
        WHEN("assign to a different tree with data")
        {
            auto other = binary_tree{1};
            tree = other;
            
            THEN("tree is equal to the other")
            {
                CHECK(tree == other);
            }
        }
        
        WHEN("compose with raw data")
        {
            tree.compose(test_data{1});
            
            THEN("tree is size 2")
            {
                CHECK(tree.size() == 2);
            }
            
            THEN("first value is composed data")
            {
                REQUIRE(!tree.empty());
                CHECK(*tree.begin() == test_data{1});
            }

            THEN("second element is the original data")
            {
                REQUIRE(tree.size() >= 2);
                CHECK(*std::next(tree.begin()) == test_data{0});
            }
        }
        
        WHEN("compose the first with a second")
        {
            tree.compose(
                test_data{1},
                binary_tree(test_data{2}));
            
            THEN("tree is size 3")
            {
                CHECK(tree.size() == 3);
            }
            
            THEN("the first item has value 1")
            {
                REQUIRE(!tree.empty());
                CHECK(*tree.begin() == test_data{1});
            }

            THEN("the second item has data from original")
            {
                REQUIRE(tree.size() >= 2);
                CHECK(*std::next(tree.begin()) == test_data{0});
            }

            THEN("the third item has data from composed bt")
            {
                REQUIRE(tree.size() >= 3);
                CHECK(*std::next(tree.begin(), 2) == test_data{2});
            }
        }
    }

    GIVEN("a complex binary tree")
    {
        auto tree = binary_tree(test_data{0});
        {
            auto tree1 = binary_tree(test_data{1});
            auto tree2 = binary_tree(test_data{2});
            {
                auto tree3 = binary_tree(test_data{3});
                auto tree5 = binary_tree(test_data{5});
                {
                    auto tree4 = binary_tree(test_data{4});
                    tree3.set_child(std::move(tree4));
                }
                tree2.set_children(
                    std::move(tree3),
                    std::move(tree5)
                );
            }
            tree.set_children(
                std::move(tree1),
                std::move(tree2)
            );
        }

        test_data_ctor_listener listener;
        test_data_ctor_broadcaster::register_listener(&listener);

        THEN("the tree has size 6")
        {
            CHECK(tree.size() == 6);
        }
        
        
        THEN("tree can be indexed using boost::adaptors::indexed")
        {
            CHECK(tree | boost::adaptors::indexed());
        }
        
        THEN("the elements are traversed in correct order")
        {
            for (const auto i : tree | boost::adaptors::indexed())
            {
                CHECK(i.value() == i.index());
            }
        }

        WHEN("copy the tree")
        {
            auto other = tree;
            
            THEN("the tree equals the copy")
            {
                CHECK(tree == other);
            }
            
            THEN("data copy constructor called 6 times")
            {
                CHECK(listener.copy_ctor_count() == 6);
            }
        }
    }
}

}