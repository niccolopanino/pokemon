#include "NoGfxException.h"

namespace pkmn::gfx
{
	const wchar_t* NoGfxException::get_type() const noexcept
	{
		return L"Pok�mon Window Exception [No Graphics]";
	}
}
