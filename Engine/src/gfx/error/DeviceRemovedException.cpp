#include "DeviceRemovedException.h"

namespace pkmn::gfx
{
    const wchar_t* DeviceRemovedException::get_type() const noexcept
    {
        return L"Pok�mon Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
    }
}
