#ifndef __CTLIST_CT_LIST_H__
#define __CTLIST_CT_LIST_H__
#include <type_traits>
#include <utility>
#include <string>

#include <iostream>

namespace ctlist
{
    namespace detail
    {
        template<std::size_t N, typename T, T... Ts> requires (N < sizeof...(Ts))
        struct get_list_element;
    
        template<std::size_t N, typename T, T v, T... Ts>
        struct get_list_element<N, T, v, Ts...> : get_list_element<N - 1, T, Ts...> {};
    
        template<typename T, T v, T... Ts>
        struct get_list_element<0, T, v, Ts...> { inline static constexpr T value = v; };
        
        template<typename T, T> struct value_wrapper { };
    }
    
    template<typename T, T... Ts>
    class comptime_list
    {
        using contained = comptime_list<T, Ts...>;
    public:
        using value_type = T;
        inline static constexpr std::size_t size = sizeof...(Ts);
    
        template<std::size_t N> requires (N < size)
        inline static constexpr T get = detail::get_list_element<N, T, Ts...>::value;
    
        template<typename M>
        static std::string to_string(M m)
        {
            if constexpr(size == 0)
                return "{}";
            else
            {
                auto ret = (std::string("{") + ... + (m(Ts) + ","));
                ret.pop_back();
                return ret + "}";
            }
        }
    };
}

#endif
