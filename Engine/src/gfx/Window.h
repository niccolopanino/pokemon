#pragma once
#include "../gfx/Graphics.h"
#include "../io/Keyboard.h"
#include <memory>
#include <optional>
#include <Windows.h>

namespace pkmn
{
    class Window
    {
    public:
        static std::optional<int> process_messages() noexcept;
    public:
        Window() = default;
        Window(int width, int height, const wchar_t* name);
        Window(const Window& wnd) = delete;
    public:
        Window& operator=(const Window& wnd) = delete;
    public:
        void set_title(const wchar_t* title) const;
        gfx::Graphics& gfx();
    public:
        Keyboard m_kbd;
    private:
        // singleton class to manage registration and cleanup of window class (WinAPI stuff)
        class WindowClass
        {
        public:
            static const wchar_t* get_name() noexcept;
            static HINSTANCE get_instance() noexcept;
        private:
            WindowClass() noexcept;
            WindowClass(const WindowClass& cls) = delete;
            ~WindowClass() noexcept;
        private:
            static constexpr const wchar_t* k_wnd_class_name = L"Pokemon Engine Main Window";
            static WindowClass s_wnd_class;
            HINSTANCE m_hinst;
        };
    private:
        static LRESULT WINAPI handle_msg_setup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) noexcept;
        static LRESULT WINAPI handle_msg_thunk(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) noexcept;
        LRESULT handle_msg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) noexcept;
    private:
        int m_width;
        int m_height;
        HWND m_hwnd;
        std::unique_ptr<gfx::Graphics> m_gfx;
    };
}
