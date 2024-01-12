#pragma once

#include "base_include.hpp"

namespace CQue
{
    /// @brief A naive, shallow wrapper class for referring to an iterable class whose iterators are convertible to or are themselves pointers.
    /// @tparam T Type of object(s) to be iterated upon
    template <class T>
    class IterWrapper
    {
    public:
        constexpr IterWrapper(T* first, T* last) noexcept : _First(first), _Last(last){}
        template <Iterable<T> _It>
        constexpr IterWrapper(const _It& iter) noexcept : _First((T*) iter.begin()), _Last((T*) iter.end()){}

        constexpr T* begin() const noexcept{ return _First; }
        constexpr T* end() const noexcept{ return _Last; }
    private:
        T* _First, *_Last;
    };

    /// @brief A contiguous, array-based collection equipped with indexing and some helper methods.
    // The member functions' names are unashamedly given due to .NET generic collection library.
    /// @tparam T 
    template <std::default_initializable T>
    class List
    {
    public:
        constexpr List() noexcept : _Capacity(0), _Size(0), _Elems(nullptr) {}
        constexpr List(const List<T>& other) noexcept(std::is_nothrow_copy_constructible_v<T>) : _Capacity(other._Capacity), _Size(other._Size), _Elems(new T[other._Capacity]) { std::copy(_Elems, &_Elems[other._Size], _Elems); }
        constexpr List(List<T>&& other) noexcept : _Capacity(std::exchange(other._Capacity, 0)), _Size(std::exchange(other._Size, 0)), _Elems(std::exchange(other._Elems, nullptr)) {}

        constexpr List(std::size_t initial_size) noexcept(std::is_nothrow_default_constructible_v<T>) : _Capacity(initial_size), _Size(initial_size), _Elems(new T[initial_size]) {}

        template <Iterable<T> _It>
        constexpr List(const _It& lst) noexcept(std::is_nothrow_copy_constructible_v<T>) : _Capacity(static_cast<std::size_t>(lst.end() - lst.begin())), _Size(static_cast<std::size_t>(lst.end() - lst.begin())), _Elems(new T[static_cast<std::size_t>(lst.end() - lst.begin())])
        {
            std::copy(lst.begin(), lst.end(), _Elems); 
        }

        template <Iterable<T> _It>
        constexpr List(_It&& lst) noexcept(std::is_nothrow_move_constructible_v<T>) : _Capacity(static_cast<std::size_t>(lst.end() - lst.begin())), _Size(static_cast<std::size_t>(lst.end() - lst.begin())), _Elems(new T[static_cast<std::size_t>(lst.end() - lst.begin())])
        {
            std::move(lst.begin(), lst.end(), _Elems); 
        }

        constexpr void Add(const T& what) noexcept(std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_move_assignable_v<T>) requires std::copyable<T>
        {
            if (_Size == _Capacity)
            {
                if (_Capacity == 0)
                    _Elems = new T[_Capacity = 1];
                else
                    _Reallocate(_Capacity * 2);
            }

            _Elems[_Size++] = what;
        }

        constexpr void Add(T&& what) noexcept(std::is_nothrow_move_assignable_v<T>) requires std::movable<T>
        {
            if (_Size == _Capacity)
            {
                if (_Capacity == 0)
                    _Elems = new T[_Capacity = 1];
                else
                    _Reallocate(_Capacity * 2);
            }

            _Elems[_Size++] = std::move(what);
        }

        template <Iterable<T> _It>
        constexpr void AddRange(const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_move_assignable_v<T>) requires std::copyable<T>
        {
            std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

            if(add_count > _Capacity - _Size)
                _Reallocate(_Size * 2 + add_count);

            std::copy(what.begin(), what.end(), &_Elems[_Size]);
            _Size += add_count;
        }

        template <Iterable<T> _It>
        constexpr void AddRange(_It&& what) noexcept(std::is_nothrow_move_assignable_v<T>) requires std::copyable<T>
        {
            std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

            if(add_count > _Capacity - _Size)
                _Reallocate(_Size * 2 + add_count);

            std::move(what.begin(), what.end(), &_Elems[_Size]);
            _Size += add_count;
        }

        constexpr std::size_t Capacity() const noexcept
        {
            return _Capacity;
        }

        constexpr void Clear()
        {
            std::destroy_n(_Elems, _Size);
            _Size = 0;
        }

        constexpr bool Contains(const T& what) const noexcept requires std::equality_comparable<T>
        {
            for(std::size_t i = 0; i < _Size; i++)
                if(_Elems[i] == what)
                    return true;

            return false;
        }

        template <class TOutput>
        constexpr List<TOutput> ConvertAll(Converter<T, TOutput> converter = &DefaultConvert<T, TOutput>) const
        {
            List<TOutput> out(_Size);
            for(std::size_t i = 0; i < _Size; i++)
                out[i] = converter(_Elems[i]);

            return out;
        }

        template <Iterable<T> _It>
        constexpr void CopyTo(_It& where) const
        {
            std::copy(_Elems, &_Elems[_Size], where.begin());
        }

        constexpr std::size_t Count() const noexcept
        {
            return _Size;
        }

        constexpr bool Exists(Predicate<const T&> match) const noexcept
        {
            for(std::size_t i = 0; i < _Size; i++)
                if(match(_Elems[i]))
                    return true;

            return false;
        }

        constexpr const T& Find(Predicate<const T&> match) const
        {
            for(std::size_t i = 0; i < _Size; i++)
                if(match(_Elems[i]))
                    return _Elems[i];

            return T();
        }

        constexpr List<T> FindAll(Predicate<const T&> match) const
        {
            List<T> res;
            for(std::size_t i = 0; i < _Size; i++)
                if(match(_Elems[i]))
                    res.Add(_Elems[i]);

            return res;
        }

        constexpr std::size_t FindIndex(Predicate<const T&> match) const
        {
            for(std::size_t i = 0; i < _Size; i++)
                if(match(_Elems[i]))
                    return i;

            return -1;
        }

        constexpr const T& FindLast(Predicate<const T&> match) const
        {
            for(std::size_t i = 0; i < _Size; i++)
                if(match(_Elems[_Size-i-1]))
                    return _Elems[_Size-i-1];

            return T();
        }

        constexpr std::size_t FindLastIndex(Predicate<const T&> match) const
        {
            for(std::size_t i = 0; i < _Size; i++)
                if(match(_Elems[_Size-i-1]))
                    return _Size-i-1;

            return -1;
        }

        constexpr std::size_t IndexOf(const T& what) const noexcept
        {
            for(std::size_t i = 0; i < _Size; i++)
                if(_Elems[i] == what)
                    return i;

            return -1;
        }
        
        constexpr void Insert(std::size_t index, const T& what)
        {
            if(index == _Size)
                this->Add(what);
            else if(index < _Size)
            {
                if(_Size == _Capacity)
                {
                    T* new_Elems = new T[_Capacity *= 2];
                    std::move(_Elems, &_Elems[index], new_Elems);
                    std::move(&_Elems[index], &_Elems[_Size], &new_Elems[index+1]);
                    delete[] _Elems;
                    _Elems = new_Elems;
                }
                else
                    std::move_backward(&_Elems[index], &_Elems[_Size], &_Elems[_Size+1]);

                _Elems[index] = what;
                _Size++;
            }
            
            else
                throw std::out_of_range("index");
        }

        constexpr void Insert(std::size_t index, T&& what)
        {
            if (index == _Size)
                this->Add(what);
            else if (index < _Size)
            {
                if (_Size == _Capacity)
                {
                    T* new_Elems = new T[_Capacity *= 2];
                    std::move(_Elems, &_Elems[index], new_Elems);
                    std::move(&_Elems[index], &_Elems[_Size], &new_Elems[index + 1]);
                    delete[] _Elems;
                    _Elems = new_Elems;
                }
                else
                    std::move_backward(&_Elems[index], &_Elems[_Size], &_Elems[_Size + 1]);

                _Elems[index] = std::move(what);
                _Size++;
            }

            else
                throw std::out_of_range("index");
        }

        template <Iterable<T> _It>
        constexpr void InsertRange(std::size_t index, const _It& what) noexcept(std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_move_assignable_v<T>)
        {
            if(index == _Size)
                this->AddRange(what);
            else if(index < _Size)
            {
                std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());

                // If the number of items to be added exceeds the remaining space, allocate a new chunk of memory and move the items
                // The move is split into two parts for more efficiency: items before the place of insertion and those after
                if(add_count > _Capacity - _Size)
                {
                    T* new_Elems = new T[_Capacity = _Size * 2 + add_count];
                    
                    std::move(_Elems, &_Elems[index], new_Elems);
                    std::move(&_Elems[index], &_Elems[_Size], &new_Elems[index+add_count]);
                    delete[] _Elems;
                    _Elems = new_Elems;
                }
                // Otherwise, shift items after the place of insertion to give space for items that are to be added
                else
                    std::move_backward(&_Elems[index], &_Elems[_Size], &_Elems[_Size + add_count]);
                
                std::copy(what.begin(), what.end(), &_Elems[index]);
                _Size += add_count;
            }
            else
                throw std::out_of_range("index");
        }

        template <Iterable<T> _It>
        constexpr void InsertRange(std::size_t index, _It&& what)
        {
            if (index == _Size)
                this->AddRange(std::move(what));
            else if(index < _Size)
            {
                std::size_t add_count = static_cast<std::size_t>(what.end() - what.begin());
                if(add_count > _Capacity - _Size)
                {
                    T* new_Elems = new T[_Capacity = _Size * 2 + add_count];

                    std::move(_Elems, &_Elems[index], new_Elems);
                    std::move(&_Elems[index], &_Elems[_Size], &new_Elems[index+add_count]);
                    delete[] _Elems;
                    _Elems = new_Elems;
                }
                else
                    std::move_backward(&_Elems[index], &_Elems[_Size], &_Elems[_Size+add_count]);
                
                std::move(what.begin(), what.end(), &_Elems[index]);
                _Size += add_count;
            }
            else
               throw std::out_of_range("index");
        }

        constexpr std::size_t LastIndexOf(const T& what) const noexcept
        {
            for(std::size_t i = 0; i < _Size; i++)
                if(_Elems[_Size-i-1] == what)
                    return _Size-i-1;

            return -1;
        }

        constexpr bool Remove(const T& what) noexcept
        {
            try
            {
                std::size_t pos = IndexOf(what);
                if(pos != (std::size_t)(-1))
                {
                    RemoveAt(pos);
                    return true;
                }
                else
                    return false;
            }
            catch(...)
            {
                return false;
            }
        }

        constexpr void RemoveAt(std::size_t index)
        {
            if (index < _Size)
            {
                std::destroy_at(&_Elems[index]);
                std::construct_at(&_Elems[index]);
                std::move(&_Elems[index + 1], &_Elems[_Size--], &_Elems[index]);
            }
            else
                throw std::out_of_range("where");
        }

        constexpr void RemoveRange(std::size_t index, std::size_t count)
        {
            if(index + count <= _Size)
            {
                std::destroy_n(&_Elems[index], count);
                for(std::size_t i = index; i < index + count; i++)
                    std::construct_at(&_Elems[i]);

                std::move(&_Elems[index+count], &_Elems[_Size], &_Elems[index]);
                _Size -= count;
            }
            else
                throw std::out_of_range("where");
        }

        constexpr void Resize(std::size_t n) noexcept
        {
            if(n > _Capacity)
                _Reallocate(n);
            
            if(n > _Size)
            {
                for(std::size_t i = _Size; i < n; i++)
                    std::construct_at(&_Elems[i]);
                
                _Size = n;
            }
            else if(n < _Size)
            {
                for(std::size_t i = 0; i < _Size - n; i++)
                {
                    std::destroy_at(&_Elems[n+i]);
                    std::construct_at(&_Elems[n+i]);
                }

                _Size = n;
            }
        }

        constexpr void Reverse() noexcept(std::is_nothrow_move_assignable_v<T>)
        {
            for(std::size_t i = 0; i < _Size/2; i++)
                _Elems[i] = std::exchange(_Elems[_Size-i-1], _Elems[i]);
        }

        constexpr void Sort(Comparison<T> compare = &DefaultCompare<T>)
        {
            _HeapSort(compare);
        }

        constexpr T* begin() const noexcept
        {
            return _Elems;
        }

        constexpr const T* cbegin() const noexcept
        {
            return _Elems;
        }

        constexpr T* end() const noexcept
        {
            return &_Elems[_Size];
        }

        constexpr const T* cend() const noexcept
        {
            return &_Elems[_Size];
        }

        constexpr List<T>& operator=(const List<T>& other)
        {
            if(other._Size > _Capacity)
                _Reallocate(other._Size);

            std::copy(other.cbegin(), other.cend(), _Elems);
            _Size = other._Size;

            return *this;
        }

        constexpr List<T>& operator=(List<T>&& other)
        {
            if(_Elems)
                delete[] _Elems;
            
            _Elems = std::exchange(other._Elems, nullptr);
            _Size = std::exchange(other._Size, 0);
            _Capacity = std::exchange(other._Capacity, 0);

            return *this;
        }

        constexpr T& operator[](std::size_t index)
        {
            return _Elems[index];
        }

        constexpr const T& operator[](std::size_t index) const
        {
            return _Elems[index];
        }

        constexpr bool operator==(const List<T>& other) const requires std::equality_comparable<T>
        {
            if(_Size != other._Size)
                return false;
            else
            {
                for(std::size_t i = 0; i < _Size; i++)
                {
                    if(_Elems[i] != other._Elems[i])
                        return false;
                }
                return true;
            }
        }

        constexpr ~List() noexcept(std::is_nothrow_destructible_v<T>)
        {
            delete[] _Elems;
        }

    protected:
        constexpr void _Reallocate(std::size_t new_capacity) noexcept(std::is_nothrow_move_assignable_v<T>)
        {
            if(_Capacity == 0)
                _Elems = new T[new_capacity];
            else if(_Capacity != new_capacity)
            {
                T* new_Elems = new T[new_capacity];
                std::move(_Elems, &_Elems[_Size], new_Elems);
                delete[] _Elems;
                _Elems = new_Elems;
            }
            _Capacity = new_capacity;
        }

        constexpr void _HeapSort(Comparison<T> compare = &DefaultCompare<T>)
        {
            std::size_t start = _Size/2;
            std::size_t end = _Size;

            while(end > 1)
            {
                if(start > 0) 
                    start--;
                else 
                {
                    end--;
                    _Elems[end] = std::exchange(_Elems[0], _Elems[end]);
                }

                std::size_t root = start;
                while(root * 2 + 1 < end)
                {
                    std::size_t child = root * 2 + 1;
                    if(child+1 < end && compare(_Elems[child], _Elems[child+1]) == std::partial_ordering::less)
                        child++;

                    if(_Elems[root] < _Elems[child])
                    {
                        _Elems[root] = std::exchange(_Elems[child], _Elems[root]);
                        root = child;
                    }
                    else
                        break;
                }
            }
        }

        std::size_t _Capacity;
        std::size_t _Size;
        T* _Elems;
    };
};