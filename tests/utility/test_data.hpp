#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <ostream>
#include <set>

namespace tests {

struct test_data
{
    int val;
    test_data();
    test_data(int);
    test_data(const test_data&);
    test_data(test_data&&);
    test_data& operator=(const test_data&);
    test_data& operator=(test_data&&);
};

std::ostream& operator<<(std::ostream&, const test_data&);
bool operator==(const test_data&, const test_data&);

class test_data_ctor_broadcaster;
class test_data_ctor_listener 
{
public:
    ~test_data_ctor_listener();
    int default_ctor_count() const;
    int data_ctor_count() const;
    int copy_ctor_count() const;
    int move_ctor_count() const;
    int copy_assignment_count() const;
    int move_assignment_count() const;
    void reset() noexcept;

private:
    friend class test_data_ctor_broadcaster;
    std::function<void()> m_deregister_callback;
    int m_default_ctor_count{0};
    int m_data_ctor_count{0};
    int m_copy_ctor_count{0};
    int m_move_ctor_count{0};
    int m_copy_assignment_count{0};
    int m_move_assignment_count{0};
};

class test_data_ctor_broadcaster
{
public:
    static void register_listener(test_data_ctor_listener* listener);

private:
    test_data_ctor_broadcaster() = default;

    friend struct test_data;
    friend std::unique_ptr<test_data_ctor_broadcaster> std::make_unique<test_data_ctor_broadcaster>();
    
    void default_ctor();
    void data_ctor();
    void copy_ctor();
    void move_ctor();
    void copy_assignment();
    void move_assignment();

    template<typename F>
    void with_each_listener(const F& function)
    {
        std::for_each(m_listeners.begin(), m_listeners.end(), function);
    }

    std::set<test_data_ctor_listener*> m_listeners;
    static std::unique_ptr<test_data_ctor_broadcaster> s_instance;
};

}