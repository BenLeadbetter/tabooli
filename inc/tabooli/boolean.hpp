#pragma once

#include <tabooli/detail/expression.hpp>
#include <tabooli/detail/operators.hpp>
#include <tabooli/detail/operator_code.hpp>

#include <functional>
#include <stack>
#include <type_traits>

namespace tabooli {
    
template<typename D>
class boolean : protected detail::expression<D>
{
public:
    using detail::expression<D>::expression;
    using base = detail::expression<D>;
    boolean(const base& b) : base(b) {}
    boolean(base&& b) : base(std::move(b)) {}
    friend boolean operator|(boolean&& /*dummy*/, D&& d)
    {
        return boolean(std::move(d));
    }

    friend boolean operator|(boolean&& /*dummy*/, const D& d)
    {
        return boolean(std::move(d));
    }
    
    TABOOLI_BINARY_OPERATOR(boolean,&&,logical_and)
    TABOOLI_BINARY_OPERATOR(boolean,||,logical_or)
    TABOOLI_UNARY_OPERATOR(boolean,!,logical_not)
    
    template<typename Convert>
    bool evaluate(const Convert& c)
    {
        evaluate_visitor v(c);
        base::visit(v);
        return v.value;
    }
    
    bool evaluate() const
    {
        static_assert(std::is_convertible_v<D, bool>);
        evaluate_visitor v([](const auto& d){ return static_cast<bool>(d); });
        base::visit(v);
        return v.value;
    }

private:
    using base::visit;

    struct evaluate_visitor : public detail::expression_visitor<D>
    {
        bool value;
        std::stack<std::pair<
            detail::operator_code, 
            std::optional<bool>
        >> op_stack;
        using Convert = std::function<bool(const D&)>;
        const Convert& convert;

        evaluate_visitor(const Convert& c) : convert(c) {}
        void with_data(const D& d) override
        {
            merge_to_top(convert(d));
        }
        void push_operator(detail::operator_code c) override
        {
            op_stack.push(std::make_pair(c, std::nullopt));
        }
        void pop_operator(detail::operator_code c) override
        {
            auto top_op = op_stack.top();
            op_stack.pop();
            assert(top_op.second);
            merge_to_top(*top_op.second);
        }
        void merge_to_top(bool v)
        {
            if (op_stack.empty()) 
            {
                value = v;
                return;
            }
            auto& operator_value = op_stack.top().second;
            switch (op_stack.top().first) 
            {
                using namespace detail;
                case operator_code::logical_and:
                    if (!operator_value) operator_value = v;
                    else *operator_value &= v;
                    break;
                case operator_code::logical_or:
                    if (!operator_value) operator_value = v;
                    else *operator_value |= v;
                    break;
                case operator_code::logical_not:
                    assert(!operator_value);
                    operator_value = !v;
                    break;
                default:
                    assert(false);
            }
        }
    };
};

}

