#include "window.h"
#include <sstream>
#include "resource.h"
#include "util.h"

// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

enum class MenuItems : UINT_PTR {
    Load = ID_FILE_LOADMODEL,
    Exit = ID_FILE_EXIT,
    ShowGrid = ID_OPTIONS_DRAWGRID,
    Style_VGUI = ID_STYLES_VGUI,
    Style_Dark = ID_STYLES_DARK,
    Style_Cherry = ID_STYLES_CHERRY,
    PlayGame = ID_MODE_GAME,
    About = ID_HELP_ABOUT,
};

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
    : hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEXA wcWindow = { 0 };
    wcWindow.cbSize = sizeof(wcWindow);
    wcWindow.style = CS_OWNDC;
    wcWindow.lpfnWndProc = HandleMsgSetup;
    wcWindow.cbClsExtra = 0;
    wcWindow.cbWndExtra = 0;
    wcWindow.hInstance = GetInstance();
    wcWindow.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcWindow.hIcon = LoadIcon(wcWindow.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcWindow.hbrBackground = nullptr;
    wcWindow.lpszMenuName = MAKEINTRESOURCEA(IDR_MENU1);
    wcWindow.hIconSm = nullptr;
    wcWindow.lpszClassName = GetName();
    RegisterClassExA(&wcWindow);
}
Window::WindowClass::~WindowClass()
{
    UnregisterClassA(wndClassName, GetInstance());
}
const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

// Window namespace
Window::Window(unsigned int width, unsigned int height, const char* name)
    : width(width), height(height)
{
    static uint32_t monitor = 0;

    struct A {
        uint32_t monitor;
        RECT rMonitor;
        HMONITOR hMonitor;
    } mon{};

    // Enum all monitors
    EnumDisplayMonitors(nullptr, nullptr, [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
        MONITORINFOEX mi;
        mi.cbSize = sizeof(mi);
        GetMonitorInfo(hMonitor, &mi);
        std::stringstream ss;
        ss << "Monitor " << dwData << ": " << mi.szDevice;
        OutputDebugStringA(ss.str().c_str());
        OutputDebugStringA("\n");
        
        A& mon = *reinterpret_cast<A*>(dwData);
        if (mon.monitor++ == monitor) {
            mon.rMonitor = mi.rcMonitor;
            return FALSE;
        }

        return TRUE; }, LPARAM(&mon));

    monitor++;

    RECT rWindow;
    rWindow.left = 100;
    rWindow.right = width + rWindow.left;
    rWindow.top = 100;
    rWindow.bottom = height + rWindow.top;
    // Automatic calculation of window height and width to client region
    wis::check_windows(AdjustWindowRect(&rWindow, WS_OVERLAPPEDWINDOW, TRUE));

    hWnd.reset(CreateWindowExA(0, WindowClass::GetName(), name,
                               WS_OVERLAPPEDWINDOW,
                               mon.rMonitor.left + 1, mon.rMonitor.top + 1,
                               rWindow.right - rWindow.left,
                               rWindow.bottom - rWindow.top,
                               nullptr, nullptr,
                               WindowClass::GetInstance(), this));

    // Error checks
    wis::check_windows(!!hWnd);
    ShowWindow(hWnd.get(), SW_SHOWDEFAULT);

    Accelerator.reset(LoadAccelerators(WindowClass::GetInstance(), MAKEINTRESOURCE(IDR_ACCELERATOR1)));

    // Init GUI (only one window supported)
    // wis::check_windows(ImGui_ImplWin32_Init(hWnd.get()));

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01; // mouse page
    rid.usUsage = 0x02; // mouse usage
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;
    wis::check_windows(RegisterRawInputDevices(&rid, 1, sizeof(rid)));
}
Window::~Window()
{
    // ImGui_ImplWin32_Shutdown();
}

void Window::ChangeToFullScreen()
{
    SetMenu(hWnd.get(), nullptr);
    auto st = GetWindowLong(hWnd.get(), GWL_STYLE);
    auto stex = GetWindowLong(hWnd.get(), GWL_EXSTYLE);
    SetWindowLong(hWnd.get(), GWL_STYLE,
                  st & ~(WS_CAPTION | WS_THICKFRAME));
    SetWindowLong(hWnd.get(), GWL_EXSTYLE,
                  stex & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

    MONITORINFO monitor_info;
    monitor_info.cbSize = sizeof(monitor_info);
    GetMonitorInfo(MonitorFromWindow(hWnd.get(), MONITOR_DEFAULTTONEAREST),
                   &monitor_info);

    RECT window_rect(monitor_info.rcMonitor);
    SetWindowPos(hWnd.get(), NULL, window_rect.left, window_rect.top,
                 window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
}

void Window::SetTitle(std::string_view title)
{
    wis::check_windows(SetWindowTextA(hWnd.get(), title.data()));
}

void Window::EnableLoading()
{
    menu.EnableLoading();
}

void Window::EnableCursor() noexcept
{
    cursorEnabled = true;
    ShowCursor();
    EnableImGuiMouse();
    FreeCursor();
}
void Window::DisableCursor() noexcept
{
    cursorEnabled = false;
    HideCursor();
    DisableImGuiMouse();
    ConfineCursor();
}
bool Window::CursorEnabled() const noexcept
{
    return cursorEnabled;
}

void Window::ConfineCursor() noexcept
{
    RECT rect;
    GetClientRect(hWnd.get(), &rect);
    MapWindowPoints(hWnd.get(), nullptr, reinterpret_cast<POINT*>(&rect), 2);
    ClipCursor(&rect);
}
void Window::FreeCursor() noexcept
{
    ClipCursor(nullptr);
}
void Window::HideCursor() noexcept
{
    while (::ShowCursor(FALSE) >= 0)
        ;
    cursorShown = false;
}
void Window::ShowCursor() noexcept
{
    while (::ShowCursor(TRUE) < 0)
        ;
    cursorShown = true;
}
void Window::ShowImGuiMouse() noexcept
{
    if (!cursorShown)
        ShowCursor();
}
void Window::EnableImGuiMouse() noexcept
{
    // ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}
void Window::DisableImGuiMouse() noexcept
{
    // ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

std::optional<WPARAM> Window::ProcessMessages() const noexcept
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (!TranslateAccelerator(
                    hWnd.get(), // handle to receiving window
                    Accelerator.get(), // handle to active accelerator table
                    &msg)) // message data
        {
            if (msg.message == WM_QUIT) {
                return msg.wParam;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    if (!bActive) {
        WaitMessage();
    }
    return {};
}
LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Create routine initializer
    if (msg == WM_NCCREATE) {
        // Extract data from creation of window
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to win class
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set msgproc to to non setup handle
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // retrieve ptr to win class
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward msg to class handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    //{
    // return true;
    //}
    // const auto& imio = ImGui::GetIO();

    switch (msg) {
    // we don't want the DefProc to handle this message because
    // we want our destructor to destroy the window, so return 0 instead of break
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    // clear keystate when window loses focus to prevent input getting "stuck"
    case WM_KILLFOCUS:
        kbd.ClearState();
        break;
    case WM_SYSCOMMAND:
        if (wParam == SC_MINIMIZE) {
            bActive = false;
        }
        if (wParam == SC_RESTORE) {
            bActive = true;
        }
        break;
    case WM_SIZE:
        if (!LOWORD(lParam) || !HIWORD(lParam))
            break;
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        events.push(Event::Resize);
        break;
    case WM_CREATE:
        menu.Initialize(GetMenu(hWnd));
        break;
    case WM_COMMAND:
        switch (MenuItems(LOWORD(wParam))) {
        case MenuItems::Load:
            menu.DisableLoading();
            events.push(Event::LoadAsset);
            break;
        case MenuItems::Exit:
            PostQuitMessage(0);
            return 0;
        case MenuItems::ShowGrid:
            menu.ToggleGrid();
            break;
        // case MenuItems::Style_VGUI:
        //	if (menu.SetStyle(UT::Menu::Style::VGUI))
        //		events.push(Event::Restyle);
        //	break;
        // case MenuItems::Style_Dark:
        //	if (menu.SetStyle(UT::Menu::Style::Dark))
        //		events.push(Event::Restyle);
        //	break;
        // case MenuItems::Style_Cherry:
        //	if (menu.SetStyle(UT::Menu::Style::Cherry))
        //		events.push(Event::Restyle);
        //	break;
        case MenuItems::PlayGame:
            events.push(Event::Play);
            break;
        case MenuItems::About:
            DialogBox(WindowClass::GetInstance(), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, About);
            break;
        default:
            break;
        }
        break;
    case WM_ACTIVATE:
        // confine/free cursor on window to foreground/background if cursor disabled
        if (!cursorEnabled) {
            if (wParam & WA_ACTIVE) {
                ConfineCursor();
                HideCursor();
            } else {
                FreeCursor();
                ShowCursor();
            }
        }
        break;

    /*********** KEYBOARD MESSAGES ***********/
    case WM_KEYDOWN:
    // syskey commands need to be handled to track ALT key (VK_MENU) and F10
    case WM_SYSKEYDOWN:
        // stifle this keyboard message if imgui wants to capture
        // if (imio.WantCaptureKeyboard)
        //{
        //	break;
        //}
        if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
        {
            kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        // stifle this keyboard message if imgui wants to capture
        // if (imio.WantCaptureKeyboard)
        //{
        //	break;
        //}
        kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        // stifle this keyboard message if imgui wants to capture
        // if (imio.WantCaptureKeyboard)
        //{
        //	break;
        //}
        kbd.OnChar(static_cast<unsigned char>(wParam));
        break;
    /*********** END KEYBOARD MESSAGES ***********/

    /************* MOUSE MESSAGES ****************/
    case WM_MOUSEMOVE: {
        const POINTS pt = MAKEPOINTS(lParam);
        // cursorless exclusive gets first dibs
        if (!cursorEnabled) {
            if (!mouse.IsInWindow()) {
                SetCapture(hWnd);
                mouse.OnMouseEnter();
                HideCursor();
            }
            break;
        }
        // stifle this mouse message if imgui wants to capture
        // if (imio.WantCaptureMouse)
        //{
        //	ShowImGuiMouse();
        //	break;
        //}
        if (cursorShown && !cursorActive)
            HideCursor();

        // in client region -> log move, and log enter + capture mouse (if not previously in window)
        if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
            mouse.OnMouseMove(pt.x, pt.y);
            if (!mouse.IsInWindow()) {
                SetCapture(hWnd);
                mouse.OnMouseEnter();
            }
        }
        // not in client -> log move / maintain capture if button down
        else {
            if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
                mouse.OnMouseMove(pt.x, pt.y);
            }
            // button up -> release capture / log event for leaving
            else {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN: {
        SetForegroundWindow(hWnd);
        if (!cursorEnabled) {
            ConfineCursor();
            HideCursor();
        }
        // stifle this mouse message if imgui wants to capture
        // if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN: {
        // stifle this mouse message if imgui wants to capture
        // if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP: {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(pt.x, pt.y);
        // release mouse if outside of window
        if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height) {
            ReleaseCapture();
            mouse.OnMouseLeave();
        }
        break;
    }
    case WM_RBUTTONUP: {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightReleased(pt.x, pt.y);
        // release mouse if outside of window
        if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height) {
            ReleaseCapture();
            mouse.OnMouseLeave();
        }
        break;
    }
    case WM_MOUSEWHEEL: {
        // stifle this mouse message if imgui wants to capture
        // if (imio.WantCaptureMouse)
        //{
        //	break;
        //}
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(pt.x, pt.y, delta);
        break;
    }
    /************** END MOUSE MESSAGES **************/

    /************** RAW MOUSE MESSAGES **************/
    case WM_INPUT: {
        if (!mouse.RawEnabled()) {
            break;
        }
        UINT size = 0;
        // first get the size of the input data
        if (GetRawInputData(
                    reinterpret_cast<HRAWINPUT>(lParam),
                    RID_INPUT,
                    nullptr,
                    &size,
                    sizeof(RAWINPUTHEADER)) == -1) {
            // bail msg processing if error
            break;
        }
        rawBuffer.resize(size);
        // read in the input data
        if (GetRawInputData(
                    reinterpret_cast<HRAWINPUT>(lParam),
                    RID_INPUT,
                    rawBuffer.data(),
                    &size,
                    sizeof(RAWINPUTHEADER)) != size) {
            // bail msg processing if error
            break;
        }
        // process the raw input data
        auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
        if (ri.header.dwType == RIM_TYPEMOUSE &&
            (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0)) {
            mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
        }
        break;
    }
        /************** END RAW MOUSE MESSAGES **************/
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
