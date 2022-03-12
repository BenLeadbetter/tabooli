#include <utility/test_data.hpp>

namespace tests {
 
test_data_ctor_listener::~test_data_ctor_listener()
{
    if (m_deregister_callback) m_deregister_callback();
}
int test_data_ctor_listener::default_ctor_count() const 
{ 
    return m_default_ctor_count; 
}

int test_data_ctor_listener::data_ctor_count() const 
{ 
    return m_data_ctor_count; 
}

int test_data_ctor_listener::copy_ctor_count() const 
{ 
    return m_copy_ctor_count; 
}

int test_data_ctor_listener::move_ctor_count() const 
{ 
    return m_move_ctor_count; 
}

int test_data_ctor_listener::copy_assignment_count() const 
{ 
    return m_copy_assignment_count; 
}

int test_data_ctor_listener::move_assignment_count() const 
{ 
    return m_move_assignment_count; 
}

void test_data_ctor_listener::reset() noexcept
{
    m_default_ctor_count = 0;
    m_data_ctor_count = 0;
    m_copy_ctor_count = 0;
    m_move_ctor_count = 0;
    m_copy_assignment_count = 0;
    m_move_assignment_count = 0;
}

std::unique_ptr<test_data_ctor_broadcaster> 
    test_data_ctor_broadcaster::s_instance 
        = std::make_unique<test_data_ctor_broadcaster>();

void test_data_ctor_broadcaster::register_listener(test_data_ctor_listener* listener)
{
    s_instance->m_listeners.insert(listener);        
    listener->m_deregister_callback = [listener]() { 
        s_instance->m_listeners.erase(s_instance->m_listeners.find(listener));
    };
}
void test_data_ctor_broadcaster::default_ctor()
{
    with_each_listener([](auto* listener){++listener->m_default_ctor_count;});
}
void test_data_ctor_broadcaster::data_ctor()
{
    with_each_listener([](auto* listener){++listener->m_data_ctor_count;});
}
void test_data_ctor_broadcaster::copy_ctor()
{
    with_each_listener([](auto* listener){++listener->m_copy_ctor_count;});
}
void test_data_ctor_broadcaster::move_ctor()
{
    with_each_listener([](auto* listener){++listener->m_move_ctor_count;});
}
void test_data_ctor_broadcaster::copy_assignment()
{
    with_each_listener([](auto* listener){++listener->m_copy_assignment_count;});
}
void test_data_ctor_broadcaster::move_assignment()
{
    with_each_listener([](auto* listener){++listener->m_move_assignment_count;});
}

test_data::test_data() : val(0) 
{
    test_data_ctor_broadcaster::s_instance->default_ctor();
}

test_data::test_data(int i) : val(i) 
{
    test_data_ctor_broadcaster::s_instance->data_ctor();
}

test_data::test_data(const test_data& d) : val(d.val)
{
    test_data_ctor_broadcaster::s_instance->copy_ctor();
}

test_data::test_data(test_data&& d) : val(std::move(d.val))
{
    test_data_ctor_broadcaster::s_instance->move_ctor();
}

test_data& test_data::operator=(const test_data& d)
{
    val = d.val;
    test_data_ctor_broadcaster::s_instance->copy_assignment();
    return *this;
}

test_data& test_data::operator=(test_data&& d)
{
    val = std::move(d.val);
    test_data_ctor_broadcaster::s_instance->move_assignment();
    return *this;
}

std::ostream& operator<<(std::ostream& out, const test_data& d)
{
    return out << d.val;
}

bool operator==(const test_data& lhs, const test_data& rhs) 
{
    return lhs.val == rhs.val;
}

}
