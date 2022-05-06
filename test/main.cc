#define D_AKR_TEST
#include "akr_test.hh"

#include "..\enumerator.hh"

#include <cstdio>
#include <vector>

int main()
{
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
}
