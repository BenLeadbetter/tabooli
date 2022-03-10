#include <tabooli/combinator.hpp>
#include <tabooli/detail/overload.hpp>

#include <iterator>
#include <ostream>
#include <string_view>

namespace tabooli {

namespace detail {
    
template<typename> struct custom_prefix : std::false_type {};

constexpr std::string_view default_intersect_prefix() { return "intersect"; }
constexpr std::string_view default_unite_prefix() { return "unite"; }
constexpr std::string_view default_negate_prefix() { return "negate"; }
constexpr std::string_view default_seperator() { return ","; }
    
template<typename D>
constexpr std::string_view intersect_prefix()
{
    if constexpr (custom_prefix<combinator<D>>::value)
    {
        return custom_prefix<combinator<D>>::intersect_prefix;
    }
    else
    {
        return default_intersect_prefix();
    }
}

template<typename D>
constexpr std::string_view unite_prefix()
{
    if constexpr (custom_prefix<combinator<D>>::value)
    {
        return custom_prefix<combinator<D>>::unite_prefix;
    }
    else
    {
        return default_unite_prefix();
    }
}

template<typename D>
constexpr std::string_view negate_prefix()
{
    if constexpr (custom_prefix<combinator<D>>::value)
    {
        return custom_prefix<combinator<D>>::negate_prefix;
    }
    else
    {
        return default_negate_prefix();
    }
}

template<typename D>
constexpr std::string_view seperator()
{
    if constexpr (custom_prefix<combinator<D>>::value)
    {
        return custom_prefix<combinator<D>>::seperator;
    }
    else
    {
        return default_seperator();
    }
}

template<typename O>
void write_contents(std::ostream& os, const O& o, const std::string_view& sep)
{
    os << "{";
    if (!o.empty())
    {
        auto itr = o.begin();
        for (; itr != std::prev(o.cend()); std::advance(itr, 1))
        {
            os << *itr << sep;
        }
        os << *itr;
    }
    os << "}";
}

} // namespace detail

template<typename D>
std::ostream& operator<<(std::ostream& os, const combinator<D>& l)
{
    const auto visitor = detail::overload(
        [&os](const D& d) { os << d; },
        [&os](const intersect<D>& a) { os << a; },
        [&os](const unite<D>& o) { os << o; },
        [&os](const negate<D>& o) { os << o; },
        [](auto){}
    );
    std::visit(visitor, l);
    return os;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const intersect<D>& a)
{
    os << detail::intersect_prefix<D>();
    detail::write_contents(os, a, detail::seperator<D>());
    return os;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const unite<D>& o)
{
    os << detail::unite_prefix<D>();
    detail::write_contents(os, o, detail::seperator<D>());
    return os;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const negate<D>& n)
{
    os << detail::negate_prefix<D>() << "{" << *n.data << "}";
    return os;
}

}

#define TABOOLI_CUSTOM_PREFIX(combinator_type,iprefix,uprefix,nprefix,sep) \
template<> struct tabooli::detail::custom_prefix<combinator_type> \
: std::true_type \
{ \
    static constexpr std::string_view intersect_prefix = iprefix; \
    static constexpr std::string_view unite_prefix = uprefix; \
    static constexpr std::string_view negate_prefix = nprefix; \
    static constexpr std::string_view seperator = sep; \
}