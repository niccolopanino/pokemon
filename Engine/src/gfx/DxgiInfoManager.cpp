#include "DxgiInfoManager.h"
#include "../util/Macros.h"
#include "../util/StringConv.h"
#include "../util/Windoof.h"
#include <memory>

#pragma comment(lib, "dxguid.lib")

namespace pkmn::gfx
{
    DxgiInfoManager::DxgiInfoManager()
    {
        // define function signature of DXGIGetDebugInterface
        typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

        // load the dll that contains the function DXGIGetDebugInterface
        const HMODULE hmod_dxgi_debug =
            LoadLibraryExW(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (hmod_dxgi_debug == nullptr)
            throw LAST_HRWINERR();

        // get address of DXGIGetDebugInterface in dll
        const auto dxgi_get_debug_interface = reinterpret_cast<DXGIGetDebugInterface>(
            reinterpret_cast<void*>(GetProcAddress(hmod_dxgi_debug, "DXGIGetDebugInterface")));
        if (dxgi_get_debug_interface == nullptr)
            throw LAST_HRWINERR();

        HRESULT hr;
        GFX_THROW_NOINFO(dxgi_get_debug_interface(__uuidof(IDXGIInfoQueue), &m_dxgi_info_queue));
    }

    void DxgiInfoManager::set() noexcept
    {
        m_next = m_dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    }

    std::vector<std::wstring> DxgiInfoManager::get_messages() const
    {
        std::vector<std::wstring> msgs;
        const unsigned long long end = m_dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);

        for (unsigned long long i = m_next; i < end; ++i) {
            HRESULT hr;
            SIZE_T msg_length;
            /// get the size of message i in bytes
            GFX_THROW_NOINFO(m_dxgi_info_queue->GetMessageW(DXGI_DEBUG_ALL, i, nullptr, &msg_length));
            // allocate memory for message
            std::unique_ptr<byte[]> bytes = std::make_unique<byte[]>(msg_length);
            DXGI_INFO_QUEUE_MESSAGE* msg = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
            // get the message and push its description into the vector
            GFX_THROW_NOINFO(m_dxgi_info_queue->GetMessageW(DXGI_DEBUG_ALL, i, msg, &msg_length));
            msgs.emplace_back(str_to_wstr(msg->pDescription));
        }

        return msgs;
    }
}
