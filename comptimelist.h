#include <type_traits>
#include <utility>
#include <string>
namespace detail
{
    template<std::size_t N, typename T, T... Ts> requires (N < sizeof...(Ts))
    struct get_list_element;

    template<std::size_t N, typename T, T v, T... Ts>
    struct get_list_element<N, T, v, Ts...> : get_list_element<N - 1, T, Ts...> {};

    template<typename T, T v, T... Ts>
    struct get_list_element<0, T, v, Ts...> { inline static constexpr T value = v; };
}

template<typename T, T... Ts>
class comptime_list
{
    using contained = comptime_list<T, Ts...>;
public:
    using value_type = T;
    inline static constexpr std::size_t size = sizeof...(Ts) + 1;

    template<std::size_t N> requires (N < size)
    inline static constexpr T get = detail::get_list_element<N, T, Ts...>::value;

    template<typename M>
    static std::string to_string(M m)
    {
        auto ret = (std::string("{") + ... + (m(Ts) + ","));
        ret.pop_back();
        return ret + "}";
    }
};

template<typename L1, typename L2>
using list_concat = decltype([]<typename E, E... As, E... Bs>(comptime_list<E, As...>, comptime_list<E, Bs...>) ->
    comptime_list<E, As..., Bs...> {}(std::declval<L1>(), std::declval<L2>()));

template<typename L1, typename L2> requires (L1::size == L2::size)
using list_zip = decltype([]<typename T1, T1... As, typename T2, T2... Bs>(comptime_list<T1, As...>, comptime_list<T2, Bs...>) ->
    comptime_list<std::pair<T1, T2>, std::make_pair(As, Bs)...> {}(std::declval<L1>(), std::declval<L2>()));

template<typename L, typename L::value_type val>
using list_prepend = decltype([]<typename L::value_type... Ts>(comptime_list<typename L::value_type, Ts...>) ->
    comptime_list<typename L::value_type, val, Ts...> {}(std::declval<L>()));

template<typename L, typename L::value_type val>
using list_append = decltype([]<typename L::value_type... Ts>(comptime_list<typename L::value_type, Ts...>) ->
    comptime_list<typename L::value_type, Ts..., val> {}(std::declval<L>()));

template<typename L, template<typename L::value_type> typename Mapper>
using list_map = decltype([]<typename T, T... Ts>(comptime_list<T, Ts...>) -> 
    comptime_list<T, Mapper<Ts>::value...> {}(std::declval<L>()));

namespace detail
{
    template<typename T, template<T, T> typename, auto v> struct folder { constexpr static auto value = v; };
    template<typename T, T> struct value_wrapper { };

    template<typename L, template<typename L::value_type> typename Pred>
    struct list_filter_impl;

    template<typename T, T current, T... Ts, template<typename comptime_list<T, current, Ts...>::value_type> typename Pred>
    struct list_filter_impl<comptime_list<T, current, Ts...>, Pred>
    {
        using type = std::conditional_t<
            sizeof...(Ts) == 0,
            std::conditional_t<
                Pred<current>::value,
                comptime_list<T>,
                comptime_list<T, current>
            >,
            std::conditional_t<
                Pred<current>::value,
                typename list_filter_impl<comptime_list<T, Ts...>, Pred>::type,
                typename list_prepend<typename list_filter_impl<comptime_list<T, Ts...>, Pred>::type, current>::type
            >
        >;
    };

}

template<typename T, template<T, T> typename Folder, T V, T W, auto R = Folder<V, W>::value>
constexpr detail::folder<decltype(R), Folder, R> operator+(detail::folder<T, Folder, V>, detail::value_wrapper<T, W>) noexcept;

template<typename T, template<T, T> typename Folder, T V, T W, auto R = Folder<V, W>::value>
constexpr detail::folder<decltype(R), Folder, R> operator+(detail::value_wrapper<T, W>, detail::folder<T, Folder, V>) noexcept;

template<typename List, template<typename List::value_type, typename List::value_type> typename Folder>
constexpr auto list_foldl = decltype([]<typename T, T First, T ...Ts>(comptime_list<T, First, Ts...>) ->
        decltype((detail::folder<T, Folder, First>{} + ... + detail::value_wrapper<T, Ts>{})) {}(std::declval<List>()))::value;

template<typename List, template<typename List::value_type, typename List::value_type> typename Folder>
constexpr auto list_foldr = decltype([]<typename T, T First, T ...Ts>(comptime_list<T, First, Ts...>) ->
        decltype((detail::value_wrapper<T, Ts>{} + ... + detail::folder<T, Folder, First>{})) {}(std::declval<List>()))::value;

template<typename L, template<typename L::value_type> typename Pred>
using list_filter = typename detail::list_filter_impl<L, Pred>::type;
