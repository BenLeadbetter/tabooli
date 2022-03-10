#pragma once

#include <vector>
#include <initializer_list>

namespace tabooli::detail {
    
template<typename D>
struct nary_operator : protected std::vector<D>
{
    using typename std::vector<D>::value_type;
    using typename std::vector<D>::size_type;
    using typename std::vector<D>::difference_type;
    using typename std::vector<D>::const_reference;
    using typename std::vector<D>::const_pointer;
    using typename std::vector<D>::const_iterator;

    using std::vector<D>::vector;
    using std::vector<D>::cbegin;
    using std::vector<D>::cend;
    using std::vector<D>::empty;
    
    const_iterator begin() const { return std::vector<D>::begin(); }
    const_iterator end() const { return std::vector<D>::end(); }
};

}