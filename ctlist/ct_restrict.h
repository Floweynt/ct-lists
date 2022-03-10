#ifndef __CTLIST_CT_RESTRICT_H__
#define __CTLIST_CT_RESTRICT_H__
#include "ct_list.h"
namespace ctlist
{
    namespace detail
    {
        template<std::size_t N, typename T, T... Ts>
        struct front_skipper { using type = comptime_list<T, Ts...>; };

        template<std::size_t N, typename T, T... Ts>
        struct front_limiter { using type = comptime_list<T, Ts...>; };

        template<std::size_t Idx, std::size_t From, std::size_t To, typename T, T... Ts>
        struct sublister { using type = comptime_list<T, Ts...>; };

        template<std::size_t Idx, std::size_t From, std::size_t To, typename T, T... Ts>
        struct eraser { using type = comptime_list<T, Ts...>; };

        template<std::size_t Idx, std::size_t From, std::size_t To, typename T, T V, T... Ts> requires (Idx >= From && Idx < To)
        constexpr sublister<Idx + 1, From, To, T, Ts..., V> operator+(sublister<Idx, From, To, T, Ts...>, value_wrapper<T, V>) noexcept;

        template<std::size_t Idx, std::size_t From, std::size_t To, typename T, T V, T... Ts> requires (!(Idx >= From && Idx < To))
        constexpr sublister<Idx + 1, From, To, T, Ts...> operator+(sublister<Idx, From, To, T, Ts...>, value_wrapper<T, V>) noexcept;

        template<std::size_t Idx, std::size_t From, std::size_t To, typename T, T V, T... Ts> requires (Idx >= From && Idx < To)
        constexpr eraser<Idx + 1, From, To, T, Ts...> operator+(eraser<Idx, From, To, T, Ts...>, value_wrapper<T, V>) noexcept;

        template<std::size_t Idx, std::size_t From, std::size_t To, typename T, T V, T... Ts> requires (!(Idx >= From && Idx < To))
        constexpr eraser<Idx + 1, From, To, T, Ts..., V> operator+(eraser<Idx, From, To, T, Ts...>, value_wrapper<T, V>) noexcept;
    };

    // Creates a sublist from [From, min(To, L::size))
    template<typename L, std::size_t From, std::size_t To>
    using list_sublist = decltype([]<typename T, T... Ts>(comptime_list<T, Ts...>) ->
        typename decltype((detail::sublister<0, From, To, T>{} + ... + detail::value_wrapper<T, Ts>{}))::type {}(std::declval<L>()));

    // Removes elements from [From, min(To, L::size))
    template<typename L, std::size_t From, std::size_t To>
    using list_erase = decltype([]<typename T, T... Ts>(comptime_list<T, Ts...>) ->
        typename decltype((detail::eraser<0, From, To, T>{} + ... + detail::value_wrapper<T, Ts>{}))::type {}(std::declval<L>()));

    template<typename L, std::size_t N>
    using list_drop_front = list_sublist<L, N, L::size>;
    template<typename L, std::size_t N>
    using list_limit_front = list_sublist<L, 0, N>;
    template<typename L, std::size_t N>
    using list_drop_back = list_sublist<L, 0, L::size - N>;
    template<typename L, std::size_t N>
    using list_limit_back = list_sublist<L, L::size - N, 0>;
}
#endif

