#pragma once
#include "../../error/BaseException.h"

namespace pkmn::gfx
{
	class NoGfxException : public BaseException
	{
	public:
		using BaseException::BaseException;
		virtual const wchar_t* get_type() const noexcept override;
	};
}
