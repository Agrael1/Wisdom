#pragma once
#include <queue>
#include <optional>

namespace ex {
class Mouse
{
public:
    struct RawDelta {
        int x, y;
    };
    class Event
    {
    public:
        enum class Type {
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelUp,
            WheelDown,
            Move,
            Enter,
            Leave,
        };

    private:
        Type _type;
        bool left;
        bool right;
        int x;
        int y;

    public:
        Event(Type atype, const Mouse& parent) noexcept
            : _type(atype), left(parent.left_is_pressed), right(parent.right_is_pressed), x(parent.x), y(parent.y)
        {
        }

    public:
        Type GetType() const noexcept
        {
            return _type;
        }
        std::pair<int, int> GetPos() const noexcept
        {
            return { x, y };
        }
        int GetPosX() const noexcept
        {
            return x;
        }
        int GetPosY() const noexcept
        {
            return y;
        }
        bool LeftPressed() const noexcept
        {
            return left;
        }
        bool RightPressed() const noexcept
        {
            return right;
        }
    };

public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;

public:
    std::pair<int, int> GetPos() const noexcept
    {
        return { x, y };
    }
    std::optional<RawDelta> ReadRawDelta() noexcept;
    int GetPosX() const noexcept
    {
        return x;
    }
    int GetPosY() const noexcept
    {
        return y;
    }
    bool IsInWindow() const noexcept
    {
        return is_in_window;
    }
    bool LeftIsPressed() const noexcept
    {
        return left_is_pressed;
    }
    bool RightIsPressed() const noexcept
    {
        return right_is_pressed;
    }
    std::optional<Mouse::Event> Read() noexcept;
    bool IsEmpty() const noexcept
    {
        return buffer.empty();
    }
    void Flush() noexcept
    {
        buffer = std::queue<Event>();
    }
    void BoundCursor(int width, int height) noexcept;

    void EnableRaw() noexcept
    {
        raw_enabled = true;
    }
    void DisableRaw() noexcept
    {
        raw_enabled = false;
    }
    bool RawEnabled() const noexcept
    {
        return raw_enabled;
    }

public:
    void OnMouseMove(int x, int y) noexcept;
    void OnMouseLeave() noexcept;
    void OnMouseEnter() noexcept;
    void OnRawDelta(int dx, int dy) noexcept;
    void OnLeftPressed(int x, int y) noexcept;
    void OnLeftReleased(int x, int y) noexcept;
    void OnRightPressed(int x, int y) noexcept;
    void OnRightReleased(int x, int y) noexcept;
    void OnWheelUp(int x, int y) noexcept;
    void OnWheelDown(int x, int y) noexcept;
    void TrimBuffer() noexcept;
    void TrimRawInputBuffer() noexcept;
    void OnWheelDelta(int x, int y, int delta) noexcept;

private:
    static constexpr unsigned int buffer_size = 16u;

private:
    int x;
    int y;
    bool left_is_pressed = false;
    bool right_is_pressed = false;
    bool is_in_window = false;
    int wheel_delta_carry = 0;
    bool raw_enabled = false;
    std::queue<Event> buffer;
    std::queue<RawDelta> raw_delta_buffer;
};
} // namespace ex
