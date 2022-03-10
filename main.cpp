#include <iostream>
#include "ctlist/ct_algo.h"
#include "ctlist/ct_restrict.h"

int main()
{
    using namespace ctlist;
    //::test();
    //using t = decltype(detail::front_skipper<2, int>{} + detail::value_wrapper<int, 69>{});
    std::cout << ctlist::list_drop_front<comptime_list<int, 69, 420, 42, 21, 92, 10>, 2>::to_string([](int a) { return std::to_string(a); });
}
