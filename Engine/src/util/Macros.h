#pragma once
#include "../gfx/error/HResWinException.h"
#include "../gfx/error/NoGfxException.h"
#include "../gfx/error/HResGfxException.h"
#include "../gfx/error/DeviceRemovedException.h"
#include "../gfx/error/GfxInfoException.h"
//#include "../gfx/error/SurfaceException.h"
//#include "../error/InvalidArgumentException.h"
#include "../gfx/DxgiInfoManager.h"
#include "Windoof.h"
#include <string>
#include <vector>

// conditional noexcept

#define noexndbg noexcept(!IS_DEBUG)

// Windows exceptions

#define HRWINERR(hr) pkmn::gfx::HResWinException(__LINE__, __FILEW__, hr)
#define LAST_HRWINERR() pkmn::gfx::HResWinException(__LINE__, __FILEW__, GetLastError())
#define WINERR_NOGFX() pkmn::gfx::NoGfxException(__LINE__, __FILEW__)

// Graphics exceptions
// HRESULT hr should be declared in the local scope for these macros to work

#define GFXERR_NOINFO(hr) pkmn::gfx::HResGfxException(__LINE__, __FILEW__, hr, std::vector<std::wstring>())
#define GFX_THROW_NOINFO(hrcall) if (FAILED(hr = hrcall)) throw GFXERR_NOINFO(hr)

#ifndef NDEBUG
#define GFXERR(hr) pkmn::gfx::HResGfxException(__LINE__, __FILEW__, hr, m_info_manager.get_messages())
#define GFX_THROW_INFO(hrcall) m_info_manager.set(); if (FAILED(hr = hrcall)) throw GFXERR(hr)
#define GFXERR_NODEVICE(hr) \
    pkmn::gfx::DeviceRemovedException(__LINE__, __FILEW__, hr, m_info_manager.get_messages())
#define GFX_THROW_INFO_ONLY(call) \
    m_info_manager.set(); \
    call; \
    { \
        auto v = m_info_manager.get_messages(); \
        if (!v.empty()) \
            throw pkmn::gfx::GfxInfoException(__LINE__, __FILEW__, v); \
    }
#else
#define GFXERR(hr) pkmn::gfx::HResGfxException(__LINE__, __FILEW__, hr)
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFXERR_NODEVICE(hr) pkmn::gfx::DeviceRemovedException(__LINE__, __FILEW__, hr)
#define GFX_THROW_INFO_ONLY(call) call
#endif // !NDEBUG

// macro for importing infomanager into local scope
// this->get_info_manager(Graphics &gfx) must exist
#ifndef NDEBUG
#define INFOMAN(gfx) HRESULT hr; pkmn::gfx::DxgiInfoManager &m_info_manager = get_info_manager(gfx)
#else
#define INFOMAN(gfx) HRESULT hr
#endif // !NDEBUG

// Surface exceptions

#define SURFERR(note) pkmn::gfx::SurfaceException(__LINE__, __FILEW__, note)
#define SURFERR_FILE(filename, note) \
    pkmn::gfx::SurfaceException(__LINE__, __FILEW__, filename, note)
#define HRSURFERR(note, hr) pkmn::gfx::SurfaceException(__LINE__, __FILEW__, note, hr)
#define HRSURFERR_FILE(filename, note, hr) \
    pkmn::gfx::SurfaceException(__LINE__, __FILEW__, filename, note, hr)

// HRESULT hr should be declared in the local scope for these macros to work

#define HRSURFERR_THROW(hrcall, note) \
    if (FAILED(hr = hrcall)) \
        throw HRSURFERR(note, hr)
#define HRSURFERR_THROW_FILE(hrcall, filename, note) \
    if (FAILED(hr = hrcall)) \
        throw HRSURFERR_FILE(filename, note, hr)

#define INVALIDARG(note) pkmn::InvalidArgumentException(__LINE__, __FILEW__, note)
#define INVALIDARGNAME(note, argname) pkmn::InvalidArgumentException(__LINE__, __FILEW__, note, argname)
