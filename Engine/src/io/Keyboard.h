#pragma once
#include <bitset>
#include <optional>
#include <queue>

namespace pkmn
{
    class Keyboard
    {
        friend class Window;
    public:
        class Event
        {
        public:
            enum class Type
            {
                e_press,
                e_release
            };
        public:
            Event(Type type, unsigned char code) noexcept;
            bool is_press() const noexcept;
            bool is_release() const noexcept;
            unsigned char get_code() const noexcept;
        private:
            Type m_type;
            unsigned char m_code;
        };
    public:
        Keyboard() = default;
        Keyboard(const Keyboard& kbd) = delete;
    public:
        Keyboard& operator=(const Keyboard& kbd) = delete;
    public:
        std::optional<Event> read_key() noexcept;
        std::optional<char> read_char() noexcept;
        bool is_key_empty() const noexcept;
        bool is_char_empty() const noexcept;
        bool is_key_pressed(unsigned char keycode) const noexcept;
        void flush() noexcept;
        void flush_key() noexcept;
        void flush_char() noexcept;
        void enable_autorepeat() noexcept;
        void disable_autorepeat() noexcept;
        bool is_autorepeat_enabled() const noexcept;
    private:
        template<typename T>
        static void trim_buffer(std::queue<T>& buffer) noexcept;
    private:
        void on_key_pressed(unsigned char keycode) noexcept;
        void on_key_released(unsigned char keycode) noexcept;
        void on_char(char mander) noexcept;
        void clear_state() noexcept;
    private:
        static constexpr unsigned int k_num_keys = 256u;
        static constexpr unsigned int k_buffer_size = 16u;
    private:
        bool m_autorepeat_enabled = false;
        std::bitset<k_num_keys> m_keystates;
        std::queue<Event> m_keybuffer;
        std::queue<char> m_charbuffer;
    };

    template<typename T>
    inline void Keyboard::trim_buffer(std::queue<T>& buffer) noexcept
    {
        while (buffer.size() > k_buffer_size)
            buffer.pop();
    }
}

