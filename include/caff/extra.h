#pragma once

// tuple
namespace caff
{
    namespace detail
    {
        template<typename T, std::size_t I>
        concept has_tuple_element =
        requires(T t)
        {
            typename std::tuple_element_t<I, std::remove_const_t<T>>;

            // NOTE: This causes a "cannot form a reference to 'void'"
            // compile-time error with std::tuple<void>;
            //{ get<I>(t) } -> std::convertible_to<const std::tuple_element_t<I,T>&>;
        };

        template <typename T, std::size_t... I>
        constexpr bool has_tuple_elements(std::index_sequence<I...>)
        { 
            return (has_tuple_element<T,I> && ...);
        };
    }

    template<typename T>
    concept tuple_like = !std::is_reference_v<T> &&
    requires(T t)
    { 
        typename std::tuple_size<T>::type;
        requires std::derived_from<
            std::tuple_size<T>,
            std::integral_constant<std::size_t, std::tuple_size_v<T>>
        >;
    } && detail::has_tuple_elements<T>(
        std::make_index_sequence<std::tuple_size_v<T>>());

    template <tuple_like... Tuples>
    using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

    template <tuple_like Tuple, typename... Types>
    using tuple_append_t = tuple_cat_t<Tuple, std::tuple<Types...>>;

    template <tuple_like Tuple, typename... Types>
    using tuple_prepend_t = tuple_cat_t<std::tuple<Types...>, Tuple>;


    // iteration functions

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_for_each_index(F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<Is>(), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_for_each_type(F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<std::tuple_element_t<Is, tuple_t>>(), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_enumerate_types(F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<Is,
                std::tuple_element_t<Is, tuple_t>>(), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_for_each(Tuple&& t, F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f(std::get<Is>(std::forward<Tuple>(t))), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_enumerate(Tuple&& t, F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<Is>(
                std::get<Is>(std::forward<Tuple>(t))), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }
}

// type_list
namespace caff
{
    using standard_character_types = std::tuple
    <
        char,
        wchar_t,
        char8_t,
        char16_t,
        char32_t
    >;

    using signed_integer_types = std::tuple
    <
        short,
        int,
        long,
        long long
    >;

    using unsigned_integer_types = std::tuple
    <
        unsigned short,
        unsigned int,
        unsigned long,
        unsigned long long
    >;

    using integer_types = tuple_cat_t
    <
        signed_integer_types,
        unsigned_integer_types
    >;

    using signed_standard_integer_types = tuple_cat_t
    <
        std::tuple<signed char>,
        signed_integer_types
    >;

    using unsigned_standard_integer_types = tuple_cat_t
    <
        std::tuple<unsigned char>,
        unsigned_integer_types
    >;

    using standard_integer_types = tuple_cat_t
    <
        signed_standard_integer_types,
        unsigned_standard_integer_types
    >;

    using integral_types = tuple_cat_t
    <
        std::tuple<bool>,
        standard_character_types,
        standard_integer_types
    >;

    using standard_floating_point_types = std::tuple
    <
        float,
        double,
        long double
    >;
}

// type_traits
namespace caff
{
    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    struct is_any_of : std::disjunction<Trait<T, Ts>...>
    {
    };

    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    inline constexpr bool is_any_of_v = is_any_of<T, Trait, Ts...>::value;


    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    struct is_none_of : std::negation<is_any_of<T, Trait, Ts...>>
    {
    };

    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    inline constexpr bool is_none_of_v = is_none_of<T, Trait, Ts...>::value;

    template <typename T, typename... Ts>
    struct is_none_of_same : is_none_of<T, std::is_same, Ts...>
    {
    };

    template <typename T, typename... Ts>
    inline constexpr bool is_none_of_same_v = is_none_of_same<T, Ts...>::value;

    template <typename T>
    struct is_boolean : std::is_same<bool, std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_boolean_v = is_boolean<T>::value;

    template <typename T>
    struct is_standard_integer : std::conjunction<std::is_integral<T>,
        is_none_of_same<std::remove_cv_t<T>, bool, char, wchar_t, char8_t,
        char16_t, char32_t>> {};

    template <typename T>
    inline constexpr bool is_standard_integer_v = is_standard_integer<T>::value;
}

// concepts
namespace caff
{
    template <typename T>
    concept boolean = is_boolean_v<T>;

    template <typename T>
    concept standard_integer = is_standard_integer_v<T>;

    template <typename T>
    concept signed_standard_integer =
        is_standard_integer_v<T> && std::is_signed_v<T>;

    template <typename T>
    concept unsigned_standard_integer =
        is_standard_integer_v<T> && std::is_unsigned_v<T>;
}
