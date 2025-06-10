#pragma once
#include "../util/Macros.h"
#include "../util/Wintard.h"
#ifndef NDEBUG
#include "DxgiInfoManager.h"
#endif // !NDEBUG
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <dxgi.h>
#include <wrl/client.h>

namespace pkmn::gfx
{
    namespace bind
    {
        class Bindable;
    }

    class Graphics
    {
        friend class bind::Bindable;
    public:
        Graphics(HWND hwnd, unsigned int width, unsigned int height);
        Graphics(const Graphics& gfx) = delete;
        ~Graphics() = default;
    public:
        Graphics& operator=(const Graphics& gfx) = delete;
    public:
        DirectX::XMMATRIX get_projection() const noexcept;
        void set_projection(DirectX::FXMMATRIX proj) noexcept;
        DirectX::XMMATRIX get_camera() const noexcept;
        void set_camera(DirectX::FXMMATRIX cam) noexcept;
        void begin_frame(float r, float g, float b) noexcept;
        void end_frame();
        void draw_indexed(UINT count) noexndbg;
    private:
        DirectX::XMMATRIX m_proj = DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX m_cam = DirectX::XMMatrixIdentity();
#ifndef NDEBUG
        DxgiInfoManager m_info_manager;
#endif // !NDEBUG
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_target;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
    };
}
