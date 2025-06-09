#include "Window.h"
#include "../resource.h"

//------------------------------------------------------------------------------
//---- Window stuff ------------------------------------------------------------
//------------------------------------------------------------------------------

namespace pkmn
{
    std::optional<int> Window::process_messages() noexcept
    {
        MSG msg;

        // while queue has messages, remove and dispatch them (but don't block on empty queue)
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // check for quit because PeekMessage does not signal this via return value
            if (msg.message == WM_QUIT) {
                // return optional wrapping int (arg to PostQuitMessage is in wParam) signals quit
                return int(msg.wParam);
            }

            // TranslateMessage will post auxilliary WM_CHAR messages from key messages
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        // return empty optional when not quitting app
        return std::optional<int>();
    }

    Window::Window(int width, int height, const wchar_t* name) : m_width(width), m_height(height)
    {
        DWORD win_style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

        // calculate window size based on desired client region size
        RECT wr = { 0 };
        wr.left = 100;
        wr.right = wr.left + width;
        wr.top = 100;
        wr.bottom = wr.top + height;
        if (AdjustWindowRect(&wr, win_style, FALSE) == 0)
            throw LAST_HRWINERR();

        // create window & get hwnd
        m_hwnd = CreateWindowExW(
            0, WindowClass::get_name(),
            name, win_style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            wr.right - wr.left, wr.bottom - wr.top,
            nullptr, nullptr,
            WindowClass::get_instance(), this
        );

        // check for error
        if (m_hwnd == nullptr)
            throw LAST_HRWINERR();

        // show window (newly created windows start off as hidden)
        if (m_hwnd != nullptr)
            ShowWindow(m_hwnd, SW_SHOWDEFAULT);

        // create graphics object
        m_gfx = std::make_unique<gfx::Graphics>(m_hwnd, m_width, m_height);
    }

    void Window::set_title(const wchar_t* title) const
    {
        if (SetWindowTextW(m_hwnd, title) == 0)
            throw LAST_HRWINERR();
    }

    gfx::Graphics& Window::gfx()
    {
        if (!m_gfx)
            throw WINERR_NOGFX();
        return *m_gfx;
    }

    LRESULT WINAPI Window::handle_msg_setup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) noexcept
    {
        // use create parameter passed in from CreateWindowEx
        // to store window class pointer at WinAPI side
        if (msg == WM_NCCREATE)
        {
            // extract ptr to window class from creation data
            const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lp);
            Window* const wnd = static_cast<Window*>(create->lpCreateParams);
            // set WinAPI-managed user data to store ptr to window instance
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
            // set message proc to normal (non-setup) handler now that setup is finished
            SetWindowLongPtrW(hwnd, GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(&Window::handle_msg_thunk));
            // forward message to window instance handler
            return wnd->handle_msg(hwnd, msg, wp, lp);
        }
        return DefWindowProcW(hwnd, msg, wp, lp);
    }

    LRESULT WINAPI Window::handle_msg_thunk(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) noexcept
    {
        // retrieve ptr to window class
        Window* const wnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        // forward message to window class handler
        return wnd->handle_msg(hwnd, msg, wp, lp);
    }

    LRESULT Window::handle_msg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) noexcept
    {
        switch (msg)
        {
            // we don't want the DefWindowProcA to handle this message because
            // we want our destructor to destroy the window, so return 0 instead of break
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

            //----------------------- KEYBOARD MESSAGES ------------------------
        case WM_KEYDOWN:
            // syskey commands need to be handled to track ALT key (VK_MENU) and F10
        case WM_SYSKEYDOWN:
            // filter autorepeat
            if (!(lp & 0x40000000) || m_kbd.is_autorepeat_enabled())
                m_kbd.on_key_pressed(static_cast<unsigned char>(wp));
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            m_kbd.on_key_released(static_cast<unsigned char>(wp));
            break;
        case WM_CHAR:
            m_kbd.on_char(static_cast<char>(wp));
            break;
            //--------------------- END KEYBOARD MESSAGES ----------------------
        }

        return DefWindowProcW(hwnd, msg, wp, lp);
    }
}

//------------------------------------------------------------------------------
//---- Window class stuff ------------------------------------------------------
//------------------------------------------------------------------------------

namespace pkmn
{
    Window::WindowClass Window::WindowClass::s_wnd_class;

    const wchar_t* Window::WindowClass::get_name() noexcept
    {
        return k_wnd_class_name;
    }

    HINSTANCE Window::WindowClass::get_instance() noexcept
    {
        return s_wnd_class.m_hinst;
    }

    Window::WindowClass::WindowClass() noexcept : m_hinst(GetModuleHandleW(nullptr))
    {
        // register window class on WinAPI side
        WNDCLASSEXW wc = { };
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = handle_msg_setup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = get_instance();
        wc.hIcon = static_cast<HICON>(LoadImageW(
            get_instance(), MAKEINTRESOURCEW(IDI_APPICON),
            IMAGE_ICON, 32, 32, 0
        ));
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = get_name();
        wc.hIconSm = static_cast<HICON>(LoadImageW(
            get_instance(), MAKEINTRESOURCEW(IDI_APPICON),
            IMAGE_ICON, 16, 16, 0
        ));
        RegisterClassExW(&wc);
    }

    Window::WindowClass::~WindowClass() noexcept
    {
        UnregisterClassW(k_wnd_class_name, get_instance());
    }
}
