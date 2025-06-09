#include "Graphics.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace pkmn::gfx
{
	Graphics::Graphics(HWND hwnd, unsigned int width, unsigned int height)
	{
		DXGI_SWAP_CHAIN_DESC sd = { };
		sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.RefreshRate = { 0u, 0u }; // Numerator, Denominator
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc = { 1u, 0u }; // Count, Quality
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1u;
        sd.OutputWindow = hwnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0u;

        UINT swap_create_flags = 0u;
#ifndef NDEBUG
        swap_create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !NDEBUG

        // for checking results of d3d functions
        HRESULT hr;

        //create device, front / back buffers, swap chain and rendering context
        GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, swap_create_flags,
            nullptr, 0u,
            D3D11_SDK_VERSION,
            &sd, &m_swap, &m_device,
            nullptr, &m_context
        ));

        // gain access to texture subresource in swap chain (back buffer)
        wrl::ComPtr<ID3D11Resource> back_buffer;
        GFX_THROW_INFO(m_swap->GetBuffer(0u, __uuidof(ID3D11Resource), &back_buffer));
        GFX_THROW_INFO(m_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &m_target));

        // create and bind depth stencil state
        D3D11_DEPTH_STENCIL_DESC dsd = { };
        dsd.DepthEnable = TRUE;
        dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsd.DepthFunc = D3D11_COMPARISON_LESS;
        wrl::ComPtr<ID3D11DepthStencilState> dss;
        GFX_THROW_INFO(m_device->CreateDepthStencilState(&dsd, &dss));
        m_context->OMSetDepthStencilState(dss.Get(), 1u);

        // create depth stencil texture
        wrl::ComPtr<ID3D11Texture2D> depth_stencil;
        D3D11_TEXTURE2D_DESC dstd = { };
        dstd.Width = width;
        dstd.Height = height;
        dstd.MipLevels = 1u;
        dstd.ArraySize = 1u;
        dstd.Format = DXGI_FORMAT_D32_FLOAT;
        dstd.SampleDesc = { 1u, 0u };
        dstd.Usage = D3D11_USAGE_DEFAULT;
        dstd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        GFX_THROW_INFO(m_device->CreateTexture2D(&dstd, nullptr, &depth_stencil));

        // create view of depth stencil texture
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = { };
        dsvd.Format = DXGI_FORMAT_D32_FLOAT;
        dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvd.Texture2D.MipSlice = 0u;
        GFX_THROW_INFO(m_device->CreateDepthStencilView(depth_stencil.Get(), &dsvd, &m_dsv));

        // bind depth stencil view to Output Merger
        m_context->OMSetRenderTargets(1u, m_target.GetAddressOf(), m_dsv.Get());

        // configure viewport
        D3D11_VIEWPORT vp = { };
        vp.Width = (float)width;
        vp.Height = (float)height;
        vp.MinDepth = .0f;
        vp.MaxDepth = 1.f;
        vp.TopLeftX = .0f;
        vp.TopLeftY = .0f;
        m_context->RSSetViewports(1u, &vp);
	}

    dx::XMMATRIX Graphics::get_projection() const noexcept
    {
        return m_proj;
    }

    void Graphics::set_projection(dx::FXMMATRIX proj) noexcept
    {
        m_proj = proj;
    }

    dx::XMMATRIX Graphics::get_camera() const noexcept
    {
        return m_cam;
    }

    void Graphics::set_camera(dx::FXMMATRIX cam) noexcept
    {
        m_cam = cam;
    }

    void Graphics::begin_frame(float r, float g, float b) noexcept
    {
        // clear back and depth buffer
        const float color[] = { r, g, b, 1.f };
        m_context->ClearRenderTargetView(m_target.Get(), color);
        m_context->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
    }

    void Graphics::end_frame()
    {
        // swap back and front buffer
        HRESULT hr;
#ifndef NDEBUG
        m_info_manager.set();
#endif // !NDEBUG
        if (FAILED(hr = m_swap->Present(1u, 0u)))
        {
            if (hr == DXGI_ERROR_DEVICE_REMOVED)
                throw GFXERR_NODEVICE(m_device->GetDeviceRemovedReason());
            else
                throw GFXERR(hr);
        }
    }

    void Graphics::draw_indexed(UINT count) noexndbg
    {
        GFX_THROW_INFO_ONLY(m_context->DrawIndexed(count, 0u, 0u));
    }
}
