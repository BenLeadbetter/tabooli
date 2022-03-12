#pragma once

#define TABOOLI_BINARY_OPERATOR(CLASS,OP,CODE)\
friend CLASS operator OP(CLASS&& l, CLASS&& r)\
{\
    return std::move(l.operate(\
        detail::operator_code::CODE,\
        std::move(r)));\
}\
friend CLASS operator OP(CLASS&& l, const CLASS& r)\
{\
    return std::move(l.operate(\
        detail::operator_code::CODE,\
        r));\
}\
friend CLASS operator OP(const CLASS& l, CLASS&& r)\
{\
    return l.copy_operate(\
        detail::operator_code::CODE,\
        std::move(r));\
}\
friend CLASS operator OP(const CLASS& l, const CLASS& r)\
{\
    return l.copy_operate(\
        detail::operator_code::CODE,\
        r);\
}
    
#define TABOOLI_UNARY_OPERATOR(CLASS,OP,CODE) \
friend CLASS operator OP(CLASS&& c)  \
{ \
    return std::move(c.operate(detail::operator_code::CODE)); \
} \
friend CLASS operator OP(const CLASS& c) \
{ \
    return c.copy_operate(detail::operator_code::CODE); \
}