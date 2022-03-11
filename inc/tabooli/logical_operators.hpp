#pragma once

#include <tabooli/detail/overload.hpp>
#include <tabooli/combinator.hpp>

#include <type_traits>

namespace tabooli {

namespace detail {

template< class T >
struct remove_cvref {
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template< class T >
using remove_cvref_t = typename remove_cvref<T>::type;

template<typename T, typename D>
inline constexpr bool is_combinator_v = std::is_same_v<combinator<D>, remove_cvref_t<T>>;
template<typename T, typename D>
using enable_if_combinator = std::enable_if<is_combinator_v<T, D>, combinator<D>>;
template<typename T, typename D>
using enable_if_combinator_t = typename enable_if_combinator<T, D>::type;

template<typename T, typename D>
inline constexpr bool is_intersect_v = std::is_base_of_v<intersect<D>, T>;
template<typename T, typename D>
using enable_if_intersect = std::enable_if<is_intersect_v<T, D>, intersect<D>>;
template<typename T, typename D>
using enable_if_intersect_t = typename enable_if_intersect<T, D>::type;

template<typename T, typename D>
inline constexpr bool is_unite_v = std::is_base_of_v<unite<D>, T>;
template<typename T, typename D>
using enable_if_unite = std::enable_if<is_unite_v<T, D>, unite<D>>;
template<typename T, typename D>
using enable_if_unite_t = typename enable_if_unite<T, D>::type;

template<typename T, typename D>
inline constexpr bool is_unary_operator_v = std::is_base_of_v<unary_operator<D>, T>;
template<typename T, typename D>
using enable_if_unary_operator = std::enable_if<is_unary_operator_v<T, D>, unary_operator<D>>;
template<typename T, typename D>
using enable_if_unary_operator_t = typename enable_if_unary_operator<T, D>::type;

} // namespace detail

template<
    typename T, typename D,
    typename = detail::enable_if_combinator_t<T, D>>
auto operator|(T&& c, D&& d)
{
    const auto overload = detail::overload(
        [&d](intersect<D>&& i) {
            return unite<D>{std::forward<intersect<D>>(i), std::forward<D>(d)};
        },
        [&d](unite<D>&& u) {
            u.append(std::forward<D>(d));
            return u;
        },
        [&d](negate<D>&& n) {
            return unite<D>{std::forward<negate<D>>(n), std::forward<D>(d)};
        },
        [&d](D&& cd) {
            return unite<D>{std::forward<D>(cd), std::forward<D>(d)};
        },
        [&d](const intersect<D>& i) {
            return unite<D>{i, std::forward<D>(d)};
        },
        [&d](const unite<D>& u) {
            auto copy = u;
            copy.append(std::forward<D>(d));
            return copy;
        },
        [&d](const negate<D>& n) {
            return unite<D>{n, std::forward<D>(d)};
        },
        [&d](const D& cd) {
            return unite<D>{cd, std::forward<D>(d)};
        },
        [](const auto&) {
            return unite<D>{};
        }
    );
    return std::visit(overload, std::forward<combinator<D>>(c));
}

}