#include <iostream>
using namespace std;

namespace tme3
{
    template <typename Iterator> // Declare Iterator as a template parameter
    size_t count(Iterator begin, Iterator end)
    {
        size_t cpt = 0;
        while (begin != end)
        {
            ++cpt;
            ++begin;
        }
        return cpt;
    };

    template <typename Iterator, typename T>
    size_t count_if_equal(Iterator begin, Iterator end, const T &val)
    {
        size_t cpt = 0;
        while (begin != end)
        {
            if (*begin == val)
            {
                cpt++;
            }
            ++begin;
        }
        return cpt;
    }
}
