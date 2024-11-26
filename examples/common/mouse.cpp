#include "mouse.h"
#include <algorithm>

std::optional<ex::Mouse::RawDelta> ex::Mouse::ReadRawDelta() noexcept
{
    if (raw_delta_buffer.empty()) {
        return {};
    }
    const RawDelta d = raw_delta_buffer.front();
    raw_delta_buffer.pop();
    return d;
}

std::optional<ex::Mouse::Event> ex::Mouse::Read() noexcept
{
    if (buffer.size() > 0u) {
        ex::Mouse::Event e = buffer.front();
        buffer.pop();
        return e;
    }
    return {};
}

void ex::Mouse::BoundCursor(int width, int height) noexcept
{
    x = std::clamp(x, 0, width - 1);
    y = std::clamp(y, 0, height - 1);
}

void ex::Mouse::OnMouseMove(int newx, int newy) noexcept
{
    x = newx;
    y = newy;

    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void ex::Mouse::OnMouseLeave() noexcept
{
    is_in_window = false;
    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void ex::Mouse::OnMouseEnter() noexcept
{
    is_in_window = true;
    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void ex::Mouse::OnRawDelta(int dx, int dy) noexcept
{
    raw_delta_buffer.push({ dx, dy });
    TrimBuffer();
}

void ex::Mouse::OnLeftPressed(int x, int y) noexcept
{
    left_is_pressed = true;

    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void ex::Mouse::OnLeftReleased(int x, int y) noexcept
{
    left_is_pressed = false;

    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::LRelease, *this));
    TrimBuffer();
}

void ex::Mouse::OnRightPressed(int x, int y) noexcept
{
    right_is_pressed = true;

    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void ex::Mouse::OnRightReleased(int x, int y) noexcept
{
    right_is_pressed = false;

    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

void ex::Mouse::OnWheelUp(int x, int y) noexcept
{
    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void ex::Mouse::OnWheelDown(int x, int y) noexcept
{
    buffer.push(ex::Mouse::Event(ex::Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void ex::Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > buffer_size) {
        buffer.pop();
    }
}

void ex::Mouse::TrimRawInputBuffer() noexcept
{
    while (raw_delta_buffer.size() > buffer_size) {
        raw_delta_buffer.pop();
    }
}

void ex::Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
    wheel_delta_carry += delta;
    // generate Events for every 120
    while (wheel_delta_carry >= 120) {
        wheel_delta_carry -= 120;
        OnWheelUp(x, y);
    }
    while (wheel_delta_carry <= -120) {
        wheel_delta_carry += 120;
        OnWheelDown(x, y);
    }
}
