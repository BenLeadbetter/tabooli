#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <memory>
#include <type_traits>

namespace tabooli::detail {

template<typename D> class binary_tree;
template<typename D> class binary_tree_const_iterator;

template<typename D>
class binary_tree_node
{
public:
    binary_tree_node() = default;
    binary_tree_node(binary_tree_node&& other) :
        m_data(std::move(other.m_data)),
        m_children({
            std::move(other.m_children[0]), 
            std::move(other.m_children[1])
        }),
        m_parent(other.m_parent)
    {
        if (m_children[0]) m_children[0]->m_parent = this;
        if (m_children[1]) m_children[1]->m_parent = this;
        
        other.m_parent = nullptr;
    }
    binary_tree_node& operator=(binary_tree_node&& other)
    {
        m_data = std::move(other.m_data);
        m_children[0] = std::move(other.m_children[0]);
        m_children[1] = std::move(other.m_children[1]);
        m_parent = other.m_parent;
        
        if (m_children[0]) m_children[0]->m_parent = this;
        if (m_children[1]) m_children[1]->m_parent = this;
        
        other.m_parent = nullptr;
        return *this;
    }
    binary_tree_node& operator=(const binary_tree_node& other)
    {
        if (other.m_data)
        {
            m_data = std::make_unique<D>(*other.m_data);
        }
        if (other.m_children[0])
        {
            binary_tree_node copy = *other.m_children[0];
            m_children[0] = std::make_unique<binary_tree_node<D>>(std::move(copy));
            m_children[0]->m_parent = this;
        }
        if (other.m_children[1])
        {
            binary_tree_node copy = *other.m_children[1];
            m_children[1] = std::make_unique<binary_tree_node<D>>(std::move(copy));
            m_children[1]->m_parent = this;
        }
        return *this;
    }
    binary_tree_node(const binary_tree_node& other)
    {
        *this = other;
    }
    binary_tree_node(D d) : m_data(std::make_unique<D>(std::move(d))) {}
    binary_tree_node(
        D d,
        std::unique_ptr<binary_tree_node> lc,
        std::unique_ptr<binary_tree_node> rc) :
            m_data(std::make_unique<D>(std::move(d))),
            m_children({
                std::move(lc), std::move(rc)
            })
    {
        m_children[0]->m_parent = this;
        m_children[1]->m_parent = this;
    }
    binary_tree_node(
        D d,
        std::unique_ptr<binary_tree_node> c) :
            m_data(std::make_unique<D>(std::move(d))),
            m_children({std::move(c), nullptr})
    {
        m_children[0]->m_parent = this;
    }
    const D* data() const noexcept { return m_data.get(); }
    binary_tree_node* first_child() const noexcept { return m_children[0].get(); }
    binary_tree_node* second_child() const noexcept { return m_children[1].get(); }
    binary_tree_node* parent() const noexcept { return m_parent; }

    void set_parent(binary_tree_node* p) noexcept { m_parent = p; }
    void set_child(std::unique_ptr<binary_tree_node> n)
    {
        m_children[0] = std::move(n);
        m_children[1].reset();

        m_children[0]->m_parent = this;
    }
    void set_children(
        std::unique_ptr<binary_tree_node> f, 
        std::unique_ptr<binary_tree_node> s)
    {
        m_children[0] = std::move(f);
        m_children[1] = std::move(s);
            
        m_children[0]->m_parent = this;
        m_children[1]->m_parent = this;
    }
    
    bool operator==(const binary_tree_node& other) const
    {
        return m_data == other.m_data;
    }

private:
    std::unique_ptr<D> m_data;
    std::array<std::unique_ptr<binary_tree_node>, 2> m_children;
    binary_tree_node* m_parent{nullptr};
};

template<typename D>
class binary_tree_const_iterator
{
public:
    using value_type = const D;
    using pointer = const D*;
    using reference = const D&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    binary_tree_const_iterator() = default;

    bool operator==(binary_tree_const_iterator other) const noexcept 
    { 
        return p == other.p;
    }
    bool operator!=(binary_tree_const_iterator other) const noexcept { return !(*this == other); }
    reference operator*() const { return *p->data(); }
    pointer operator->() const { return p->data(); }
    binary_tree_const_iterator& operator++() 
    {
        if (p->first_child())
        {
            // has first child
            p = p->first_child();
            return *this;
        }

        if (!p->parent())
        {
            // is root node
            p = nullptr;
            return *this;
        }
        
        if (p == p->parent()->first_child())
        {
            // is first child
            while (p->parent() && !p->parent()->second_child()) 
            {
                p = p->parent();
            }
            if (!p->parent())
            {
                p = nullptr;
                return *this;
            }
            p = p->parent()->second_child();
            return *this;
        }

        // is second child
        while (
            p->parent() && p->parent()->parent()
            && p->parent() == p->parent()->parent()->second_child()) 
        {
            p = p->parent();
        }
        p = p->parent();
        while (p->parent() && !p->parent()->second_child()) 
        {
            p = p->parent();
        }
        if (!p->parent())
        {
            p = nullptr;
            return *this;
        }
        p = p->parent()->second_child();
        return *this;
    }
    binary_tree_const_iterator operator++(int)
    {
        auto ret = *this;
        ++(*this);
        return ret;
    }
    binary_tree_const_iterator parent() const 
    {
        return binary_tree_const_iterator(p->parent());
    }

private:
    friend class binary_tree<D>;
    binary_tree_const_iterator(const binary_tree_node<D>* node) : p(node) {}
    const binary_tree_node<D>* p{nullptr};
};

template<typename D>
class binary_tree
{
    using node = binary_tree_node<D>;

public:
    using const_iterator = binary_tree_const_iterator<D>;
    using iterator = binary_tree_const_iterator<D>;
    using value_type = node;
    using const_reference = const node&;
    using const_pointer = const node*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    binary_tree() = default;
    binary_tree(binary_tree&&) = default;
    binary_tree& operator=(binary_tree&&) = default;
    binary_tree(const D& data) : m_root(data) {}
    binary_tree(D&& data) : m_root(std::move(data)) {}
    binary_tree& operator=(const binary_tree& other)
    {
        m_root = other.m_root;
        assert(root_valid());
        return *this;
    }
    binary_tree(const binary_tree& other) :
        m_root(other.m_root)
    {
        assert(root_valid());
    }

    const_iterator begin() const
    {
        if (!m_root.data()) return end();
        return const_iterator(&m_root);
    }

    const_iterator end() const
    {
        return const_iterator(nullptr); 
    }
    
    size_type size() const { return std::distance(begin(), end()); }
    size_type max_size() const { return std::numeric_limits<difference_type>::max(); }
    bool empty() const { return begin() == end(); }
    
    friend bool operator==(const binary_tree& lhs, const binary_tree& rhs)
    {
        return std::mismatch(
            lhs.begin(), lhs.end(), 
            rhs.begin(), rhs.end()
        ) == std::make_pair(lhs.end(), rhs.end());
    }
    friend bool operator!=(const binary_tree& lhs, const binary_tree& rhs)
    {
        return !(lhs == rhs);
    }
    
    void compose(const D& new_root)
    {
        if (m_root.data())
        {
            m_root = node(
                new_root, std::make_unique<node>(std::move(m_root))
            );
        }
        else 
        {
            m_root = node(new_root);
        }
        assert(root_valid());
    }

    void compose(D&& new_root)
    {
        if (m_root.data())
        {
            m_root = node(
                std::move(new_root),
                std::make_unique<node>(std::move(m_root))
            );
        }
        else 
        {
            m_root = node(std::move(new_root));
        }
        assert(root_valid());
    }

    void compose(const D& new_root, const binary_tree& other)
    {
        m_root = node(
            new_root,
            std::make_unique<node>(std::move(m_root)),
            std::make_unique<node>(other.m_root)
        );
        assert(root_valid());
    }

    void compose(D&& new_root, const binary_tree& other)
    {
        m_root = node(
            std::move(new_root),
            std::make_unique<node>(std::move(m_root)),
            std::make_unique<node>(other.m_root)
        );
        assert(root_valid());
    }

    void compose(const D& new_root, binary_tree&& other)
    {
        assert(m_root);
        m_root = node(
            new_root,
            std::make_unique<node>(std::move(m_root)),
            std::make_unique<node>(std::move(other.m_root))
        );
        assert(root_valid());
    }

    void compose(D&& new_root, binary_tree&& other)
    {
        m_root = node(
            std::move(new_root),
            std::make_unique<node>(std::move(m_root)),
            std::make_unique<node>(std::move(other.m_root))
        );
        assert(root_valid());
    }
    
    void set_child(const binary_tree& child)
    {
        m_root.set_child(std::make_unique<node>(child.m_root));
        assert(root_valid());
    }

    void set_child(binary_tree&& child)
    {
        m_root.set_child(std::make_unique<node>(std::move(child.m_root)));
        assert(root_valid());
    }

    void set_children(
        const binary_tree& first,
        const binary_tree& second)
    {
        m_root.set_children(
            std::make_unique<node>(*first.m_root), 
            std::make_unique<node>(*second.m_root)
        );
        assert(root_valid());
    }

    void set_children(
        binary_tree&& first,
        const binary_tree& second)
    {
        m_root.set_children(
            std::move(first.m_root), 
            std::make_unique<node>(second.m_root));
        assert(root_valid());
    }

    void set_children(
        const binary_tree& first,
        binary_tree&& second)
    {
        m_root.set_children(
            std::make_unique<node>(first.m_root), 
            std::move(second.m_root)
        );
        assert(root_valid());
    }

    void set_children(
        binary_tree&& first,
        binary_tree&& second)
    {
        m_root.set_children(
            std::make_unique<node>(std::move(first.m_root)),
            std::make_unique<node>(std::move(second.m_root))
        );
        assert(root_valid());
    }
    
private:
    bool root_valid() const
    {
        bool ret = true;
        if (auto* c = m_root.first_child())
        {
            ret &= c->parent() == &m_root;
        }
        if (auto* c = m_root.first_child())
        {
            ret &= c->parent() == &m_root;
        }
        return ret;
    }
    node m_root;
};

}