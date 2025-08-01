#ifndef WEBVIEW_APP_HPP
#define WEBVIEW_APP_HPP


#define WM_WEBVIEW_REQUEST WM_USER + 0x01

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <wil/com.h>
#include <wrl.h>

#include <fmt/xchar.h>
#include <fmt/core.h>

#include <cpprest/json.h>
#include <cmrc/cmrc.hpp>


using namespace Microsoft::WRL;
using namespace web;

inline HWND g_window = NULL;


class c_webview_app {
private:
	wil::com_ptr<ICoreWebView2> webview;

	bool initialize();

public:
	c_webview_app() = default;
	~c_webview_app() = default;

	wil::com_ptr<ICoreWebView2Controller> webview_ctrl;

	void run();
};


#endif // !WEBVIEW_APP_HPP