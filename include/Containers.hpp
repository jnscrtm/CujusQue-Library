#pragma once

#include "base_include.hpp"

// ####################################### FORWARD DECLARATIONS #######################################

namespace CQue::Container
{
    template <class T, ForwardIterableObjectOf<T> _Container>
    constexpr bool Exists(const _Container& container, Predicate<const T&> match);

    template <class T, ForwardIterableObjectOf<T> _Container>
    constexpr T Find(const _Container& container, Predicate<const T&> match);

    template <class T, ForwardIterableObjectOf<T> _Container, ForwardIterableObjectOf<T> _OutputContainer = _Container>
    constexpr _OutputContainer FindAll(const _Container& container, Predicate<const T&> match);

    template <class T, ForwardIterableObjectOf<T> _Container>
    constexpr std::size_t FindIndex(const _Container& container, Predicate<const T&> match);

    template <class T, ForwardIterableObjectOf<T> _Container>
    constexpr T FindLast(const _Container& container, Predicate<const T&> match);

    template <class T, ForwardIterableObjectOf<T> _Container>
    constexpr std::size_t FindLastIndex(const _Container& container, Predicate<const T&> match);

    template <std::equality_comparable T, ForwardIterableObjectOf<T> _Container>
    constexpr std::size_t IndexOf(const _Container& container, const T& what) noexcept(noexcept(std::declval<T>() == std::declval<T>()));

    template <std::equality_comparable T, BidirectionalIterableObjectOf<T> _Container>
    constexpr std::size_t LastIndexOf(const _Container& container, const T& what) noexcept(noexcept(std::declval<T>() == std::declval<T>()));

    template <RandomAccessIterable _Container>
    constexpr void Reverse(_Container& container);

    template <RandomAccessIterable _Container, class T = std::decay_t<decltype(*std::declval<_Container>().begin())>>
    constexpr void Sort(_Container& container, Comparison<T> compare = &DefaultCompare<T>);
};

namespace CQue
{
    template <std::forward_iterator _OutputIterator>
    constexpr _OutputIterator UninitializedDefaultConstruct(_OutputIterator _Dest, std::size_t n) noexcept(std::is_nothrow_default_constructible_v<std::remove_reference_t<decltype(*_Dest)>*>);

    template <std::forward_iterator _InputIterator, std::forward_iterator _OutputIterator>
    constexpr _OutputIterator UninitializedCopy(_InputIterator _First, _InputIterator _Last, _OutputIterator _Dest) noexcept(std::is_nothrow_copy_constructible_v<std::remove_reference_t<decltype(*_Dest)>*>);

    template <std::forward_iterator _InputIterator, std::forward_iterator _OutputIterator>
    constexpr _OutputIterator UninitializedMove(_InputIterator _First, _InputIterator _Last, _OutputIterator _Dest) noexcept(std::is_nothrow_move_constructible_v<std::remove_reference_t<decltype(*_Dest)>*>);

    /// @brief A naive, shallow wrapper class for referring to an iterable class whose iterators are convertible to or are themselves 
    /// pointers.
    /// @tparam T Type of object(s) to be iterated upon
    template <class T>
    class IterWrapper
    {
    public:
        // Constructor

        constexpr IterWrapper(T* first, T* last) noexcept;

        template <ForwardIterableObjectOf<T> _It>
        constexpr IterWrapper(const _It& iter) noexcept;

        // Iterators

        constexpr const T* begin() const noexcept;
        constexpr const T* end() const noexcept;
    private:
        T* _First, * _Last;
    };
};

// ######################################## BODY DECLARATIONS #########################################

namespace CQue::Container
{
    template <std::equality_comparable T, ForwardIterableObjectOf<T> _Container>
    constexpr bool Exists(const _Container& container, Predicate<const T&> match)
    {
        for(const auto& x : container)
            if(match(x)) return true;

        return false;
    }

    template <class T, ForwardIterableObjectOf<T> _Container>
    constexpr T Find(const _Container& container, Predicate<const T&> match)
    {
        for(const auto& x : container)
            if(match(x)) return x;

        return T();
    }

    template <class T, ForwardIterableObjectOf<T> _Container, ForwardIterableObjectOf<T> _OutputContainer>
    constexpr _OutputContainer FindAll(const _Container& container, Predicate<const T&> match)
    {
        std::size_t full_size = static_cast<std::size_t>(container.end() - container.begin());

        T* _Tmp = std::allocator<T>{}.allocate(full_size);

        std::size_t nfound = 0;
        for(const auto& x : container)
            if(match(x))
                std::construct_at(&_Tmp[nfound++], x);

        _OutputContainer out;

        if constexpr (std::is_constructible_v<_OutputContainer, T*, T*>)
            out = _OutputContainer(_Tmp, &_Tmp[nfound]);
        else
        {
            out = _OutputContainer{};
            std::copy(_Tmp, &_Tmp[nfound], out.begin());
        }

        std::allocator<T>{}.deallocate(_Tmp, full_size);
        return out;
    }

    template <class T, ForwardIterableObjectOf<T> _Container>
    constexpr std::size_t FindIndex(const _Container& container, Predicate<const T&> match)
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(container.end() - container.begin()); i++)
            if (match(*(container.begin() + i)))
                return i;

        return (std::size_t)(-1);
    }

    template <class T, ForwardIterableObjectOf<T> _Container>
    constexpr T FindLast(const _Container& container, Predicate<const T&> match)
    {
        std::size_t count = static_cast<std::size_t>(container.end() - container.begin());
        for (std::size_t i = 0; i < count; i++)
            if (match(*(container.begin() + count - i - 1)))
                return *(container.begin() + count - i - 1);

        return T();
    }

    template <class T, BidirectionalIterableObjectOf<T> _Container>
    constexpr std::size_t FindLastIndex(const _Container& container, Predicate<const T&> match)
    {
        for (auto iterator = container.end()-1; iterator >= container.begin(); iterator--)
            if (match(*iterator))
                return static_cast<std::size_t>(iterator - container.begin());

        return (std::size_t)(-1);
    }

    template <std::equality_comparable T, ForwardIterableObjectOf<T> _Iterable>
    constexpr std::size_t IndexOf(const _Iterable& container, const T& what) noexcept(noexcept(std::declval<T>() == std::declval<T>()))
    {
        for(auto iterator = container.begin(); iterator < container.end(); iterator++)
            if(*iterator == what)
                return static_cast<std::size_t>(iterator - container.begin());

        return (std::size_t)(-1);
    }

    template <std::equality_comparable T, ForwardIterableObjectOf<T> _Iterable>
    constexpr std::size_t LastIndexOf(const _Iterable& lst, const T& what) noexcept(noexcept(std::declval<T>() == std::declval<T>()))
    {
        std::size_t count = static_cast<std::size_t>(lst.end() - lst.begin());
        for (std::size_t i = 0; i < count; i++)
            if (*(lst.begin() + count - i - 1) == what)
                return count - i - 1;

        return (std::size_t)(-1);
    }

    template <RandomAccessIterable _Container>
    constexpr void Reverse(_Container& container)
    {
        std::size_t count = static_cast<std::size_t>(container.end() - container.begin());

        for (std::size_t i = 0; i < count / 2; i++)
            *(container.begin() + i) = std::exchange(*(container.begin() + count - i - 1), *(container.begin() + i));
    }

    template <RandomAccessIterable _Container, class T>
    constexpr void Sort(_Container& container, Comparison<T> compare)
    {
        std::size_t end = static_cast<std::size_t>(container.end() - container.begin());
        std::size_t start = end / 2;
        
        while (end > 1)
        {
            if (start > 0)
                start--;
            else
            {
                end--;
                *(container.begin() + end) = std::exchange(*container.begin(), *(container.begin() + end));
            }

            std::size_t root = start;
            while (root * 2 + 1 < end)
            {
                std::size_t child = root * 2 + 1;
                if (child + 1 < end && compare(*(container.begin() + child), *(container.begin() + child + 1)) == std::partial_ordering::less)
                    child++;

                if (*(container.begin() + root) < *(container.begin() + child))
                {
                    *(container.begin() + root) = std::exchange(*(container.begin() + child), *(container.begin() + root));
                    root = child;
                }
                else
                    break;
            }
        }
    }
};

namespace CQue
{
    template <std::forward_iterator _OutputIterator>
    constexpr _OutputIterator UninitializedDefaultConstruct(_OutputIterator _Dest, std::size_t n) noexcept(std::is_nothrow_default_constructible_v<std::remove_reference_t<decltype(*_Dest)>*>)
    {
        for (std::size_t i = 0; i < n; i++)
            std::construct_at((std::remove_reference_t<decltype(*_Dest)>*) _Dest++);

        return _Dest;
    }

    template <std::forward_iterator _InputIterator, std::forward_iterator _OutputIterator>
    constexpr _OutputIterator UninitializedCopy(_InputIterator _First, _InputIterator _Last, _OutputIterator _Dest) noexcept(std::is_nothrow_copy_constructible_v<std::remove_reference_t<decltype(*_Dest)>*>)
    {
        for (_InputIterator cur = _First; cur < _Last; cur++)
            std::construct_at((std::remove_reference_t<decltype(*_Dest)>*) _Dest++, *cur);

        return _Dest;
    }

    template <std::forward_iterator _InputIterator, std::forward_iterator _OutputIterator>
    constexpr _OutputIterator UninitializedMove(_InputIterator _First, _InputIterator _Last, _OutputIterator _Dest) noexcept(std::is_nothrow_move_constructible_v<std::remove_reference_t<decltype(*_Dest)>*>)
    {
        for (_InputIterator cur = _First; cur < _Last; cur++)
            std::construct_at((std::remove_reference_t<decltype(*_Dest)>*) _Dest++, std::move(*cur));

        return _Dest;
    }

    // ****************************************** IterWrapper<T> ******************************************

#if 1
    // IterWrapper<T> - Constructors

    template <class T>
    constexpr IterWrapper<T>::IterWrapper(T* first, T* last) noexcept : _First(first), _Last(last) {}

    template <class T>
    template <ForwardIterableObjectOf<T> _It>
    constexpr IterWrapper<T>::IterWrapper(const _It& iter) noexcept : _First((T*)iter.begin()), _Last((T*)iter.end()) {}

    // IterWrapper<T> - Iterators

    template <class T>
    constexpr const T* IterWrapper<T>::begin() const noexcept { return _First; }

    template <class T>
    constexpr const T* IterWrapper<T>::end() const noexcept { return _Last; }
#endif
};

#include "List.hpp"