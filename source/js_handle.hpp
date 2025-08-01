#ifndef JS_HANDLE_HPP
#define JS_HANDLE_HPP


namespace js_handle {
	inline HRESULT on_message(ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args) {
		wil::unique_cotaskmem_string raw;
		args->get_WebMessageAsJson(&raw);
		
		auto message = json::value::parse(raw.get());

		if (!message.has_field(L"action"))
			return S_OK;

		auto action = message.at(L"action").as_string();

		if (action == L"exit")
			std::exit(ERROR_SUCCESS);

		if (action == L"minimize")
			ShowWindow(g_window, SW_MINIMIZE);

		return S_OK;
	}
}


#endif // !JS_HANDLE_HPP