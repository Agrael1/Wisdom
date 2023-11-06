//#include <example/mouse.h>
//
//std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
//{
//    if (rawDeltaBuffer.empty()) {
//        return {};
//    }
//    const RawDelta d = rawDeltaBuffer.front();
//    rawDeltaBuffer.pop();
//    return d;
//}
//
//std::optional<Mouse::Event> Mouse::Read() noexcept
//{
//    if (buffer.size() > 0u) {
//        Mouse::Event e = buffer.front();
//        buffer.pop();
//        return e;
//    }
//    return {};
//}
//
//void Mouse::OnMouseMove(int newx, int newy) noexcept
//{
//    x = newx;
//    y = newy;
//
//    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
//    TrimBuffer();
//}
//
//void Mouse::OnMouseLeave() noexcept
//{
//    isInWindow = false;
//    buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
//    TrimBuffer();
//}
//
//void Mouse::OnMouseEnter() noexcept
//{
//    isInWindow = true;
//    buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
//    TrimBuffer();
//}
//
//void Mouse::OnRawDelta(int dx, int dy) noexcept
//{
//    rawDeltaBuffer.push({ dx, dy });
//    TrimBuffer();
//}
//
//void Mouse::OnLeftPressed(int x, int y) noexcept
//{
//    leftIsPressed = true;
//
//    buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
//    TrimBuffer();
//}
//
//void Mouse::OnLeftReleased(int x, int y) noexcept
//{
//    leftIsPressed = false;
//
//    buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
//    TrimBuffer();
//}
//
//void Mouse::OnRightPressed(int x, int y) noexcept
//{
//    rightIsPressed = true;
//
//    buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
//    TrimBuffer();
//}
//
//void Mouse::OnRightReleased(int x, int y) noexcept
//{
//    rightIsPressed = false;
//
//    buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
//    TrimBuffer();
//}
//
//void Mouse::OnWheelUp(int x, int y) noexcept
//{
//    buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
//    TrimBuffer();
//}
//
//void Mouse::OnWheelDown(int x, int y) noexcept
//{
//    buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
//    TrimBuffer();
//}
//
//void Mouse::TrimBuffer() noexcept
//{
//    while (buffer.size() > bufferSize) {
//        buffer.pop();
//    }
//}
//
//void Mouse::TrimRawInputBuffer() noexcept
//{
//    while (rawDeltaBuffer.size() > bufferSize) {
//        rawDeltaBuffer.pop();
//    }
//}
//
//void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
//{
//    wheelDeltaCarry += delta;
//    // generate events for every 120
//    while (wheelDeltaCarry >= 120) {
//        wheelDeltaCarry -= 120;
//        OnWheelUp(x, y);
//    }
//    while (wheelDeltaCarry <= -120) {
//        wheelDeltaCarry += 120;
//        OnWheelDown(x, y);
//    }
//}