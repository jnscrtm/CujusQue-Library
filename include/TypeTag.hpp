#pragma once

#include "base_include.hpp"

namespace CQue
{
	class TypeTag
	{
	public:
		TypeTag(const TypeTag&) = delete;

		std::uint64_t GetID() const noexcept;

		template <class T>
		friend constexpr const TypeTag& GetType() noexcept;

		friend class Any;

	private:
		constexpr TypeTag() noexcept = default;

		template <class T>
		struct _TagGenerator;
	};

	template <class T>
	struct TypeTag::_TagGenerator
	{
	public:
		static inline const TypeTag Tag = TypeTag();
	};

	constexpr bool operator==(const TypeTag& a, const TypeTag& b) noexcept
	{
		return (&a == &b);
	}

	template <class T>
	constexpr const TypeTag& GetType() noexcept
	{
		return TypeTag::_TagGenerator<T>::Tag;
	}
};