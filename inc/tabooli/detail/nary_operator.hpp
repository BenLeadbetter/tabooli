#pragma once

#include <initializer_list>
#include <type_traits>

namespace tabooli::detail {

template<typename D, template<typename> typename C>
class nary_operator 
{
public:
    using const_iterator = typename C<D>::const_iterator;
    using value_type = typename C<D>::value_type;
    using size_type = typename C<D>::size_type;
    using distance_type = typename C<D>::difference_type;
    using const_reference = typename C<D>::const_reference;
    using const_pointer = typename C<D>::const_pointer;

    nary_operator() = default;
    nary_operator(const nary_operator&) = default;
    nary_operator(nary_operator&&) = default;
    nary_operator& operator=(const nary_operator&) = default;
    nary_operator& operator=(nary_operator&&) = default;
    nary_operator(std::initializer_list<D> l) : m_data(l.begin(), l.end()) {}
    
    bool operator==(const nary_operator& other) const { return m_data == other.m_data; }
    
    const_iterator begin() const { return m_data.begin(); }
    const_iterator end() const { return m_data.end(); }
    const_iterator cbegin() const { return m_data.cbegin(); }
    const_iterator cend() const { return m_data.cend(); }
    
    bool empty() const { return m_data.empty(); }

    template<typename T>
    void append(T&& val)
    {
        if constexpr (std::is_base_of_v<nary_operator<D, C>, T>)
        {
            m_data.insert(m_data.end(), val.m_data.begin(), val.m_data.end());
        }
        else
        {
            m_data.push_back(std::forward<T>(val));
        }
    }

    template<typename T>
    void prepend(T&& val)
    {
        if constexpr (std::is_base_of_v<nary_operator<D, C>, T>)
        {
            m_data.insert(m_data.begin(), val.m_data.begin(), val.m_data.end());
        }
        else
        {
            m_data.insert(m_data.begin(), val);
        }
    }
    
private:
    C<D> m_data;
};

}