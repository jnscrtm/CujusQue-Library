#pragma once

#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

template <class T>
concept NonRValueRef = !std::is_rvalue_reference_v<T>;

template <class T>
concept NonLValueRef = !std::is_lvalue_reference_v<T>;

template <class T>
concept NonRef = !std::is_reference_v<T>;