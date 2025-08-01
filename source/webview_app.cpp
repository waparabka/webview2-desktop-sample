#include "webview_app.hpp"

#include "resource_handle.hpp"
#include "js_handle.hpp"


bool c_webview_app::initialize() {
    static bool initialized = false;

    if (initialized)
        return true;

    wchar_t userdata[MAX_PATH];
    SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, userdata);

    auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
    options->put_AdditionalBrowserArguments(L"--enable-features=msWebView2EnableDraggableRegions --disable-web-security --disk-cache-size=1 --media-cache-size=1");

    CreateCoreWebView2EnvironmentWithOptions(nullptr, userdata, options.Get(), Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([&](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
        if (FAILED(result) || env == nullptr)
            return S_OK;

        initialized = true;

        env->CreateCoreWebView2Controller(g_window, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([&](HRESULT result, ICoreWebView2Controller* ctrl) -> HRESULT {
            webview_ctrl = ctrl;
            webview_ctrl->get_CoreWebView2(&webview);

            wil::com_ptr<ICoreWebView2Settings> settings;
            webview->get_Settings(&settings);

            settings->put_AreDevToolsEnabled(0);

            webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(js_handle::on_message).Get(), nullptr);

            webview->AddWebResourceRequestedFilter(L"*", COREWEBVIEW2_WEB_RESOURCE_CONTEXT_ALL);
            webview->add_WebResourceRequested(Callback<ICoreWebView2WebResourceRequestedEventHandler>(resource_handle::on_resource).Get(), nullptr);

            RECT bounds;
            GetClientRect(g_window, &bounds);
            
            webview_ctrl->put_Bounds(bounds);
            
            // uncomment this for transparent background
            // webview_ctrl.query<ICoreWebView2Controller2>()->put_DefaultBackgroundColor({ 0 });

            auto ef = cmrc::WebResources::get_filesystem();

            auto index = ef.open("index.html");
            auto content = std::wstring(index.begin(), index.end());

            webview->NavigateToString(content.c_str());
            
            // we can use links as well
            // webview->Navigate(L"http://localhost:8080/");

            return S_OK;

        }).Get());

        return S_OK;
    }).Get());

    return initialized;
}


void c_webview_app::run() {
    // we can download WebView2 runtime if missing
    if (!initialize()) {
        // download::to_path("https://go.microsoft.com/fwlink/p/?LinkId=2124703", "webview2_runtime.exe");
        // then run setup and restart program when setup is finished
    }
}