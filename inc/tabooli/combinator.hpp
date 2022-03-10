#pragma once

#include <variant>
#include <vector>

namespace tabooli {

template<typename D> struct intersect;
template<typename D> struct unite;

template<typename D> 
struct  combinator : public std::variant<intersect<D>, unite<D>, D>
{
    using std::variant<intersect<D>, unite<D>, D>::variant;
};

template<typename D>
struct intersect : public std::vector<combinator<D>>
{
    using std::vector<combinator<D>>::vector;
};

template<typename D>
struct unite : public std::vector<combinator<D>>
{
    using std::vector<combinator<D>>::vector;
};

}