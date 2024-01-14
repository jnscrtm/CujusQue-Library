#pragma once

#include "base_include.hpp"

namespace CQue
{
    // ####################################### FORWARD DECLARATIONS #######################################

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

    /// @brief A naive, shallow wrapper class for referring to an iterable class whose iterators are convertible to or are themselves pointers.
    /// @tparam T Type of object(s) to be iterated upon
    template <class T>
    class IterWrapper
    {
    public:
        // Constructor

        constexpr IterWrapper(T* first, T* last) noexcept;

        template <Iterable<T> _It>
        constexpr IterWrapper(const _It& iter) noexcept;

        // Iterators

        constexpr const T* begin() const noexcept;
        constexpr const T* end() const noexcept;
    private:
        T* _First, *_Last;
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
    template <class T>
    class List
    {
    public:
        using Allocator = std::allocator<T>;

        // Constructors

        constexpr List() noexcept;
        constexpr List(const List<T>& other) noexcept(std::is_nothrow_copy_constructible_v<T>);
        constexpr List(List<T>&& other) noexcept;

        constexpr List(std::size_t initial_size) noexcept(std::is_nothrow_default_constructible_v<T>) requires std::default_initializable<T>;

        template <Iterable<T> _It>
        constexpr List(const _It& lst) noexcept(std::is_nothrow_copy_constructible_v<T>);

        template <Iterable<T> _It>
        constexpr List(_It&& lst) noexcept(std::is_nothrow_move_constructible_v<T>);

        // Non-Template Member Functions

        constexpr void Add(const T& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>);
        constexpr void Add(T&& what) noexcept(std::is_nothrow_move_assignable_v<T>);
        constexpr std::size_t Capacity() const noexcept;
        constexpr void Clear() noexcept(std::is_nothrow_destructible_v<T>);
        constexpr bool Contains(const T& what) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>;
        constexpr std::size_t Count() const noexcept;
        constexpr bool Exists(Predicate<const T&> match) const;
        constexpr const T& Find(Predicate<const T&> match) const;
        constexpr List<T> FindAll(Predicate<const T&> match) const;
        constexpr std::size_t FindIndex(Predicate<const T&> match) const;
        constexpr const T& FindLast(Predicate<const T&> match) const;
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

        template <Iterable<T> _It>
        constexpr void AddRange(const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>);

        template <Iterable<T> _It>
        constexpr void AddRange(_It&& what) noexcept(std::is_nothrow_move_assignable_v<T>);

        template <class TOutput>
        constexpr List<TOutput> ConvertAll(Converter<T, TOutput> converter = &DefaultConvert<T, TOutput>) const;

        template <Iterable<T> _It>
        constexpr void CopyTo(_It& where) const noexcept(std::is_nothrow_copy_assignable_v<T>);

        template <Iterable<T> _It>
        constexpr void InsertRange(std::size_t index, const _It& what);

        template <Iterable<T> _It>
        constexpr void InsertRange(std::size_t index, _It&& what);

        // Iterators

        constexpr T* begin() const noexcept;
        constexpr T* end() const noexcept;
        constexpr const T* cbegin() const noexcept;
        constexpr const T* cend() const noexcept;
        
        // Operators

        constexpr List<T>& operator=(const List<T>& other);
        constexpr List<T>& operator=(List<T>&& other);

        constexpr T& operator[](std::size_t index);
        constexpr const T& operator[](std::size_t index) const;
        constexpr bool operator==(const List<T>& other) const requires std::equality_comparable<T>;

        // Destructor

        constexpr ~List() noexcept(std::is_nothrow_destructible_v<T>);

    protected:
        constexpr void _Reallocate(std::size_t new_capacity) noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_destructible_v<T>);
        constexpr void _Release() noexcept(std::is_nothrow_destructible_v<T>);
        constexpr void _HeapSort(Comparison<T> compare = &DefaultCompare<T>);

        std::size_t _Capacity;
        std::size_t _Count;
        T* _Elems;

        Allocator _Alloc{};
    };

    
    /// @brief A specialization of `List<T>` for types that are 
    /// @tparam T 
    template <std::default_initializable T>
    class List
    {
    public:
        // Constructors

        constexpr List() noexcept;
        constexpr List(const List<T>& other) noexcept(std::is_nothrow_copy_constructible_v<T>);
        constexpr List(List<T>&& other) noexcept;

        constexpr List(std::size_t initial_size) noexcept(std::is_nothrow_default_constructible_v<T>) requires std::default_initializable<T>;

        template <Iterable<T> _It>
        constexpr List(const _It& lst) noexcept(std::is_nothrow_copy_constructible_v<T>);

        template <Iterable<T> _It>
        constexpr List(_It&& lst) noexcept(std::is_nothrow_move_constructible_v<T>);

        // Non-Template Member Functions

        constexpr void Add(const T& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>);
        constexpr void Add(T&& what) noexcept(std::is_nothrow_move_assignable_v<T>);
        constexpr std::size_t Capacity() const noexcept;
        constexpr void Clear() noexcept(std::is_nothrow_destructible_v<T>);
        constexpr bool Contains(const T& what) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>;
        constexpr std::size_t Count() const noexcept;
        constexpr bool Exists(Predicate<const T&> match) const;
        constexpr const T& Find(Predicate<const T&> match) const;
        constexpr List<T> FindAll(Predicate<const T&> match) const;
        constexpr std::size_t FindIndex(Predicate<const T&> match) const;
        constexpr const T& FindLast(Predicate<const T&> match) const;
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

        template <Iterable<T> _It>
        constexpr void AddRange(const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>);

        template <Iterable<T> _It>
        constexpr void AddRange(_It&& what) noexcept(std::is_nothrow_move_assignable_v<T>);

        template <class TOutput>
        constexpr List<TOutput> ConvertAll(Converter<T, TOutput> converter = &DefaultConvert<T, TOutput>) const;

        template <Iterable<T> _It>
        constexpr void CopyTo(_It& where) const noexcept(std::is_nothrow_copy_assignable_v<T>);

        template <Iterable<T> _It>
        constexpr void InsertRange(std::size_t index, const _It& what);

        template <Iterable<T> _It>
        constexpr void InsertRange(std::size_t index, _It&& what);

        // Iterators

        constexpr T* begin() const noexcept;
        constexpr T* end() const noexcept;
        constexpr const T* cbegin() const noexcept;
        constexpr const T* cend() const noexcept;

        // Operators

        constexpr List<T>& operator=(const List<T>& other);
        constexpr List<T>& operator=(List<T>&& other);

        constexpr T& operator[](std::size_t index);
        constexpr const T& operator[](std::size_t index) const;
        constexpr bool operator==(const List<T>& other) const requires std::equality_comparable<T>;

        // Destructor

        constexpr ~List() noexcept(std::is_nothrow_destructible_v<T>);

    protected:
        constexpr void _Reallocate(std::size_t new_capacity) noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_destructible_v<T>);
        constexpr void _Release() noexcept(std::is_nothrow_destructible_v<T>);
        constexpr void _HeapSort(Comparison<T> compare = &DefaultCompare<T>);

        std::size_t _Capacity;
        std::size_t _Count;
        T* _Elems;

        Allocator _Alloc{};
    };
    */

    // ######################################## BODY DECLARATIONS #########################################

    // ****************************************** IterWrapper<T> ******************************************

    // IterWrapper<T> - Constructors

    template <class T>
    constexpr IterWrapper<T>::IterWrapper(T* first, T* last) noexcept : _First(first), _Last(last) {}

    template <class T> 
    template <Iterable<T> _It>
    constexpr IterWrapper<T>::IterWrapper(const _It& iter) noexcept : _First((T*)iter.begin()), _Last((T*)iter.end()) {}

    // IterWrapper<T> - Iterators

    template <class T>
    constexpr const T* IterWrapper<T>::begin() const noexcept { return _First; }

    template <class T>
    constexpr const T* IterWrapper<T>::end() const noexcept { return _Last; }



    // ********************************************* List<T> **********************************************

    // List<T> - Constructors

    template <class T>
    constexpr List<T>::List() noexcept : _Capacity(0), _Count(0), _Elems(nullptr) {}

    template <class T>
    constexpr List<T>::List(const List<T>& other) noexcept(std::is_nothrow_copy_constructible_v<T>) : _Capacity(other._Count), _Count(other._Count), _Elems(other._Elems ? _Alloc.allocate(other._Count) : nullptr) 
    { 
        for (std::size_t i = 0; i < other._Count; i++)
            std::construct_at(&_Elems[i], other._Elems[i]);
    }

    template <class T>
    constexpr List<T>::List(List<T>&& other) noexcept : _Capacity(std::exchange(other._Capacity, 0)), _Count(std::exchange(other._Count, 0)), _Elems(std::exchange(other._Elems, nullptr)) {}

    template <class T>
    constexpr List<T>::List(std::size_t initial_size) noexcept(std::is_nothrow_default_constructible_v<T>) requires std::default_initializable<T> : _Capacity(initial_size), _Count(initial_size), _Elems(_Alloc.allocate(initial_size))
    {
        UninitializedDefaultConstruct(_Elems, initial_size);
    }

    template <class T>
    template <Iterable<T> _It>
    constexpr List<T>::List(const _It& lst) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = _Alloc.allocate(_Capacity);

        UninitializedCopy(lst.begin(), lst.end(), _Elems);
    }

    template <class T> 
    template <Iterable<T> _It>
    constexpr List<T>::List(_It&& lst) noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = _Alloc.allocate(_Capacity);

        UninitializedMove(lst.begin(), lst.end(), _Elems);
    }

    // List<T> - Non-Template Member Functions

    template <class T>
    constexpr void List<T>::Add(const T& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>)
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

    template <class T>
    constexpr void List<T>::Add(T&& what) noexcept(std::is_nothrow_move_assignable_v<T>)
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

    template <class T>
    constexpr std::size_t List<T>::Capacity() const noexcept
    {
        return _Capacity;
    }

    template <class T>
    constexpr void List<T>::Clear() noexcept(std::is_nothrow_destructible_v<T>)
    {
        std::destroy_n(_Elems, _Count);
        _Count = 0;
    }

    template <class T>
    constexpr bool List<T>::Contains(const T& what) const noexcept(std::declval<T>() == std::declval<T>()) requires std::equality_comparable<T>
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (_Elems[i] == what)
                return true;

        return false;
    }

    template <class T>
    constexpr std::size_t List<T>::Count() const noexcept
    {
        return _Count;
    }

    template <class T>
    constexpr bool List<T>::Exists(Predicate<const T&> match) const
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (match(_Elems[i]))
                return true;

        return false;
    }

    template <class T>
    constexpr const T& List<T>::Find(Predicate<const T&> match) const
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (match(_Elems[i]))
                return _Elems[i];

        return T();
    }

    template <class T>
    constexpr List<T> List<T>::FindAll(Predicate<const T&> match) const
    {
        List<T> res;
        for (std::size_t i = 0; i < _Count; i++)
            if (match(_Elems[i]))
                res.Add(_Elems[i]);

        return res;
    }

    template <class T>
    constexpr std::size_t List<T>::FindIndex(Predicate<const T&> match) const
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (match(_Elems[i]))
                return i;

        return (std::size_t)(-1);
    }

    template <class T>
    constexpr const T& List<T>::FindLast(Predicate<const T&> match) const
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (match(_Elems[_Count - i - 1]))
                return _Elems[_Count - i - 1];

        return T();
    }

    template <class T>
    constexpr std::size_t List<T>::FindLastIndex(Predicate<const T&> match) const
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (match(_Elems[_Count - i - 1]))
                return _Count - i - 1;

        return (std::size_t)(-1);
    }

    template <class T>
    constexpr std::size_t List<T>::IndexOf(const T& what) const noexcept
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (_Elems[i] == what)
                return i;

        return -1;
    }

    template <class T>
    constexpr void List<T>::Insert(std::size_t index, const T& what)
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

    template <class T>
    constexpr void List<T>::Insert(std::size_t index, T&& what)
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

    template <class T>
    constexpr std::size_t List<T>::LastIndexOf(const T& what) const noexcept
    {
        for (std::size_t i = 0; i < _Count; i++)
            if (_Elems[_Count - i - 1] == what)
                return _Count - i - 1;

        return -1;
    }

    template <class T>
    constexpr bool List<T>::Remove(const T& what) noexcept
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
    constexpr void List<T>::RemoveAt(std::size_t index)
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

    template <class T>
    constexpr void List<T>::RemoveRange(std::size_t index, std::size_t count)
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

    template <class T>
    constexpr void List<T>::Resize(std::size_t n) noexcept requires std::default_initializable<T>
    { 
        if (n < _Count)
            std::destroy_n(&_Elems[_Count - n], n);
            
        _Reallocate(_Count = n);
    }

    template <class T>
    constexpr void List<T>::Reverse() noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        for (std::size_t i = 0; i < _Count / 2; i++)
            _Elems[i] = std::exchange(_Elems[_Count - i - 1], _Elems[i]);
    }

    template <class T>
    constexpr void List<T>::Sort(Comparison<T> compare)
    {
        _HeapSort(compare);
    }

    // List<T> - Template Member Functions

    template <class T> 
    template <Iterable<T> _It>
    constexpr void List<T>::AddRange(const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>)
    {
        std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

        if (add_count > _Capacity - _Count)
            _Reallocate(_Count * 2 + add_count);
        
        UninitializedCopy(what.begin(), what.end(), &_Elems[_Count]);
        _Count += add_count;
    }

    template <class T> 
    template <Iterable<T> _It>
    constexpr void List<T>::AddRange(_It&& what) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

        if (add_count > _Capacity - _Count)
            _Reallocate(_Count * 2 + add_count);

        UninitializedMove(what.begin(), what.end(), &_Elems[_Count]);
        _Count += add_count;
    }

    template <class T> 
    template <class TOutput>
    constexpr List<TOutput> List<T>::ConvertAll(Converter<T, TOutput> converter) const
    {
        List<TOutput> out(_Count);
        for (std::size_t i = 0; i < _Count; i++)
            out[i] = converter(_Elems[i]);

        return out;
    }

    template <class T> 
    template <Iterable<T> _It>
    constexpr void List<T>::CopyTo(_It& where) const noexcept(std::is_nothrow_copy_assignable_v<T>)
    {
        std::copy(_Elems, &_Elems[_Count], where.begin());
    }

    template <class T> 
    template <Iterable<T> _It>
    constexpr void List<T>::InsertRange(std::size_t index, const _It& what)
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

    template <class T> 
    template <Iterable<T> _It>
    constexpr void List<T>::InsertRange(std::size_t index, _It&& what)
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

    // List<T> - Iterators

    template <class T>
    constexpr T* List<T>::begin() const noexcept
    {
        return _Elems;
    }

    template <class T>
    constexpr T* List<T>::end() const noexcept
    {
        return &_Elems[_Count];
    }

    template <class T>
    constexpr const T* List<T>::cbegin() const noexcept
    {
        return _Elems;
    }

    template <class T>
    constexpr const T* List<T>::cend() const noexcept
    {
        return &_Elems[_Count];
    }

    template <class T>
    constexpr List<T>& List<T>::operator=(const List<T>& other)
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

    // List<T> - Operators

    template <class T>
    constexpr List<T>& List<T>::operator=(List<T>&& other)
    {
        if (_Elems)
            _Release();

        _Elems = std::exchange(other._Elems, nullptr);
        _Count = std::exchange(other._Count, 0);
        _Capacity = std::exchange(other._Capacity, 0);

        return *this;
    }

    template <class T>
    constexpr T& List<T>::operator[](std::size_t index)
    {
        if (index < _Count)
            return _Elems[index];
        else
            throw std::out_of_range("index");
    }

    template <class T>
    constexpr const T& List<T>::operator[](std::size_t index) const
    {
        if (index < _Count)
            return _Elems[index];
        else
            throw std::out_of_range("index");
    }

    template <class T>
    constexpr bool List<T>::operator==(const List<T>& other) const requires std::equality_comparable<T>
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

    // List<T> - Destructor

    template <class T>
    constexpr List<T>::~List() noexcept(std::is_nothrow_destructible_v<T>)
    {
        _Release();
    }

    // List<T> - Protected Member Functions

    template <class T>
    constexpr void List<T>::_Reallocate(std::size_t new_capacity) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>)
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

    template <class T>
    constexpr void List<T>::_Release() noexcept(std::is_nothrow_destructible_v<T>)
    {
        std::destroy_n(_Elems, _Count);
        _Alloc.deallocate(_Elems, _Capacity);
    }

    template <class T>
    constexpr void List<T>::_HeapSort(Comparison<T> compare)
    {
        std::size_t start = _Count / 2;
        std::size_t end = _Count;

        while (end > 1)
        {
            if (start > 0)
                start--;
            else
            {
                end--;
                _Elems[end] = std::exchange(_Elems[0], _Elems[end]);
            }

            std::size_t root = start;
            while (root * 2 + 1 < end)
            {
                std::size_t child = root * 2 + 1;
                if (child + 1 < end && compare(_Elems[child], _Elems[child + 1]) == std::partial_ordering::less)
                    child++;

                if (_Elems[root] < _Elems[child])
                {
                    _Elems[root] = std::exchange(_Elems[child], _Elems[root]);
                    root = child;
                }
                else
                    break;
            }
        }
    }
};