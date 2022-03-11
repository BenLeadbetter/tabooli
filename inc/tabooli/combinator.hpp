#pragma once

#include <tabooli/detail/unary_operator.hpp>
#include <tabooli/detail/nary_operator.hpp>

#include <variant>

namespace tabooli {
    
template<typename D> struct intersect;
template<typename D> struct unite;
template<typename D> struct negate;

template<typename D> 
struct  combinator final : public std::variant<intersect<D>, unite<D>, negate<D>, D>
{
    using intersect = intersect<D>;
    using unite = unite<D>;
    using negate = negate<D>;
    using data = D;

    using std::variant<intersect, unite, negate, D>::variant;
};

template<typename D>
struct intersect final : public detail::nary_operator<combinator<D>>
{
    using detail::nary_operator<combinator<D>>::nary_operator;
};

template<typename D>
struct unite final : public detail::nary_operator<combinator<D>>
{
    using detail::nary_operator<combinator<D>>::nary_operator;
};

template<typename D>
struct negate final : public detail::unary_operator<combinator<D>> 
{
    using detail::unary_operator<combinator<D>>::unary_operator;
};

}