#ifndef __CTLIST_CT_HIGHER_ORDER_H__
#define __CTLIST_CT_HIGHER_ORDER_H__
#include "ct_list.h"
#include "ct_joining.h"

namespace ctlist
{
    namespace detail
    {    
        template<typename U, typename T, template<U, T> typename, U v> struct folder { constexpr static U value = v; };

        template<typename L, template<typename L::value_type> typename Pred>
        struct list_filter_impl;
    
        template<typename T, T current, T... Ts, template<typename comptime_list<T, current, Ts...>::value_type> typename Pred>
        struct list_filter_impl<comptime_list<T, current, Ts...>, Pred>
        {
            static constexpr auto helper()
            {
                if constexpr(sizeof...(Ts) == 0)
                {
                    if constexpr(Pred<current>::value)
                        return comptime_list<T>{};
                    else
                        return comptime_list<T, current>{};
                }
                else
                {
                    if constexpr(Pred<current>::value)
                        return typename list_filter_impl<comptime_list<T, Ts...>, Pred>::type{};
                    else
                        return list_prepend<typename list_filter_impl<comptime_list<T, Ts...>, Pred>::type, current>{};
                }
            }
            using type = decltype(helper());
        };
    }

    /**
     * Performs a map operation on the list, given a mapper template
     * <p>
     * The Mapper must return the same type for all values of L, and it must have a constexpr static value named Mapper<V>::value, where V is any element of the list
     * @tparam L the list
     * @tparam Mapper the template that maps all E -> U
     */
    template<typename L, template<typename L::value_type> typename Mapper>
    using list_map = decltype([]<typename T, T... Ts>(comptime_list<T, Ts...>) -> 
        comptime_list<T, Mapper<Ts>::value...> {}(std::declval<L>()));

    /**
     * Performs a filter operation on the list, given a predicate
     * <p>
     * Produces a new list, containing values in L such that Pred<E>::value is true
     * @tparam L the list
     * @tparam Pred the template predicate E -> bool
     */
    template<typename L, template<typename L::value_type> typename Pred>
    using list_filter = typename detail::list_filter_impl<L, Pred>::type;

    // Utility operators in order to abuse fold operation on template paramter packs
    /*template<typename U, typename T, template<U, T> typename Folder, U u, T t>
    constexpr detail::folder<U, T, Folder, Folder<u, t>::value> operator+(detail::folder<U, T, Folder, u>, detail::value_wrapper<T, t>) noexcept;
    
    template<typename T, template<T, T> typename Folder, T V, T W, auto R = Folder<V, W>::value>
    constexpr detail::folder<decltype(R), Folder, R> operator+(detail::value_wrapper<T, W>, detail::folder<T, Folder, V>) noexcept;

    template<typename List, template<typename List::value_type, typename List::value_type> typename Folder>
    constexpr auto list_foldl = decltype([]<typename T, T First, T ...Ts>(comptime_list<T, First, Ts...>) ->
            decltype((detail::folder<T, Folder, First>{} + ... + detail::value_wrapper<T, Ts>{})) {}(std::declval<List>()))::value;
    
    template<typename List, template<typename List::value_type, typename List::value_type> typename Folder>
    constexpr auto list_foldr = decltype([]<typename T, T First, T ...Ts>(comptime_list<T, First, Ts...>) ->
            decltype((detail::value_wrapper<T, Ts>{} + ... + detail::folder<T, Folder, First>{})) {}(std::declval<List>()))::value;*/

}

#endif
