#include <windows.h>
#include <cstdint>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "webview_app.hpp"

constexpr uint16_t WINDOW_WIDTH = 1200;
constexpr uint16_t WINDOW_HEIGHT = 720;

c_webview_app* webview_app;


void custom_terminate() { // executes when we got uncoughted error
    std::set_terminate(nullptr);

    if (std::exception_ptr ep = std::current_exception()) {
        try {
            std::rethrow_exception(ep);
        }
        catch (std::exception const& e) {
            spdlog::basic_logger_mt("crash", "crash.txt")->error("{}", e.what());
        }
    }
    std::exit(EXIT_FAILURE);
}


LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp) {
    switch (umsg) {
    case WM_SIZE:
        if (webview_app && webview_app->webview_ctrl) {
            RECT bounds;
            GetClientRect(hwnd, &bounds);

            webview_app->webview_ctrl->put_Bounds(bounds);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_WEBVIEW_REQUEST: { // executing javascript code from another thread
        auto request_ptr = reinterpret_cast<std::shared_ptr<std::wstring>*>(lp);
        if (request_ptr) {
            std::wstring request = **request_ptr;
            request_ptr->reset();

            if (webview_app && webview_app->webview_ctrl) {
                wil::com_ptr<ICoreWebView2> webview;
                webview_app->webview_ctrl->get_CoreWebView2(&webview);

                if (webview)
                    webview->ExecuteScript(request.c_str(), nullptr);
            }
        }
        break;
    }
    default:
        return DefWindowProc(hwnd, umsg, wp, lp);
    }
    return 0;
}


WNDCLASSEX register_window_class(HINSTANCE instance) {
    WNDCLASSEX window_class_ex = { sizeof(WNDCLASSEX) };
    
    window_class_ex.style = CS_HREDRAW | CS_VREDRAW;
    window_class_ex.lpfnWndProc = window_proc;
    window_class_ex.hInstance = instance;
    window_class_ex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    window_class_ex.lpszClassName = "w2ds_window_class";

    RegisterClassEx(&window_class_ex);

    return window_class_ex;
}


int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previous_instance, LPWSTR command_line, int show) {
    std::set_terminate(custom_terminate);

    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale(""));
    
    register_window_class(instance);

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    
    int window_x = (screen_width - WINDOW_WIDTH) / 2;
    int window_y = (screen_height - WINDOW_HEIGHT) / 2;
    
    g_window = CreateWindowW(L"w2ds_window_class", L"w2ds", WS_POPUP, window_x, window_y, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, instance, nullptr);

    ShowWindow(g_window, show);
    UpdateWindow(g_window);

    webview_app = new c_webview_app();
    webview_app->run();

    MSG message;

    while (GetMessage(&message, nullptr, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return (int)message.wParam;
}