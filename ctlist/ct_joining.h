#ifndef __CTLIST_CT_JOINING_H__
#define __CTLIST_CT_JOINING_H__
#include "ct_list.h"

namespace ctlist
{
    template<typename T, typename U>
    struct ctpair { T first; U second; };
    /**
     * Alias to the list that is the result of concatenating L1 and L2
     * <p>
     * L1 and L2 must be of types list<T, As...>, and list<T, Bs...>. Both lists must be of the same type. The result list will have a element type of T and value of As..., Bs...
     * @tparam L1 the first list
     * @tparam L2 the second list
     */
    template<typename L1, typename L2>
    using list_concat = decltype([]<typename E, E... As, E... Bs>(comptime_list<E, As...>, comptime_list<E, Bs...>) ->
        comptime_list<E, As..., Bs...> {}(std::declval<L1>(), std::declval<L2>()));
    
    /**
     * Alias to the list that is the result of a zip on L1 and L2
     * <p>
     * L1 and L2 shall have the same size. L1 and L2 must be of type list<T, As...> and list<U, Bs...>, respectivly. The result list will be of type pair<T, U>, with each element Ei = {Ai, Bi}.
     * @tparam L1 the first list
     * @tparam L2 the second list
     */
    template<typename L1, typename L2> requires (L1::size == L2::size)
    using list_zip = decltype([]<typename T1, T1... As, typename T2, T2... Bs>(comptime_list<T1, As...>, comptime_list<T2, Bs...>) ->
        comptime_list<ctpair<T1, T2>, ctpair<T1, T2>(As, Bs)...> {}(std::declval<L1>(), std::declval<L2>()));

    /**
     * Alias to the list that is the result prepending val and L
     * <p>
     * L must be of type list<T, As...> and val must be of type T. This is an alias to the list that is list<T, val, As...>
     * @tparam L the list
     * @tparam val the value
     */
    template<typename L, typename L::value_type val>
    using list_prepend = decltype([]<typename L::value_type... Ts>(comptime_list<typename L::value_type, Ts...>) ->
        comptime_list<typename L::value_type, val, Ts...> {}(std::declval<L>()));

    /**
     * Alias to the list that is the result prepending val and L
     * <p>
     * L must be of type list<T, As...> and val must be of type T. This is an alias to the list that is list<T, As..., vals>
     * @tparam L the list
     * @tparam val the value
     */
    template<typename L, typename L::value_type val>
    using list_append = decltype([]<typename L::value_type... Ts>(comptime_list<typename L::value_type, Ts...>) ->
        comptime_list<typename L::value_type, Ts..., val> {}(std::declval<L>()));
}


#endif

