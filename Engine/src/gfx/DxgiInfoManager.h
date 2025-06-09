#pragma once
#include <dxgidebug.h>
#include <string>
#include <vector>
#include <wrl.h>

namespace pkmn::gfx
{
    class DxgiInfoManager
    {
    public:
        DxgiInfoManager();
        DxgiInfoManager(const DxgiInfoManager& man) = delete;
        ~DxgiInfoManager() = default;
    public:
        DxgiInfoManager& operator=(const DxgiInfoManager& man) = delete;
    public:
        // set the index (m_next) so that the next call to get_messages
        // will only get errors generated after this call
        void set() noexcept;
        std::vector<std::wstring> get_messages() const;
    private:
        unsigned long long m_next = 0u;
        Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_dxgi_info_queue;
    };
}
