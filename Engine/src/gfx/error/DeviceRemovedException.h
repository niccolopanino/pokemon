#pragma once
#include "HResGfxException.h"

namespace pkmn::gfx
{
	class DeviceRemovedException : public HResGfxException
	{
		using HResGfxException::HResGfxException;
	public:
		virtual const wchar_t* get_type() const noexcept override;
	};
}
