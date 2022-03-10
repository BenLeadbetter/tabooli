#pragma once

#include <tabooli/combinator.hpp>
#include <tabooli/detail/overload.hpp>

namespace tabooli {

template<typename D>
struct visitor
{
    virtual ~visitor() = default;
    virtual void with_data(const D&) {}
    virtual void push_intersect() {}
    virtual void pop_intersect() {}
    virtual void push_unite() {}
    virtual void pop_unite() {}
    virtual void push_negate() {}
    virtual void pop_negate() {}
};

template<typename V, typename D>
void visit(V&& v, const combinator<D>& c)
{
    const auto overload = detail::overload(
        [&v](const D& data) {
            v.with_data(data);
        },
        [&v](const unite<D>& operands) {
            v.push_unite();
            for (const auto& operand : operands) 
            {
                const auto& c = combinator<D>{operand};
                tabooli::visit(v, operand);
            }
            v.pop_unite();
        },
        [&v](const intersect<D>& operands) {
            v.push_intersect();
            for (const auto& operand : operands) 
            {
                const auto& c = combinator<D>{operand};
                tabooli::visit(v, operand);
            }
            v.pop_intersect();
        },
        [&v](const negate<D>& n)
        {
            v.push_negate();
            tabooli::visit(v, n.data());
            v.pop_negate();
        },
        [](auto){}
    );
    std::visit(overload, c);
}

}