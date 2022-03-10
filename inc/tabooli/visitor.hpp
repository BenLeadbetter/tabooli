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
                const auto& c = combinator{operand};
                visit(v, operand);
            }
            v.pop_unite();
        },
        [&v](const intersect<D>& operands) {
            v.push_intersect();
            for (const auto& operand : operands) 
            {
                const auto& c = combinator{operand};
                visit(v, operand);
            }
            v.pop_intersect();
        }
    );
    std::visit(overload, c);
}

}