#pragma once

#include <memory>
#include <variant>
#include <vector>

namespace tabooli {
    
template<typename D> struct intersect;
template<typename D> struct unite;
template<typename D> struct negate;

template<typename D> 
struct  combinator : public std::variant<std::monostate, intersect<D>, unite<D>, negate<D>, D>
{
    using std::variant<std::monostate, intersect<D>, unite<D>, negate<D>, D>::variant;
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

namespace detail {

template<typename D>
struct wrapper
{
    wrapper(const D& d) : data(std::make_unique<D>(d)) {}
    wrapper(D&& d) : data(std::make_unique<D>(std::move(d))) {}
    wrapper(wrapper&& other) = default;
    wrapper(const wrapper& other) { data = std::make_unique<D>(*other.data); }
    std::unique_ptr<D> data;
};

} // namespace detail

template<typename D>
struct negate : public detail::wrapper<combinator<D>> 
{
    using detail::wrapper<combinator<D>>::wrapper;
};

}