#include "Any.hpp"

namespace CQue
{
	Any::EmptyObjectError::EmptyObjectError() : std::logic_error("Attempted to get the value of an empty object.") {}
	Any::BadCast::BadCast() : std::logic_error("Attempted to convert a value to an incompatible or unpermitted target type.") {}
};