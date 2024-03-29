#pragma once

// ####################################### FORWARD DECLARATIONS #######################################

namespace CQue
{
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
        constexpr List(std::initializer_list<T> lst) noexcept(std::is_nothrow_copy_constructible_v<T>);

        template <ForwardIterableObjectOf<T> _It>
        constexpr List(const _It& lst) noexcept(std::is_nothrow_copy_constructible_v<T>);

        template <ForwardIterableObjectOf<T> _It>
        constexpr List(_It&& lst) noexcept(std::is_nothrow_move_constructible_v<T>);

        template <std::forward_iterator _It>
        constexpr List(_It first, _It last) noexcept(std::is_nothrow_copy_constructible_v<T>);

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

        template <ForwardIterableObjectOf<T> _It>
        constexpr void AddRange(const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>);

        template <ForwardIterableObjectOf<T> _It>
        constexpr void AddRange(_It&& what) noexcept(std::is_nothrow_move_assignable_v<T>);

        template <class TOutput>
        constexpr List<TOutput> ConvertAll(Converter<T, TOutput> converter = &DefaultConvert<T, TOutput>) const;

        template <ForwardIterableObjectOf<T> _It>
        constexpr void CopyTo(_It& where) const noexcept(std::is_nothrow_copy_assignable_v<T>);

        template <ForwardIterableObjectOf<T> _It>
        constexpr void InsertRange(std::size_t index, const _It& what);

        template <ForwardIterableObjectOf<T> _It>
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
};

// ######################################## BODY DECLARATIONS #########################################

namespace CQue
{
    // **************************************** List<T, Allocator> ****************************************

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
    constexpr List<T, Allocator>::List(std::initializer_list<T> lst) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = _Alloc.allocate(_Capacity);

        UninitializedCopy(lst.begin(), lst.end(), _Elems);
    }

    template <class T, class Allocator>
    template <ForwardIterableObjectOf<T> _It>
    constexpr List<T, Allocator>::List(const _It& lst) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = _Alloc.allocate(_Capacity);

        UninitializedCopy(lst.begin(), lst.end(), _Elems);
    }

    template <class T, class Allocator>
    template <ForwardIterableObjectOf<T> _It>
    constexpr List<T, Allocator>::List(_It&& lst) noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        _Capacity = _Count = static_cast<std::size_t>(lst.end() - lst.begin());
        _Elems = _Alloc.allocate(_Capacity);

        UninitializedMove(lst.begin(), lst.end(), _Elems);
    }

    template <class T, class Allocator>
    template <std::forward_iterator _It>
    constexpr List<T, Allocator>::List(_It first, _It last) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        _Capacity = _Count = static_cast<std::size_t>(last - first);
        _Elems = _Alloc.allocate(_Capacity);

        UninitializedCopy(first, last, _Elems);
    }

    // List<T, Allocator> - Non-Template Member Functions

    template <class T, class Allocator>
    constexpr void List<T, Allocator>::Add(const T& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>)
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
    template <ForwardIterableObjectOf<T> _It>
    constexpr void List<T, Allocator>::AddRange(const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>)
    {
        std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

        if (add_count > _Capacity - _Count)
            _Reallocate(_Count * 2 + add_count);

        UninitializedCopy(what.begin(), what.end(), &_Elems[_Count]);
        _Count += add_count;
    }

    template <class T, class Allocator>
    template <ForwardIterableObjectOf<T> _It>
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
    template <ForwardIterableObjectOf<T> _It>
    constexpr void List<T, Allocator>::CopyTo(_It& where) const noexcept(std::is_nothrow_copy_assignable_v<T>)
    {
        std::copy(_Elems, &_Elems[_Count], where.begin());
    }

    template <class T, class Allocator>
    template <ForwardIterableObjectOf<T> _It>
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
                UninitializedMove(&_Elems[index], &_Elems[_Count], &new_Elems[index + add_count]);
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
    template <ForwardIterableObjectOf<T> _It>
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
                UninitializedMove(&_Elems[index], &_Elems[_Count], &new_Elems[index + add_count]);
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
};