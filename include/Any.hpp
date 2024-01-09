#pragma once

#include "TypeTag.hpp"

namespace CQue
{
	/// FORWARD DECLARATIONS

	class Any
	{
	public:
		constexpr Any() noexcept;
		constexpr Any(const Any& other);
		constexpr Any(Any&& other) noexcept;

		template <class T>
		constexpr Any(T&& val) noexcept(noexcept(std::decay_t<T>(std::forward<T>(val)))) requires(std::is_reference_v<T> ? std::is_copy_constructible_v<T> : std::is_move_constructible_v<T>);

		constexpr const TypeTag& CurrentType() const noexcept;
		constexpr bool IsEmpty() const noexcept;
		constexpr void Reset();
		
		template <NonRef T>
		constexpr const std::remove_const_t<T>& GetValue() const;

		template <NonRef T>
		constexpr T Release();

		constexpr Any& operator=(const Any& other);
		constexpr Any& operator=(Any&& other) noexcept;

		template <class T>
		constexpr Any& operator=(T&& val) noexcept(noexcept(std::decay_t<T>(std::forward<T>(val)))) requires(std::is_reference_v<T> ? std::is_copy_constructible_v<T> : std::is_move_constructible_v<T>);

		template <class T>
		constexpr operator T() const;

		constexpr ~Any();

	private:
		struct _ValueStorage
		{
			virtual constexpr void AssignCopyTo(_ValueStorage*) const = 0;
			virtual constexpr _ValueStorage* CreateCopy() const = 0;
			virtual constexpr const TypeTag& GetTypeTag() const noexcept = 0;

			virtual constexpr ~_ValueStorage() = default;
		};

		template <class T>
		struct _ValueStorageImpl : public _ValueStorage
		{
			constexpr _ValueStorageImpl() noexcept(std::is_nothrow_constructible_v<T>) requires std::is_default_constructible_v<T> = default;
			constexpr _ValueStorageImpl(const _ValueStorageImpl<T>&) noexcept(std::is_nothrow_copy_constructible_v<T>) requires std::is_copy_constructible_v<T> = default;
			constexpr _ValueStorageImpl(_ValueStorageImpl<T>&&) noexcept(std::is_nothrow_move_constructible_v<T>) requires std::is_move_constructible_v<T> = default;

			constexpr _ValueStorageImpl(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>) requires std::is_copy_constructible_v<T> : Value(val) {}
			constexpr _ValueStorageImpl(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>) requires std::is_move_constructible_v<T> : Value(std::move(val)) {}

			virtual constexpr void AssignCopyTo(_ValueStorage* other) const override
			{
				if (other->GetTypeTag() == this->GetTypeTag())
					static_cast<_ValueStorageImpl<T>*>(other)->Value = Value;
				else
					throw std::logic_error("Bruh");
			}

			virtual constexpr _ValueStorage* CreateCopy() const override
			{
				return new _ValueStorageImpl<T>(Value);
			}

			virtual constexpr const TypeTag& GetTypeTag() const noexcept override
			{
				return TypeTag::_TagGenerator<std::decay_t<T>>::Tag;
			}

			virtual constexpr ~_ValueStorageImpl() = default;

			T Value;
		};

		class EmptyObjectError : public std::logic_error
		{
		public:
			EmptyObjectError();
			EmptyObjectError(const EmptyObjectError& other) = default;
		};

		class BadCast : public std::logic_error
		{
		public:
			BadCast();
			BadCast(const BadCast& other) = default;
		};

		_ValueStorage* _ptrInstance;
	};

	// ####################################### BODY DECLARATIONS ##########################################

	// ********************************************* Any *********************************************

	constexpr Any::Any() noexcept : _ptrInstance(nullptr) {}
	constexpr Any::Any(const Any& other) : _ptrInstance(other._ptrInstance->CreateCopy()) {}
	constexpr Any::Any(Any&& other) noexcept : _ptrInstance(std::exchange(other._ptrInstance, nullptr)) {}

	template <class T>
	constexpr Any::Any(T&& val) noexcept(noexcept(std::decay_t<T>(std::forward<T>(val)))) requires(std::is_reference_v<T> ? std::is_copy_constructible_v<T> : std::is_move_constructible_v<T>) : _ptrInstance(new _ValueStorageImpl<std::decay_t<T>>(std::forward<T>(val))) {}

	constexpr const TypeTag& Any::CurrentType() const noexcept
	{
		if (_ptrInstance)
			return _ptrInstance->GetTypeTag();
		else
			return TypeTag::_TagGenerator<void>::Tag;
	}

	constexpr bool Any::IsEmpty() const noexcept
	{
		return (_ptrInstance == nullptr);
	}

	template <NonRef T>
	constexpr const std::remove_const_t<T>& Any::GetValue() const
	{
		return operator const std::remove_const_t<T>&();
	}

	template <NonRef T>
	constexpr T Any::Release()
	{
		if(_ptrInstance)
		{
			if (_ptrInstance->GetTypeTag() == TypeTag::_TagGenerator<std::decay_t<T>>::Tag)
			{
				T res = std::move(static_cast<_ValueStorageImpl<T>*>(_ptrInstance)->Value);
				delete _ptrInstance;
				_ptrInstance = nullptr;
				return res;
			}
			else
				throw BadCast();
		}
		else
			throw EmptyObjectError();
	}

	constexpr void Any::Reset()
	{
		if(_ptrInstance)
			delete _ptrInstance, _ptrInstance = nullptr;
	}

	constexpr Any& Any::operator=(const Any& other)
	{
		// The null check is done because there is a need of dereferencing
		if (_ptrInstance)
		{
			if (_ptrInstance->GetTypeTag() == other._ptrInstance->GetTypeTag())
				other._ptrInstance->AssignCopyTo(_ptrInstance);
			else
				delete _ptrInstance, _ptrInstance = other._ptrInstance->CreateCopy();
		}
		else
			_ptrInstance = other._ptrInstance->CreateCopy();

		return *this;
	}

	constexpr Any& Any::operator=(Any&& other) noexcept
	{
		delete _ptrInstance;
		_ptrInstance = std::exchange(other._ptrInstance, nullptr);

		return *this;
	}

	template <class T>
	constexpr Any& Any::operator=(T&& val) noexcept(noexcept(std::decay_t<T>(std::forward<T>(val)))) requires(std::is_reference_v<T> ? std::is_copy_constructible_v<T> : std::is_move_constructible_v<T>)
	{
		// The null check is done because there is a need of dereferencing
		if (_ptrInstance)
		{
			// If the type is still the same, reuse the instance. Otherwise, destroy the previous instance and create a new one.
			if (_ptrInstance->GetTypeTag() == TypeTag::_TagGenerator<std::decay_t<T>>::Tag)
				static_cast<_ValueStorageImpl<std::decay_t<T>>*>(_ptrInstance)->Value = std::forward<T>(val);
			else
				delete _ptrInstance, _ptrInstance = new _ValueStorageImpl<std::decay_t<T>>(std::forward<T>(val));
		}
		else
			_ptrInstance = new _ValueStorageImpl<std::decay_t<T>>(std::forward<T>(val));

		return *this;
	}

	template <class T>
	constexpr Any::operator T() const
	{
		if (_ptrInstance)
		{
			if (_ptrInstance->GetTypeTag() == TypeTag::_TagGenerator<std::decay_t<T>>::Tag)
				return static_cast<_ValueStorageImpl<std::decay_t<T>>*>(_ptrInstance)->Value;
			else
				throw BadCast();
		}
		else
			throw EmptyObjectError();
	}

	constexpr Any::~Any()
	{
		delete _ptrInstance;
	}
};