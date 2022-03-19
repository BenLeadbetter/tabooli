#pragma once

#include <tabooli/detail/operator_code.hpp>
#include <tabooli/detail/binary_tree.hpp>
#include <tabooli/detail/expression_node.hpp>

namespace tabooli::detail {

template<typename D>
struct expression_visitor
{
    virtual void with_data(const D&) {}
    virtual void push_operator(operator_code) {}
    virtual void pop_operator(operator_code) {}
};

template<typename D>
class expression : public binary_tree<expression_node<D>>
{
    using base = binary_tree<expression_node<D>>;

public:    
    using node = expression_node<D>;
    using const_iterator = typename base::const_iterator;
    using value_type = typename base::value_type;
    using const_reference = typename base::const_reference;
    using const_pointer = typename base::const_pointer;
    using size_type = typename base::size_type;
    using difference_type = typename base::difference_type;
    
    using binary_tree<expression_node<D>>::binary_tree;
    expression(const D& d) : base(expression_node<D>(d)) {}
    expression(D&& d) : base(expression_node<D>(std::move(d))) {}
    operator base() const { return *this; }
    
    using base::compose;
    using base::set_child;
    using base::set_children;
    
    template<typename V>
    void visit(V&& v) const
    {
        auto itr = base::begin();
        auto last_op = base::end();
        while(itr != base::end())
        {
            while (itr.parent() != last_op)
            {
                v.pop_operator(*last_op->op());
                last_op = last_op.parent();
            }
            if (itr->op())
            {
                last_op = itr;   
                v.push_operator(*itr->op());
            }
            else if (itr->data())
            {
                v.with_data(*itr->data());
            }
            std::advance(itr, 1);
        }
        while (last_op != base::end())
        {
            v.pop_operator(*last_op->op());
            last_op = last_op.parent();
        }
    }
    
    expression copy_operate(operator_code c) const
    {
        expression copy = *this;
        copy.compose(node(c));
        return copy;
    }

    expression& operate(operator_code c) 
    {
        compose(node(c));
        return *this;
    }

    expression copy_operate(
        operator_code c, 
        const expression<D>& other) const
    {
        expression copy = *this;
        copy.compose(node(c), other);
        return copy;
    }

    expression copy_operate(
        operator_code c, 
        expression<D>&& other) const
    {
        expression copy = *this;
        copy.compose(node(c), std::move(other));
        return copy;
    }

    expression& operate(
        operator_code c,
        const expression<D>& other) 
    {
        compose(node(c), other);
        return *this;
    }

    expression& operate(
        operator_code c,
        expression<D>&& other) 
    {
        compose(node(c), std::move(other));
        return *this;
    }
};

}