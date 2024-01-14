#pragma once

#include "base_include.hpp"

// ####################################### FORWARD DECLARATIONS #######################################

namespace CQue::Container
{
    template <class T, IterableObjectOf<T> _Container>
    constexpr bool Exists(const _Container& container, Predicate<const T&> match);

    template <class T, IterableObjectOf<T> _Container>
    constexpr T Find(const _Container& container, Predicate<const T&> match);

    template <class T, IterableObjectOf<T> _Container, IterableObjectOf<T> _OutputContainer = _Container>
    constexpr _OutputContainer FindAll(const _Container& container, Predicate<const T&> match);

    template <class T, IterableObjectOf<T> _Container>
    constexpr std::size_t FindIndex(const _Container& container, Predicate<const T&> match);

    template <class T, IterableObjectOf<T> _Container>
    constexpr T FindLast(const _Container& container, Predicate<const T&> match);

    template <class T, IterableObjectOf<T> _Container>
    constexpr std::size_t FindLastIndex(const _Container& container, Predicate<const T&> match);

    template <std::equality_comparable T, IterableObjectOf<T> _Container>
    constexpr std::size_t IndexOf(const _Container& container, const T& what) noexcept(noexcept(std::declval<T>() == std::declval<T>()));

    template <std::equality_comparable T, IterableObjectOf<T> _Container>
    constexpr std::size_t LastIndexOf(const _Container& container, const T& what) noexcept(noexcept(std::declval<T>() == std::declval<T>()));

    template <Iterable _Container>
    constexpr void Reverse(_Container& container);

    template <Iterable _Container, class T = std::decay_t<decltype(*std::declval<_Container>().begin())>>
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

        template <IterableObjectOf<T> _It>
        constexpr IterWrapper(const _It& iter) noexcept;

        // Iterators

        constexpr const T* begin() const noexcept;
        constexpr const T* end() const noexcept;
    private:
        T* _First, * _Last;
    };

    /// @brief Not really an allocator class, but may be used within the library as a marker that
    /// the container wants allocation directly using new-delete operators, in case of which the type
    /// needs to be default initializable
    template <class T>
    class NonAllocator
    {
    public:
        T* allocate(std::size_t) { throw std::logic_error(""); return nullptr; }
        void deallocate(T*, std::size_t) { throw std::logic_error(""); return; }
    };

    /// @brief A contiguous, array-based collection equipped with indexing and some helper methods.
    /// The member functions' names are unashamedly given due to .NET generic collection library.
    /// @tparam T 
    template <class T, class Allocator = std::allocator<T>>
    class List
    {
    public:
        // Constructors

        constexpr List() noexcept;
        constexpr List(const List<T, Allocator>& other) noexcept(std::is_nothrow_copy_constructible_v<T>);
        constexpr List(List<T, Allocator>&& other) noexcept;

        constexpr List(std::size_t initial_size) noexcept(std::is_nothrow_default_constructible_v<T>) requires std::default_initializable<T>;

        template <IterableObjectOf<T> _It>
        constexpr List(const _It& lst) noexcept(std::is_nothrow_copy_constructible_v<T>);

        template <IterableObjectOf<T> _It>
        constexpr List(_It&& lst) noexcept(std::is_nothrow_move_constructible_v<T>);

        // Non-Template Member Functions

        constexpr void Add(const T& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>);
        constexpr void Add(T&& what) noexcept(std::is_nothrow_move_assignable_v<T>);
        constexpr std::size_t Capacity() const noexcept;
        constexpr void Clear() noexcept(std::is_nothrow_destructible_v<T>);
        constexpr bool Contains(const T& what) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>;
        constexpr std::size_t Count() const noexcept;
        constexpr bool Exists(Predicate<const T&> match) const;
        constexpr T Find(Predicate<const T&> match) const;
        constexpr List<T, Allocator> FindAll(Predicate<const T&> match) const;
        constexpr std::size_t FindIndex(Predicate<const T&> match) const;
        constexpr T FindLast(Predicate<const T&> match) const;
        constexpr std::size_t FindLastIndex(Predicate<const T&> match) const;
        constexpr std::size_t IndexOf(const T& what) const noexcept;
        constexpr void Insert(std::size_t index, const T& what);
        constexpr void Insert(std::size_t index, T&& what);
        constexpr std::size_t LastIndexOf(const T& what) const noexcept;
        constexpr bool Remove(const T& what) noexcept;
        constexpr void RemoveAt(std::size_t index);
        constexpr void RemoveRange(std::size_t index, std::size_t count);
        constexpr void Resize(std::size_t n) noexcept requires std::default_initializable<T>;
        constexpr void Reverse() noexcept(std::is_nothrow_move_assignable_v<T>);
        constexpr void Sort(Comparison<T> compare = &DefaultCompare<T>);

        // Template Member Functions

        template <IterableObjectOf<T> _It>
        constexpr void AddRange(const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>);

        template <IterableObjectOf<T> _It>
        constexpr void AddRange(_It&& what) noexcept(std::is_nothrow_move_assignable_v<T>);

        template <class TOutput>
        constexpr List<TOutput> ConvertAll(Converter<T, TOutput> converter = &DefaultConvert<T, TOutput>) const;

        template <IterableObjectOf<T> _It>
        constexpr void CopyTo(_It& where) const noexcept(std::is_nothrow_copy_assignable_v<T>);

        template <IterableObjectOf<T> _It>
        constexpr void InsertRange(std::size_t index, const _It& what);

        template <IterableObjectOf<T> _It>
        constexpr void InsertRange(std::size_t index, _It&& what);

        // Iterators

        constexpr T* begin() const noexcept;
        constexpr T* end() const noexcept;
        constexpr const T* cbegin() const noexcept;
        constexpr const T* cend() const noexcept;

        // Operators

        constexpr List<T, Allocator>& operator=(const List<T, Allocator>& other);
        constexpr List<T, Allocator>& operator=(List<T, Allocator>&& other);

        constexpr T& operator[](std::size_t index);
        constexpr const T& operator[](std::size_t index) const;

        constexpr bool operator==(const List<T, Allocator>& other) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>;

        // Destructor

        constexpr ~List() noexcept(std::is_nothrow_destructible_v<T>);

    protected:
        constexpr void _Reallocate(std::size_t new_capacity) noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_destructible_v<T>);
        constexpr void _Release() noexcept(std::is_nothrow_destructible_v<T>);

        std::size_t _Capacity;
        std::size_t _Count;
        T* _Elems;

        static inline Allocator _Alloc{};
    };

    /// @brief Similar to `CQue::List<T, Allocator>` except that `SimpleList<T>` does not use an allocator and instead uses new-delete
    /// operators directly. The type hence implicitly also needs to be default-initializable.
    /// @tparam T 
    template <class T>
    class SimpleList
    {
    public:
        // Constructors

        constexpr SimpleList() noexcept;
        constexpr SimpleList(const SimpleList<T>& other) noexcept;
        constexpr SimpleList(SimpleList<T>&& other) noexcept;

        constexpr SimpleList(std::size_t initial_size) noexcept;

        template <IterableObjectOf<T> _It>
        constexpr SimpleList(const _It& lst) noexcept;

        template <IterableObjectOf<T> _It>
        constexpr SimpleList(_It&& lst) noexcept;

        // Non-Template Member Functions

        constexpr void Add(const T& what) noexcept;
        constexpr void Add(T&& what) noexcept;
        constexpr std::size_t Capacity() const noexcept;
        constexpr void Clear() noexcept;
        constexpr bool Contains(const T& what) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>;
        constexpr std::size_t Count() const noexcept;
        constexpr bool Exists(Predicate<const T&> match) const;
        constexpr T Find(Predicate<const T&> match) const;
        constexpr SimpleList<T> FindAll(Predicate<const T&> match) const;
        constexpr std::size_t FindIndex(Predicate<const T&> match) const;
        constexpr T FindLast(Predicate<const T&> match) const;
        constexpr std::size_t FindLastIndex(Predicate<const T&> match) const;
        constexpr std::size_t IndexOf(const T& what) const noexcept;
        constexpr void Insert(std::size_t index, const T& what);
        constexpr void Insert(std::size_t index, T&& what);
        constexpr std::size_t LastIndexOf(const T& what) const noexcept;
        constexpr bool Remove(const T& what) noexcept;
        constexpr void RemoveAt(std::size_t index);
        constexpr void RemoveRange(std::size_t index, std::size_t count);
        constexpr void Resize(std::size_t n) noexcept;
        constexpr void Reverse() noexcept;
        constexpr void Sort(Comparison<T> compare = &DefaultCompare<T>);

        // Template Member Functions

        template <IterableObjectOf<T> _It>
        constexpr void AddRange(const _It& what) noexcept;

        template <IterableObjectOf<T> _It>
        constexpr void AddRange(_It&& what) noexcept;

        template <class TOutput>
        constexpr List<TOutput> ConvertAll(Converter<T, TOutput> converter = &DefaultConvert<T, TOutput>) const;

        template <IterableObjectOf<T> _It>
        constexpr void CopyTo(_It& where) const noexcept;

        template <IterableObjectOf<T> _It>
        constexpr void InsertRange(std::size_t index, const _It& what) noexcept;

        template <IterableObjectOf<T> _It>
        constexpr void InsertRange(std::size_t index, _It&& what);

        // Iterators

        constexpr T* begin() const noexcept;
        constexpr T* end() const noexcept;
        constexpr const T* cbegin() const noexcept;
        constexpr const T* cend() const noexcept;

        // Operators

        constexpr SimpleList<T>& operator=(const List<T>& other);
        constexpr SimpleList<T>& operator=(SimpleList<T>&& other);

        constexpr T& operator[](std::size_t index);
        constexpr const T& operator[](std::size_t index) const;

        constexpr bool operator==(const SimpleList<T>& other) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>;

        // Destructor

        constexpr ~SimpleList() noexcept;

    protected:
        constexpr void _Reallocate(std::size_t new_capacity) noexcept;
        constexpr void _Release() noexcept(std::is_nothrow_destructible_v<T>);

        std::size_t _Capacity;
        std::size_t _Count;
        T* _Elems;
    };
};

// ######################################## BODY DECLARATIONS #########################################

namespace CQue::Container
{
    template <std::equality_comparable T, IterableObjectOf<T> _Container>
    constexpr bool Exists(const _Container& container, Predicate<const T&> match)
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(container.end() - container.begin()); i++)
            if (match(*(container.begin() + i)))
                return true;

        return false;
    }

    template <class T, IterableObjectOf<T> _Container>
    constexpr T Find(const _Container& container, Predicate<const T&> match)
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(container.end() - container.begin()); i++)
            if (match(*(container.begin() + i)))
                return *(container.begin() + i);

        return T();
    }

    template <class T, IterableObjectOf<T> _Container, IterableObjectOf<T> _OutputContainer>
    constexpr _OutputContainer FindAll(const _Container& container, Predicate<const T&> match)
    {
        std::size_t full_size = static_cast<std::size_t>(container.end() - container.begin());

        T* _Tmp = std::allocator<T>{}.allocate(full_size);
        std::size_t nfound = 0;

        for (std::size_t i = 0; i < static_cast<std::size_t>(container.end() - container.begin()); i++)
            if (match(*(container.begin() + i)))
                std::construct_at(&_Tmp[nfound++], *(container.begin() + i));

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

    template <class T, IterableObjectOf<T> _Container>
    constexpr std::size_t FindIndex(const _Container& container, Predicate<const T&> match)
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(container.end() - container.begin()); i++)
            if (match(*(container.begin() + i)))
                return i;

        return (std::size_t)(-1);
    }

    template <class T, IterableObjectOf<T> _Container>
    constexpr T FindLast(const _Container& container, Predicate<const T&> match)
    {
        std::size_t count = static_cast<std::size_t>(container.end() - container.begin());
        for (std::size_t i = 0; i < count; i++)
            if (match(*(container.begin() + count - i - 1)))
                return *(container.begin() + count - i - 1);

        return T();
    }

    template <class T, IterableObjectOf<T> _Container>
    constexpr std::size_t FindLastIndex(const _Container& container, Predicate<const T&> match)
    {
        std::size_t count = static_cast<std::size_t>(container.end() - container.begin());
        for (std::size_t i = 0; i < count; i++)
            if (match(*(container.begin() + count - i - 1)))
                return count - i - 1;

        return (std::size_t)(-1);
    }

    template <std::equality_comparable T, IterableObjectOf<T> _Iterable>
    constexpr std::size_t IndexOf(const _Iterable& lst, const T& what) noexcept(noexcept(std::declval<T>() == std::declval<T>()))
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(lst.end() - lst.begin()); i++)
            if (*(lst.begin() + i) == what)
                return i;

        return (std::size_t)(-1);
    }

    template <std::equality_comparable T, IterableObjectOf<T> _Iterable>
    constexpr std::size_t LastIndexOf(const _Iterable& lst, const T& what) noexcept(noexcept(std::declval<T>() == std::declval<T>()))
    {
        std::size_t count = static_cast<std::size_t>(lst.end() - lst.begin());
        for (std::size_t i = 0; i < count; i++)
            if (*(lst.begin() + count - i - 1) == what)
                return count - i - 1;

        return (std::size_t)(-1);
    }

    template <Iterable _Container>
    constexpr void Reverse(_Container& container)
    {
        std::size_t count = static_cast<std::size_t>(container.end() - container.begin());

        for (std::size_t i = 0; i < count / 2; i++)
            *(container.begin() + i) = std::exchange(*(container.begin() + count - i - 1), *(container.begin() + i));
    }

    template <Iterable _Container, class T>
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
    template <IterableObjectOf<T> _It>
    constexpr IterWrapper<T>::IterWrapper(const _It& iter) noexcept : _First((T*)iter.begin()), _Last((T*)iter.end()) {}

    // IterWrapper<T> - Iterators

    template <class T>
    constexpr const T* IterWrapper<T>::begin() const noexcept { return _First; }

    template <class T>
    constexpr const T* IterWrapper<T>::end() const noexcept { return _Last; }
#endif


    // **************************************** List<T, Allocator> ****************************************

#if 1

    // List<T, Allocator> - Constructors

    template <class T, class Allocator>
    constexpr List<T, Allocator>::List() noexcept : _Capacity(0), _Count(0), _Elems(nullptr) {}

    template <class T, class Allocator>
    constexpr List<T, Allocator>::List(const List<T, Allocator>& other) noexcept(std::is_nothrow_copy_constructible_v<T>) : _Capacity(other._Count), _Count(other._Count), _Elems(other._Elems ? _Alloc.allocate(other._Count) : nullptr)
    {
        for (std::size_t i = 0; i < other._Count; i++)
            std::construct_at(&_Elems[i], other._Elems[i]);
    }

    template <class T, class Allocator>
    constexpr List<T, Allocator>::List(List<T, Allocator>&& other) noexcept : _Capacity(std::exchange(other._Capacity, 0)), _Count(std::exchange(other._Count, 0)), _Elems(std::exchange(other._Elems, nullptr)) {}

    template <class T, class Allocator>
    constexpr List<T, Allocator>::List(std::size_t initial_size) noexcept(std::is_nothrow_default_constructible_v<T>) requires std::default_initializable<T> : _Capacity(initial_size), _Count(initial_size), _Elems(_Alloc.allocate(initial_size))
    {
        UninitializedDefaultConstruct(_Elems, initial_size);
    }

    template <class T, class Allocator>
    template <IterableObjectOf<T> _It>
    constexpr List<T, Allocator>::List(const _It& lst) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = _Alloc.allocate(_Capacity);

        UninitializedCopy(lst.begin(), lst.end(), _Elems);
    }

    template <class T, class Allocator>
    template <IterableObjectOf<T> _It>
    constexpr List<T, Allocator>::List(_It&& lst) noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = _Alloc.allocate(_Capacity);

        UninitializedMove(lst.begin(), lst.end(), _Elems);
    }

    // List<T, Allocator> - Non-Template Member Functions

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Add(const T& what) noexcept(std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_move_assignable_v<T>)
    {
        if (_Count == _Capacity)
        {
            if (_Capacity == 0)
                _Elems = _Alloc.allocate(_Capacity = 1);
            else
                _Reallocate(_Capacity * 2);
        }

        std::construct_at(&_Elems[_Count++], what);
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Add(T&& what) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        if (_Count == _Capacity)
        {
            if (_Capacity == 0)
                _Elems = _Alloc.allocate(_Capacity = 1);
            else
                _Reallocate(_Capacity * 2);
        }

        std::construct_at(&_Elems[_Count++], std::move(what));
    }

    template <class T, class Allocator>
    constexpr std::size_t List<T, Allocator>::Capacity() const noexcept
    {
        return _Capacity;
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Clear() noexcept(std::is_nothrow_destructible_v<T>)
    {
        std::destroy_n(_Elems, _Count);
        _Count = 0;
    }

    template <class T, class Allocator>
    constexpr bool List<T, Allocator>::Contains(const T& what) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (_Elems[i] == what)
                return true;

        return false;
    }

    template <class T, class Allocator>
    constexpr std::size_t List<T, Allocator>::Count() const noexcept
    {
        return _Count;
    }

    template <class T, class Allocator>
    constexpr bool List<T, Allocator>::Exists(Predicate<const T&> match) const
    {
        return Container::Exists<T>(*this, match);
    }

    template <class T, class Allocator>
    constexpr T List<T, Allocator>::Find(Predicate<const T&> match) const
    {
        return Container::Find<T>(*this, match);
    }

    template <class T, class Allocator>
    constexpr List<T, Allocator> List<T, Allocator>::FindAll(Predicate<const T&> match) const
    {
        return Container::FindAll<T>(*this, match);
    }

    template <class T, class Allocator>
    constexpr std::size_t List<T, Allocator>::FindIndex(Predicate<const T&> match) const
    {
        return Container::FindIndex<T>(*this, match);
    }

    template <class T, class Allocator>
    constexpr T List<T, Allocator>::FindLast(Predicate<const T&> match) const
    {
        return Container::FindLast<T>(*this, match);
    }

    template <class T, class Allocator>
    constexpr std::size_t List<T, Allocator>::FindLastIndex(Predicate<const T&> match) const
    {
        return Container::FindLastIndex<T>(match);
    }

    template <class T, class Allocator>
    constexpr std::size_t List<T, Allocator>::IndexOf(const T& what) const noexcept
    {
        return Container::IndexOf<T>(*this, what);
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Insert(std::size_t index, const T& what)
    {
        if (index == _Count)
            this->Add(what);
        else if (index < _Count)
        {
            if (_Count == _Capacity)
            {
                T* new_Elems = _Alloc.allocate(_Capacity * 2);

                UninitializedMove(_Elems, &_Elems[index], new_Elems);
                std::construct_at(&new_Elems[index], what);
                UninitializedMove(&_Elems[index], &_Elems[_Count], &new_Elems[index + 1]);

                _Release();

                _Elems = new_Elems;
                _Capacity *= 2;
            }
            else
            {
                std::construct_at(&_Elems[_Count], std::move(_Elems[_Count - 1]));
                std::move_backward(&_Elems[index], &_Elems[_Count - 1], &_Elems[_Count - 1]);
                _Elems[index] = what;
            }

            _Count++;
        }
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Insert(std::size_t index, T&& what)
    {
        if (index == _Count)
            this->Add(std::move(what));
        else if (index < _Count)
        {
            if (_Count == _Capacity)
            {
                T* new_Elems = _Alloc.allocate(_Capacity * 2);

                UninitializedMove(_Elems, &_Elems[index], new_Elems);
                std::construct_at(&new_Elems[index], std::move(what));
                UninitializedMove(&_Elems[index], &_Elems[_Count], &new_Elems[index + 1]);

                _Release();
                _Capacity *= 2;
                _Elems = new_Elems;
            }
            else
            {
                std::construct_at(&_Elems[_Count], std::move(_Elems[_Count - 1]));
                std::move_backward(&_Elems[index], &_Elems[_Count - 1], &_Elems[_Count - 1]);
                _Elems[index] = std::move(what);
            }

            _Count++;
        }

        else
            throw std::out_of_range("index");
    }

    template <class T, class Allocator>
    constexpr std::size_t List<T, Allocator>::LastIndexOf(const T& what) const noexcept
    {
        return Container::LastIndexOf<T>(*this, what);
    }

    template <class T, class Allocator>
    constexpr bool List<T, Allocator>::Remove(const T& what) noexcept
    {
        try
        {
            std::size_t pos = IndexOf(what);
            if (pos != (std::size_t)(-1))
            {
                RemoveAt(pos);
                return true;
            }
            else
                return false;
        }
        catch (...)
        {
            return false;
        }
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::RemoveAt(std::size_t index)
    {
        if (index < _Count)
        {
            std::destroy_at(&_Elems[index]);
            std::construct_at(&_Elems[index]);
            std::move(&_Elems[index + 1], &_Elems[_Count], &_Elems[index]);
            std::destroy_at(&_Elems[_Count--]);
        }
        else
            throw std::out_of_range("where");
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::RemoveRange(std::size_t index, std::size_t count)
    {
        if (index + count <= _Count)
        {
            std::destroy_n(&_Elems[index], count);
            UninitializedDefaultConstruct(&_Elems[index], count);

            std::move(&_Elems[index + count], &_Elems[_Count], &_Elems[index]);
            _Count -= count;
        }
        else
            throw std::out_of_range("where");
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Resize(std::size_t n) noexcept requires std::default_initializable<T>
    {
        if (n < _Count)
            std::destroy_n(&_Elems[_Count - n], n);

        _Reallocate(_Count = n);
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Reverse() noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        for (std::size_t i = 0; i < _Count / 2; i++)
            _Elems[i] = std::exchange(_Elems[_Count - i - 1], _Elems[i]);
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Sort(Comparison<T> compare)
    {
        Container::Sort<List<T, Allocator>>(*this, compare);
    }

    // List<T, Allocator> - Template Member Functions

    template <class T, class Allocator>
    template <IterableObjectOf<T> _It>
    constexpr void List<T, Allocator>::AddRange(const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>)
    {
        std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

        if (add_count > _Capacity - _Count)
            _Reallocate(_Count * 2 + add_count);

        UninitializedCopy(what.begin(), what.end(), &_Elems[_Count]);
        _Count += add_count;
    }

    template <class T, class Allocator>
    template <IterableObjectOf<T> _It>
    constexpr void List<T, Allocator>::AddRange(_It&& what) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

        if (add_count > _Capacity - _Count)
            _Reallocate(_Count * 2 + add_count);

        UninitializedMove(what.begin(), what.end(), &_Elems[_Count]);
        _Count += add_count;
    }

    template <class T, class Allocator>
    template <class TOutput>
    constexpr List<TOutput> List<T, Allocator>::ConvertAll(Converter<T, TOutput> converter) const
    {
        List<TOutput> out(_Count);
        for (std::size_t i = 0; i < _Count; i++)
            out[i] = converter(_Elems[i]);

        return out;
    }

    template <class T, class Allocator>
    template <IterableObjectOf<T> _It>
    constexpr void List<T, Allocator>::CopyTo(_It& where) const noexcept(std::is_nothrow_copy_assignable_v<T>)
    {
        std::copy(_Elems, &_Elems[_Count], where.begin());
    }

    template <class T, class Allocator>
    template <IterableObjectOf<T> _It>
    constexpr void List<T, Allocator>::InsertRange(std::size_t index, const _It& what)
    {
        if (index == _Count)
            this->AddRange(what);
        else if (index < _Count)
        {
            std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

            // If the number of items to be added exceeds the remaining space, allocate a new chunk of memory and move the items
            // The move is split into two parts for more efficiency: items before the place of insertion and those after
            if (add_count > _Capacity - _Count)
            {
                T* new_Elems = _Alloc.allocate(_Count * 2 + add_count);

                UninitializedMove(_Elems, &_Elems[index], new_Elems);
                UninitializedMove(&_Elems[index], &_Elems[_Count], &new_Elems[add_count]);
                UninitializedCopy(what.begin(), what.end(), &new_Elems[index]);

                _Release();
                _Elems = new_Elems;
                _Capacity = _Count * 2 + add_count;
            }
            // Otherwise, shift items after the place of insertion to give space for items that are to be added
            else
            {
                UninitializedDefaultConstruct(&_Elems[_Count], add_count);
                std::move_backward(&_Elems[index], &_Elems[_Count], &_Elems[_Count + add_count]);
                std::copy(what.begin(), what.end(), &_Elems[index]);
            }

            _Count += add_count;
        }
        else
            throw std::out_of_range("index");
    }

    template <class T, class Allocator>
    template <IterableObjectOf<T> _It>
    constexpr void List<T, Allocator>::InsertRange(std::size_t index, _It&& what)
    {
        if (index == _Count)
            this->AddRange(std::move(what));
        else if (index < _Count)
        {
            std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

            // If the number of items to be added exceeds the remaining space, allocate a new chunk of memory and move the items
            // The move is split into two parts for more efficiency: items before the place of insertion and those after
            if (add_count > _Capacity - _Count)
            {
                T* new_Elems = _Alloc.allocate(_Count * 2 + add_count);

                UninitializedMove(_Elems, &_Elems[index], new_Elems);
                UninitializedMove(&_Elems[index], &_Elems[_Count], &new_Elems[add_count]);
                UninitializedCopy(what.begin(), what.end(), &new_Elems[index]);

                _Release();
                _Elems = new_Elems;
                _Capacity = _Count * 2 + add_count;
            }
            // Otherwise, shift items after the place of insertion to give space for items that are to be added
            else
            {
                UninitializedDefaultConstruct(&_Elems[_Count], add_count);
                std::move_backward(&_Elems[index], &_Elems[_Count], &_Elems[_Count + add_count]);
                std::move(what.begin(), what.end(), &_Elems[index]);
            }

            _Count += add_count;
        }
        else
            throw std::out_of_range("index");
    }

    // List<T, Allocator> - Iterators

    template <class T, class Allocator>
    constexpr T* List<T, Allocator>::begin() const noexcept
    {
        return _Elems;
    }

    template <class T, class Allocator>
    constexpr T* List<T, Allocator>::end() const noexcept
    {
        return &_Elems[_Count];
    }

    template <class T, class Allocator>
    constexpr const T* List<T, Allocator>::cbegin() const noexcept
    {
        return _Elems;
    }

    template <class T, class Allocator>
    constexpr const T* List<T, Allocator>::cend() const noexcept
    {
        return &_Elems[_Count];
    }

    template <class T, class Allocator>
    constexpr List<T, Allocator>& List<T, Allocator>::operator=(const List<T, Allocator>& other)
    {
        if (_Capacity < other._Count)
        {
            _Release();
            _Elems = _Alloc.allocate(_Capacity = other._Count);
        }
        else
            std::destroy_n(_Elems, _Count);

        for (std::size_t i = 0; i < other._Count; i++)
            std::construct_at(&_Elems[i], other._Elems[i]);

        _Count = other._Count;

        return *this;
    }

    // List<T, Allocator> - Operators

    template <class T, class Allocator>
    constexpr List<T, Allocator>& List<T, Allocator>::operator=(List<T, Allocator>&& other)
    {
        if (_Elems)
            _Release();

        _Elems = std::exchange(other._Elems, nullptr);
        _Count = std::exchange(other._Count, 0);
        _Capacity = std::exchange(other._Capacity, 0);

        return *this;
    }

    template <class T, class Allocator>
    constexpr T& List<T, Allocator>::operator[](std::size_t index)
    {
        if (index < _Count)
            return _Elems[index];
        else
            throw std::out_of_range("index");
    }

    template <class T, class Allocator>
    constexpr const T& List<T, Allocator>::operator[](std::size_t index) const
    {
        if (index < _Count)
            return _Elems[index];
        else
            throw std::out_of_range("index");
    }

    template <class T, class Allocator>
    constexpr bool List<T, Allocator>::operator==(const List<T, Allocator>& other) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>
    {
        if (_Count != other._Count)
            return false;
        else
        {
            for (std::size_t i = 0; i < _Count; i++)
            {
                if (_Elems[i] != other._Elems[i])
                    return false;
            }
            return true;
        }
    }

    // List<T, Allocator> - Destructor

    template <class T, class Allocator>
    constexpr List<T, Allocator>::~List() noexcept(std::is_nothrow_destructible_v<T>)
    {
        _Release();
    }

    // List<T, Allocator> - Protected Member Functions

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::_Reallocate(std::size_t new_capacity) noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_destructible_v<T>)
    {
        if (_Capacity == 0)
            _Elems = _Alloc.allocate(new_capacity);
        else if (_Capacity != new_capacity)
        {
            T* new_Elems = _Alloc.allocate(new_capacity);
            UninitializedMove(_Elems, &_Elems[_Count], new_Elems);

            _Release();
            _Elems = new_Elems;
        }

        _Capacity = new_capacity;
    }

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::_Release() noexcept(std::is_nothrow_destructible_v<T>)
    {
        std::destroy_n(_Elems, _Count);
        _Alloc.deallocate(_Elems, _Capacity);
    }
#endif


    // ****************************************** SimpleList<T> *******************************************

#if 1
    template <class T>
    constexpr SimpleList<T>::SimpleList() noexcept : _Capacity(0), _Count(0), _Elems(0) {}

    template <class T>
    constexpr SimpleList<T>::SimpleList(const SimpleList<T>& other) noexcept : _Capacity(other._Count), _Count(other._Count), _Elems(other._Elems ? new T[other._Count]{} : nullptr) {}

    template <class T>
    constexpr SimpleList<T>::SimpleList(SimpleList<T>&& other) noexcept : _Capacity(std::exchange(other._Capacity, 0)), _Count(std::exchange(other._Count, 0)), _Elems(std::exchange(other._Elems, nullptr)) {}

    template <class T>
    constexpr SimpleList<T>::SimpleList(std::size_t initial_size) noexcept : _Capacity(initial_size), _Count(initial_size), _Elems(new T[initial_size]{}) {}

    template <class T>
    template <IterableObjectOf<T> _It>
    constexpr SimpleList<T>::SimpleList(const _It& lst) noexcept
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = new T[_Capacity]{};

        std::copy(lst.begin(), lst.end(), _Elems);
    }

    template <class T>
    template <IterableObjectOf<T> _It>
    constexpr SimpleList<T>::SimpleList(_It&& lst) noexcept
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = new T[_Capacity]{};

        std::move(lst.begin(), lst.end(), _Elems);
    }

    // Non-Template Member Functions

    template <class T>
    constexpr void SimpleList<T>::Add(const T& what) noexcept
    {
        if (_Count == _Capacity)
        {
            if (_Capacity == 0)
                _Elems = new T[_Capacity = 1]{};
            else
                _Reallocate(_Capacity * 2);
        }

        _Elems[_Count++] = what;
    }

    template <class T>
    constexpr void SimpleList<T>::Add(T&& what) noexcept
    {
        if (_Count == _Capacity)
        {
            if (_Capacity == 0)
                _Elems = new T[_Capacity = 1]{};
            else
                _Reallocate(_Capacity * 2);
        }

        _Elems[_Count++] = std::move(what);
    }

    template <class T>
    constexpr std::size_t SimpleList<T>::Capacity() const noexcept
    {
        return _Capacity;
    }

    template <class T>
    constexpr void SimpleList<T>::Clear() noexcept
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            std::destroy_n(_Elems, _Count);
            UninitializedDefaultConstruct(_Elems, _Count);
        }

        _Count = 0;
    }

    template <class T>
    constexpr bool SimpleList<T>::Contains(const T& what) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (_Elems[i] == what)
                return true;

        return false;
    }

    template <class T>
    constexpr std::size_t SimpleList<T>::Count() const noexcept
    {
        return _Count;
    }

    template <class T>
    constexpr bool SimpleList<T>::Exists(Predicate<const T&> match) const
    {
        return Container::Exists<T>(*this, match);
    }

    template <class T>
    constexpr T SimpleList<T>::Find(Predicate<const T&> match) const
    {
        return Container::Find<T>(*this, match);
    }

    template <class T>
    constexpr SimpleList<T> SimpleList<T>::FindAll(Predicate<const T&> match) const
    {
        return Container::FindAll<T>(*this, match);
    }

    template <class T>
    constexpr std::size_t SimpleList<T>::FindIndex(Predicate<const T&> match) const
    {
        return Container::FindIndex<T>(*this, match);
    }

    template <class T>
    constexpr T SimpleList<T>::FindLast(Predicate<const T&> match) const
    {
        return Container::FindLast<T>(*this, match);
    }

    template <class T>
    constexpr std::size_t SimpleList<T>::FindLastIndex(Predicate<const T&> match) const
    {
        return Container::FindLastIndex<T>(*this, match);
    }

    template <class T>
    constexpr std::size_t SimpleList<T>::IndexOf(const T& what) const noexcept
    {
        return Container::IndexOf<T>(*this, what);
    }

    template <class T>
    constexpr void SimpleList<T>::Insert(std::size_t index, const T& what)
    {
        if (index == _Count)
            this->Add(what);
        else if (index < _Count)
        {
            if (_Count == _Capacity)
            {
                T* new_Elems = new T[_Capacity * 2]{};

                std::move(_Elems, &_Elems[index], new_Elems);
                std::move(&_Elems[index], &_Elems[_Count], &new_Elems[index + 1]);

                delete[] _Elems;

                _Elems = new_Elems;
                _Capacity *= 2;
            }
            else
                std::move_backward(&_Elems[index], &_Elems[_Count], &_Elems[_Count]);

            _Elems[index] = what;
            _Count++;
        }
    }

    template <class T>
    constexpr void SimpleList<T>::Insert(std::size_t index, T&& what)
    {
        if (index == _Count)
            this->Add(std::move(what));
        else if (index < _Count)
        {
            if (_Count == _Capacity)
            {
                T* new_Elems = new T[_Capacity * 2]{};

                std::move(_Elems, &_Elems[index], new_Elems);
                std::move(&_Elems[index], &_Elems[_Count], &new_Elems[index + 1]);

                delete[] _Elems;

                _Elems = new_Elems;
                _Capacity *= 2;
            }
            else
                std::move_backward(&_Elems[index], &_Elems[_Count], &_Elems[_Count]);

            _Elems[index] = std::move(what);
            _Count++;
        }
    }

    template <class T>
    constexpr std::size_t SimpleList<T>::LastIndexOf(const T& what) const noexcept
    {
        return Container::LastIndexOf<T>(*this, what);
    }

    template <class T>
    constexpr bool SimpleList<T>::Remove(const T& what) noexcept
    {
        try
        {
            std::size_t pos = IndexOf(what);
            if (pos != (std::size_t)(-1))
            {
                RemoveAt(pos);
                return true;
            }
            else
                return false;
        }
        catch (...)
        {
            return false;
        }
    }

    template <class T>
    constexpr void SimpleList<T>::RemoveAt(std::size_t index)
    {
        if (index < _Count)
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                std::destroy_at(&_Elems[index]);
                std::construct_at(&_Elems[index]);
            }

            std::move(&_Elems[index + 1], &_Elems[_Count--], &_Elems[index]);
        }
        else
            throw std::out_of_range("where");
    }

    template <class T>
    constexpr void SimpleList<T>::RemoveRange(std::size_t index, std::size_t count)
    {
        if (index + count <= _Count)
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                std::destroy_n(&_Elems[index], count);
                UninitializedDefaultConstruct(&_Elems[index], count);
            }

            std::move(&_Elems[index + count], &_Elems[_Count], &_Elems[index]);
            _Count -= count;
        }
        else
            throw std::out_of_range("where");
    }


    template <class T>
    constexpr void SimpleList<T>::Resize(std::size_t n) noexcept
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            if (n < _Count)
            {
                std::destroy_n(&_Elems[_Count - n], n);
                UninitializedDefaultConstruct(&_Elems[_Count - n], n);
            }
        }

        _Reallocate(_Count = n);
    }

    template <class T>
    constexpr void SimpleList<T>::Reverse() noexcept
    {
        for (std::size_t i = 0; i < _Count / 2; i++)
            _Elems[i] = std::exchange(_Elems[_Count - i - 1], _Elems[i]);
    }

    template <class T>
    constexpr void SimpleList<T>::Sort(Comparison<T> compare)
    {
        Container::Sort(*this, compare);
    }

    // Template Member Functions

    template <class T>
    template <IterableObjectOf<T> _It>
    constexpr void SimpleList<T>::AddRange(const _It& what) noexcept
    {
        std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

        if (add_count > _Capacity - _Count)
            _Reallocate(_Count * 2 + add_count);

        std::copy(what.begin(), what.end(), &_Elems[_Count]);
        _Count += add_count;
    }

    template <class T>
    template <IterableObjectOf<T> _It>
    constexpr void SimpleList<T>::AddRange(_It&& what) noexcept
    {
        std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

        if (add_count > _Capacity - _Count)
            _Reallocate(_Count * 2 + add_count);

        std::move(what.begin(), what.end(), &_Elems[_Count]);
        _Count += add_count;
    }

    template <class T>
    template <class TOutput>
    constexpr List<TOutput> SimpleList<T>::ConvertAll(Converter<T, TOutput> converter) const
    {
        SimpleList<TOutput> out(_Count);
        for (std::size_t i = 0; i < _Count; i++)
            out[i] = converter(_Elems[i]);

        return out;
    }

    template <class T>
    template <IterableObjectOf<T> _It>
    constexpr void SimpleList<T>::CopyTo(_It& where) const noexcept
    {
        std::copy(_Elems, &_Elems[_Count], where.begin());
    }

    template <class T>
    template <IterableObjectOf<T> _It>
    constexpr void SimpleList<T>::InsertRange(std::size_t index, const _It& what) noexcept
    {
        if (index == _Count)
            this->AddRange(what);
        else if (index < _Count)
        {
            std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

            // If the number of items to be added exceeds the remaining space, allocate a new chunk of memory and move the items
            // The move is split into two parts for more efficiency: items before the place of insertion and those after
            if (add_count > _Capacity - _Count)
            {
                T* new_Elems = new T[_Count * 2 + add_count]();

                std::move(_Elems, &_Elems[index], new_Elems);
                std::move(&_Elems[index], &_Elems[_Count], &new_Elems[add_count]);

                delete[] _Elems;
                _Elems = new_Elems;
                _Capacity = _Count * 2 + add_count;
            }
            // Otherwise, shift items after the place of insertion to give space for items that are to be added
            else
                std::move_backward(&_Elems[index], &_Elems[_Count], &_Elems[_Count + add_count]);

            std::copy(what.begin(), what.end(), &_Elems[index]);
            _Count += add_count;
        }
        else
            throw std::out_of_range("index");
    }

    template <class T>
    template <IterableObjectOf<T> _It>
    constexpr void SimpleList<T>::InsertRange(std::size_t index, _It&& what)
    {
        if (index == _Count)
            this->AddRange(std::move(what));
        else if (index < _Count)
        {
            std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

            // If the number of items to be added exceeds the remaining space, allocate a new chunk of memory and move the items
            // The move is split into two parts for more efficiency: items before the place of insertion and those after
            if (add_count > _Capacity - _Count)
            {
                T* new_Elems = new T[_Count * 2 + add_count]{};

                std::move(_Elems, &_Elems[index], new_Elems);
                std::move(&_Elems[index], &_Elems[_Count], &new_Elems[add_count]);

                delete[] _Elems;
                _Elems = new_Elems;
                _Capacity = _Count * 2 + add_count;
            }
            // Otherwise, shift items after the place of insertion to give space for items that are to be added
            else
                std::move_backward(&_Elems[index], &_Elems[_Count], &_Elems[_Count + add_count]);

            std::move(what.begin(), what.end(), &_Elems[index]);
            _Count += add_count;
        }
        else
            throw std::out_of_range("index");
    }

    // Iterators

    template <class T>
    constexpr T* SimpleList<T>::begin() const noexcept
    {
        return _Elems;
    }

    template <class T>
    constexpr T* SimpleList<T>::end() const noexcept
    {
        return &_Elems[_Count];
    }

    template <class T>
    constexpr const T* SimpleList<T>::cbegin() const noexcept
    {
        return _Elems;
    }

    template <class T>
    constexpr const T* SimpleList<T>::cend() const noexcept
    {
        return &_Elems[_Count];
    }

    // Operators

    template <class T>
    constexpr SimpleList<T>& SimpleList<T>::operator=(const List<T>& other)
    {
        if (_Capacity < other._Count)
        {
            delete[] _Elems;
            _Elems = new T[_Capacity = other._Count]{};
        }
        else
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                std::destroy_n(_Elems, _Count);
                UninitializedDefaultConstruct(_Elems, _Count);
            }
        }

        std::copy(other._Elems, &other._Elems[_Count = other._Count], _Elems);
        return *this;
    }

    template <class T>
    constexpr SimpleList<T>& SimpleList<T>::operator=(SimpleList<T>&& other)
    {
        if (_Elems)
            delete[] _Elems;

        _Elems = std::exchange(other._Elems, nullptr);
        _Count = std::exchange(other._Count, 0);
        _Capacity = std::exchange(other._Capacity, 0);

        return *this;
    }

    template <class T>
    constexpr T& SimpleList<T>::operator[](std::size_t index)
    {
        if (index < _Count)
            return _Elems[index];
        else
            throw std::out_of_range("index");
    }

    template <class T>
    constexpr const T& SimpleList<T>::operator[](std::size_t index) const
    {
        if (index < _Count)
            return _Elems[index];
        else
            throw std::out_of_range("index");
    }

    template <class T>
    constexpr bool SimpleList<T>::operator==(const SimpleList<T>& other) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>
    {
        if (_Count != other._Count)
            return false;
        else
        {
            for (std::size_t i = 0; i < _Count; i++)
            {
                if (_Elems[i] != other._Elems[i])
                    return false;
            }
            return true;
        }
    }

    // Destructor

    template <class T>
    constexpr SimpleList<T>::~SimpleList() noexcept
    {
        delete[] _Elems;
    }

    // Protected member functions

    template <class T>
    constexpr void SimpleList<T>::_Reallocate(std::size_t new_capacity) noexcept
    {
        if (_Capacity == 0)
            _Elems = new T[new_capacity]{};
        else if (_Capacity != new_capacity)
        {
            T* new_Elems = new T[new_capacity]{};
            std::move(_Elems, &_Elems[_Count < new_capacity ? _Count : new_capacity], new_Elems);

            delete[] _Elems;
            _Elems = new_Elems;
        }

        _Capacity = new_capacity;
    }
#endif
};