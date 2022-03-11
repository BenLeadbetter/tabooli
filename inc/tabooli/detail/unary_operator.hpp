#pragma once

#include <memory>

namespace tabooli::detail {
    
template<typename D>
class unary_operator
{
public:
    unary_operator(const D& d) : m_d(std::make_unique<D>(d)) {}
    unary_operator(D&& d) : m_d(std::make_unique<D>(std::move(d))) {}

    unary_operator(const unary_operator& other) { m_d = std::make_unique<D>(*other.m_d); }
    unary_operator(unary_operator&&) = default;

    unary_operator& operator=(const D& d) { m_d = std::make_unique<D>(d); }
    unary_operator& operator=(D&& d) { m_d = std::make_unique<D>(std::move(d)); }

    unary_operator& operator=(const unary_operator& other) { m_d = std::make_unique<D>(*other.m_d); }
    unary_operator& operator=(unary_operator&&) = default;
    
    bool operator==(const unary_operator& other) const { return m_d == other.m_d; }

    const D& data() const { return *m_d; }

private:
    std::unique_ptr<D> m_d;
};

}