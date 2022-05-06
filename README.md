# **akr::ForwardEnumerator & akr::ReverseEnumerator**

## **Contents**

  - [1. Require](#1-require)
  - [2. Usage](#2-usage)

## **1. Require**
* ### `C++20`

## **2. Usage**
```c++
#include "enumerator.hh"

auto vec = std::vector({ 1, 2, 3, 4, 5 });

for (auto&& e : akr::ForwardEnumerator(vec))
{
    printf("%d ", e);
}
puts("");

for (auto&& e : akr::ForwardEnumerator(vec.begin(), vec.begin() + 3))
{
    printf("%d ", e);
}
puts("");

for (auto&& e : akr::ReverseEnumerator(vec))
{
    printf("%d ", e);
}
puts("");

for (auto&& e : akr::ReverseEnumerator(vec.begin(), vec.begin() + 3))
{
    printf("%d ", e);
}
puts("");
```
