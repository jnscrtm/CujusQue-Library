#pragma once
#include <algorithm>
#include <compare>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace CQue
{
    template<class Lambda, int = (Lambda{}(), 0) >
    constexpr bool is_constexpr_evaluated_func(Lambda) { return true; }
    constexpr bool is_constexpr_evaluated_func(...) { return false; }

    template <class T>
    constexpr bool is_default_constexpr_declarable = is_constexpr_evaluated_func([]() -> T { return T(); });

    template <class T>
    constexpr bool is_copy_default_constexpr_declarable = is_constexpr_evaluated_func([]() -> T { T a = T(); return T(a); });

    template <class T>
    constexpr bool is_move_default_constexpr_declarable = is_constexpr_evaluated_func([]() -> T { return T(T()); });



    template <class TFrom, class TWhat>
    concept DecayedSameAs = std::same_as<std::decay_t<TFrom>, std::decay_t<TWhat>>;

    template <class TComp, class T>
    concept Comparer = requires(const T& a, const T& b)
    {
        { std::declval<TComp>().Compare(a, b) } -> std::convertible_to<int>;
    } || requires(const T& a, const T& b)
    {
        { std::declval<TComp>().Compare(a, b) } -> std::convertible_to<std::partial_ordering>;
    };

    template <class T>
    concept NonRValueRef = !std::is_rvalue_reference_v<T>;

    template <class T>
    concept NonLValueRef = !std::is_lvalue_reference_v<T>;

    template <class T>
    concept NonRef = !std::is_reference_v<T>;


    template <class TInput, class TOutput>
    using Converter = TOutput(*)(std::add_const_t<TInput>&);

    template <class TInput, class TOutput>
    constexpr TOutput DefaultConvert(std::add_const_t<TInput>& what)
    {
        return (TOutput) what;
    }

    template <class T>
    constexpr std::partial_ordering DefaultCompare(const T& a, const T& b)
    {
        return static_cast<std::partial_ordering>(a <=> b);
    }

    template <class T, class _Val>
    concept IterableObjectOf = requires(T a) 
    {
        { a.begin() } -> std::random_access_iterator;
        { a.end() } -> std::same_as<decltype(a.begin())>;
        { *(a.begin()) } -> DecayedSameAs<_Val>;
    };

    template <class T>
    concept Iterable = requires(T a)
    {
        { a.begin() } -> std::random_access_iterator;
        { a.end() } -> std::same_as<decltype(a.begin())>;
    };

    template <class T>
    using Predicate = bool(*)(T);

    template <class T>
    using Comparison = std::partial_ordering(*)(const T& a, const T& b);
};