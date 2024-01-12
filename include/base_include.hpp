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
    concept Iterable = requires(T a) 
    {
        { a.begin() } -> std::random_access_iterator;
        { a.end() } -> std::same_as<decltype(a.begin())>;
        { *(a.begin()) } -> DecayedSameAs<_Val>;
    };

    template <class T>
    using Predicate = bool(*)(T);

    template <class T>
    using Comparison = std::partial_ordering(*)(const T& a, const T& b);
};