#include "NoGfxException.h"

namespace pkmn::gfx
{
	const wchar_t* NoGfxException::get_type() const noexcept
	{
		return L"Pokémon Window Exception [No Graphics]";
	}
}
