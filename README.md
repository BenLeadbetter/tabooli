# Tabooli

Tabooli is a header-only C++ library 
providing data structures and algorithms
for declarative, lazy, expression abstractions.

## Motivation

Suppose you would like to compose some data into an
expression.

```cpp
auto bugs = Toon{};
auto daffy = Toon{};
auto sylvester = Toon{};

bool value = bugs && (daffy || !sylvester);
```

This value would then be fixed.
If the values of the data were to change then 
the expression would need to be re-evaluated.
Additionally, if we wanted to store the structure 
of the expression, perhaps to be interpreted in a 
different context, then we are stuck since
all the structure is lost as soon as the expression
is evaluated.

Tabooli is designed to solve these problems.

```cpp
auto bugs = tabooli::boolean<Toon>();
auto daffy = tabooli::boolean<Toon>();
auto sylvester = tabooli::boolean<Toon>();

auto expression = bugs && (daffy || !sylvester);

// store the expression or pass it around
// or do stuff to change the data values

bool value = expression.evaluate();
```

## Features

### Boolean

Tabooli's `boolean` type allows users to wrap arbitrary data
into an expression formed of logical operators.

```cpp
struct Widget {};

auto w1 = tabooli::boolean<Widget>{};
auto w2 = tabooli::boolean<Widget>{};
auto w3 = tabooli::boolean<Widget>{};

auto expression = w1 || !(w2 && w3);
```

Expressions can also be created inline, from r-value data.
We use the pipe operator to help us,
because providing logical operators for arbitrary type
would break C++, 

```cpp
using mybool = ;
auto expression = tabooli::boolean<Widget>{} | 
    Widget{} || !(
        tabooli::boolean<Widget>{} | Widget{} && Widget{}i
    );
```

If `Widget` is implicitly convertible to bool,
then we can evaluate using the no-args `evaluate` method.

```cpp
bool value = expression.evaluate();
```

Otherwise we can pass
a functor describing how to convert the type to bool.

```cpp
bool value = expression.evaluate([](const Widget&){ return false; });
```

### Expression

The `boolean` type is derived from the `tabooli::detail::expression`.
This is a standard compliant container with methods to 
compose with other trees and also a method called `visit`
which traverses the tree while calling in on the visitor object.

```cpp
struct visitor
{
    void with_data(const Widget&) {...}
    void push_operator(operator_code) {...}
    void pop_operator(operator_code) {...}
};
expression.visit(visitor{});
```

### Binary Tree

Under the hood, the `expression` type is represented as a 
[binary expression tree](https://en.wikipedia.org/wiki/Binary_expression_tree)
by the `tabooli::detail::binary_tree`.
Traversing this tree is `O(n)` complexity where `n` is the
number of nodes in the tree. 
The binary tree type is a standard compliant container.

## installation

The easiest way to install the library is to simply
add it as a CMake subdirectory of your project and 
link your targets to the library.

```cmake
add_subdirectory(tabooli)
add_executable(my_app ...)
target_link_libraries(my_app PRIVATE tabooli)
```

## Tests

To build the unit tests set `TABOOLI_TESTS=ON`. 
The tests require the 
[Catch2](https://github.com/catchorg/Catch2) and 
[Boost](https://www.boost.org/doc/libs/1_78_0/more/getting_started/)
libraries to be installed.

## The Name

Tabooli as in to-bool-i. It sounds cute and I like the dish :-)

## Contribution

Contribution and feedback is more than welcome!