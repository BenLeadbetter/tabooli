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
inline constexpr bool is_nary_operator_v = std::is_base_of_v<nary_operator<D>, T>;
template<typename T, typename D>
using enable_if_nary_operator = std::enable_if<is_nary_operator_v<T, D>, nary_operator<D>>;
template<typename T, typename D>
using enable_if_nary_operator_t = typename enable_if_nary_operator<T, D>::type;

template<typename T, typename D>
inline constexpr bool is_unary_operator_v = std::is_base_of_v<unary_operator<D>, T>;
template<typename T, typename D>
using enable_if_unary_operator = std::enable_if<is_unary_operator_v<T, D>, unary_operator<D>>;
template<typename T, typename D>
using enable_if_unary_operator_t = typename enable_if_unary_operator<T, D>::type;

template<typename T, typename D>
inline constexpr bool is_operator_v = is_nary_operator_v<T, D> || is_unary_operator_v<T, D>;
template<typename T, typename D>
using enable_if_operator = std::enable_if<is_operator_v<T, D>, T>;
template<typename T, typename D>
using enable_if_operator_t = typename enable_if_operator<T, D>::type;

template<typename A, typename B, typename D>
inline constexpr bool matching_nary_operators_v = 
    std::is_same_v<remove_cvref_t<A>, remove_cvref_t<B>> &&
    is_nary_operator_v<A, D> && is_nary_operator_v<B, D>;
template<typename A, typename B, typename D>
using enable_if_matching_nary_operators = std::enable_if<matching_nary_operators_v<A, B, D>, nary_operator<D>>;
template<typename A, typename B, typename D>
using enable_if_matching_nary_operators_t = typename enable_if_matching_nary_operators<A, B, D>::type;

template<typename A, typename B, typename D>
inline constexpr bool different_nary_operators_v = 
    !std::is_same_v<remove_cvref_t<A>, remove_cvref_t<B>> &&
    is_nary_operator_v<A, D> && is_nary_operator_v<B, D>;
template<typename A, typename B, typename D>
using enable_if_different_nary_operators = std::enable_if<different_nary_operators_v<A, B, D>, nary_operator<D>>;
template<typename A, typename B, typename D>
using enable_if_different_nary_operators_t = typename enable_if_different_nary_operators<A, B, D>::type;

template<typename A, typename B, typename D>
inline constexpr bool different_operators_v = 
    !std::is_same_v<remove_cvref_t<A>, remove_cvref_t<B>> &&
    is_operator_v<A, D> && is_operator_v<B, D>;
template<typename A, typename B, typename D>
using enable_if_different_operators = std::enable_if<different_operators_v<A, B, D>, A>;
template<typename A, typename B, typename D>
using enable_if_different_operators_t = typename enable_if_different_operators<A, B, D>::type;

} // namespace detail

template<typename T, typename D>
unite<D> operator|(
    detail::enable_if_nary_operator_t<T, D>&& no, 
    D&& d)
{
    auto forwarded = std::forward<T>(no);
    forwarded.push_back(std::forward<D>(d));
    return forwarded;
}

template<typename T, typename D>
unite<D> operator|(
    D&& d,
    detail::enable_if_nary_operator_t<T, D>&& no)
{
    auto forwarded = std::forward<T>(no);
    forwarded.insert(forwarded.begin(), std::forward<D>(d));
    return forwarded;
}

template<typename T, typename D>
unite<D> operator|(
    detail::enable_if_unary_operator_t<T, D>&& uo,
    D&& d)
{
    return unite<D>{
        std::forward<T>(uo), 
        std::forward<D>(d)
    };
}

template<typename T, typename D>
unite<D> operator|(
    D&& d,
    detail::enable_if_unary_operator_t<T, D>&& uo)
{
    return unite<D>{
        std::forward<D>(d),
        std::forward<T>(uo)
    };
}

template<typename A, typename B, typename D>
unite<D> operator|(
    detail::enable_if_different_operators_t<A, B, D>&& lo,
    detail::enable_if_different_operators_t<A, B, D>&& ro)
{
    return unite<D>{
        std::forward<A>(lo),
        std::forward<B>(ro)
    };
}

template<typename T, typename D>
unite<D> operator|(
    detail::enable_if_intersect_t<T, D>&& li,
    detail::enable_if_intersect_t<T, D>&& ri)
{
    return unite<D>{
        std::forward<T>(li),
        std::forward<T>(ri)
    };
}

template<typename T, typename D>
unite<D> operator|(
    detail::enable_if_unite_t<T, D>&& li,
    detail::enable_if_unite_t<T, D>&& ri)
{
    auto forwarded = std::forward<T>(li);
    forwarded.insert(
        forwarded.end(),
        ri.begin(), ri.end());
}

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