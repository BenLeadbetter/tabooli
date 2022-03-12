#pragma once

#include <tabooli/detail/operator_code.hpp>

#include <optional>
#include <ostream>
#include <memory>

namespace tabooli::detail {

template<typename D>
class expression_node
{
public:
    virtual ~expression_node() = default;
    expression_node() = default;
    expression_node(expression_node&&) = default;
    expression_node& operator=(expression_node&&) = default;
    expression_node(const expression_node& other)
    {
        *this = other;
    }
    expression_node& operator=(const expression_node& other)
    {
        if (other.m_data)
        {
            m_data = std::make_unique<D>(*other.m_data);
        }
        m_operator_code = other.m_operator_code;
        return *this;
    }
    expression_node(const D& d) : m_data(std::make_unique<D>(d)) {}
    expression_node(D&& d) : m_data(std::make_unique<D>(std::move(d))) {}
    expression_node(operator_code op) : m_operator_code(op) {}

    const D* data() const noexcept { return m_data.get(); }
    D* data() noexcept { return m_data.get(); }
    std::optional<operator_code> op() const noexcept { return m_operator_code; }
    
    friend bool operator==(const expression_node& lhs, const expression_node& rhs)
    {
        if (lhs.m_operator_code != rhs.m_operator_code) return false;
        if (!lhs.m_data && !rhs.m_data) return true;
        if (!lhs.m_data || !rhs.m_data) return false;
        return *lhs.m_data == *rhs.m_data;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const expression_node& n)
    {
        if (n.m_data) out << *n.m_data;
        if (n.m_operator_code) out << static_cast<int>(*n.m_operator_code);
        return out;
    }

private:
    std::unique_ptr<D> m_data;
    std::optional<operator_code> m_operator_code;
};

}