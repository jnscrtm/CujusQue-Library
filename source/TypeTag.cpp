#include "TypeTag.hpp"

namespace CQue
{
	std::uint64_t TypeTag::GetID() const noexcept
	{
		return (std::uint64_t) this;
	}
};