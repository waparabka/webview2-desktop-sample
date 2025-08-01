#ifndef RESOURCE_HANDLE_HPP
#define RESOURCE_HANDLE_HPP

#include <shlwapi.h>
#include <shlobj_core.h>
#pragma comment(lib, "shlwapi.lib")

CMRC_DECLARE(WebResources);


namespace resource_handle {
	inline std::wstring get_mime_type(const std::string& path) {
		std::string extension = path.substr(path.find_last_of(".") + 1);

		if (extension == "svg") {
			return L"image/svg+xml";
		}
		else if (extension == "png") {
			return L"image/png";
		}
		else if (extension == "jpg" || extension == "jpeg") {
			return L"image/jpeg";
		}
		else if (extension == "css") {
			return L"text/css";
		}
		else if (extension == "js") {
			return L"application/javascript";
		}
		else if (extension == "html") {
			return L"text/html";
		}
		else {
			return L"application/octet-stream";
		}
	}


	inline HRESULT on_resource(ICoreWebView2* webview, ICoreWebView2WebResourceRequestedEventArgs* args) {
		wil::com_ptr<ICoreWebView2WebResourceRequest> request;
		args->get_Request(&request);

		wil::unique_cotaskmem_string uri;
		request->get_Uri(&uri);
		
		std::string source = utility::conversions::to_utf8string(uri.get());

		auto prefix = source.find("res://"); 

		if (prefix != std::string::npos) {
			auto ef = cmrc::WebResources::get_filesystem();

			source = source.substr(prefix + 6);

			if (ef.exists(source)) {
				auto file = ef.open(source);

				std::string content(file.begin(), file.end());

				wil::com_ptr<IStream> stream(SHCreateMemStream(reinterpret_cast<const BYTE*>(content.data()), content.size()));

				wil::com_ptr<ICoreWebView2_2> webview2;
				webview->QueryInterface(IID_PPV_ARGS(&webview2));

				wil::com_ptr<ICoreWebView2Environment> environment;
				webview2->get_Environment(&environment);

				std::wstring mime_type = get_mime_type(source);

				wil::com_ptr<ICoreWebView2WebResourceResponse> response;
				environment->CreateWebResourceResponse(stream.get(), 200, L"OK", mime_type.c_str(), &response);

				wil::com_ptr<ICoreWebView2HttpResponseHeaders> headers;
				response->get_Headers(&headers);

				headers->AppendHeader(L"Content-Length", std::to_wstring(content.size()).c_str());
				headers->AppendHeader(L"Access-Control-Allow-Origin", L"*");

				args->put_Response(response.get());
			}
		}
		return S_OK;
	}
}


#endif // !RESOURCE_HANDLE_HPP