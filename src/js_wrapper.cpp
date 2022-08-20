#include "js_wrapper.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <functional>
#include <ChakraCore.h>
#include <imgui/imgui.h>
#include "../render/renderable.h"
#include "strconvert.h"

extern std::vector<std::string> js_logs;
extern std::vector<renderable*> js_windows;

class js_window : public renderable {
	JsValueRef* callback_function;
public:
	js_window(JsValueRef* callback_function) : renderable::renderable("js_render") {
		this->callback_function = callback_function;
	}
	void on_render() override {
		JsCallFunction(*this->callback_function, nullptr, 0, nullptr);
	}
};

namespace js_wrapper {
	namespace b2c_js {
		JsValueRef CALLBACK print(JsValueRef callee, bool is_constructed_call, JsValueRef* arguments, unsigned short argument_count, void* callback_state) {
			if (argument_count == 0 || argument_count == 1) {
				return JS_INVALID_REFERENCE;
			}
			JsValueRef string_value;
			JsConvertValueToString(arguments[1], &string_value);
			char* string = nullptr;
			size_t length;
			JsCopyString(string_value, nullptr, 0, &length);
			string = (char*)malloc(length + 1);
			JsCopyString(string_value, string, length + 1, nullptr);
			printf("%s", string);
			js_logs.push_back("[SCRIPT] "+std::string(string, length));
			free(string);
			return JS_INVALID_REFERENCE;
		}
		JsValueRef CALLBACK sleep(JsValueRef callee, bool is_constructed_call, JsValueRef* arguments, unsigned short argument_count, void* callback_state) {
			if (argument_count == 0 || argument_count == 1) {
				return JS_INVALID_REFERENCE;
			}
			JsValueRef js_sleep_millis;
			JsConvertValueToNumber(arguments[1], &js_sleep_millis);
			int sleep_millis;
			JsNumberToInt(js_sleep_millis, &sleep_millis);
			Sleep(sleep_millis);
			return JS_INVALID_REFERENCE;
		}
		JsValueRef CALLBACK on_render(JsValueRef callee, bool is_constructed_call, JsValueRef* arguments, unsigned short argument_count, void* callback_state) {
			if (argument_count == 0 || argument_count == 1) {
				return JS_INVALID_REFERENCE;
			}
			js_windows.push_back(new js_window(&arguments[1]));
			return JS_INVALID_REFERENCE;
		}
		namespace JsGui {
			JsValueRef CALLBACK Begin(JsValueRef callee, bool is_constructed_call, JsValueRef* arguments, unsigned short argument_count, void* callback_state) {
				if (argument_count == 0 || argument_count == 1) {
					return JS_INVALID_REFERENCE;
				}
				JsValueRef string_value;
				JsConvertValueToString(arguments[1], &string_value);
				char* string = nullptr;
				size_t length;
				JsCopyString(string_value, nullptr, 0, &length);
				string = (char*)malloc(length + 1);
				JsCopyString(string_value, string, length + 1, nullptr);
				ImGui::Begin(string);
				free(string);
				return JS_INVALID_REFERENCE;
			}
			JsValueRef CALLBACK End(JsValueRef callee, bool is_constructed_call, JsValueRef* arguments, unsigned short argument_count, void* callback_state) {
				ImGui::End();
				return JS_INVALID_REFERENCE;
			}
		}
	}

	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result;
	JsValueRef error;
	unsigned currentSourceContext = 0;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	std::pair<JsValueRef, JsPropertyIdRef> create_function(std::string name, JsValueRef* parent, JsNativeFunction js_func) {
		//Create Js values & refs
		JsValueRef func_ref;
		JsPropertyIdRef func_id;
		JsCreateFunction(js_func, nullptr, &func_ref);
		JsCreatePropertyId(name.c_str(), name.length(), &func_id);

		//If theres no parent, add it to the global scope.
		if (parent == nullptr) {
			JsValueRef global;
			JsGetGlobalObject(&global);
			JsSetProperty(global, func_id, func_ref, true);
		}
		else {
			JsSetProperty(parent, func_id, func_ref, true);
		}

		return std::pair<JsValueRef, JsPropertyIdRef>(func_ref, func_id);
	}
	std::pair<JsValueRef, JsPropertyIdRef> create_object(std::string name, JsValueRef* parent) {
		//Create Js values & refs
		JsValueRef obj_ref;
		JsPropertyIdRef obj_id;
		JsCreateObject(&obj_ref);
		JsCreatePropertyId(name.c_str(), name.length(), &obj_id);

		//If theres no parent, add it to the global scope.
		if (parent == nullptr) {
			JsValueRef global;
			JsGetGlobalObject(&global);
			JsSetProperty(global, obj_id, obj_ref, true);
		}
		else {
			JsSetProperty(parent, obj_id, obj_ref, true);
		}

		return std::pair<JsValueRef, JsPropertyIdRef>(obj_ref, obj_id);
	}

	void setup_runtime() {
		// Create a runtime. 
		JsErrorCode error_code = JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);
		if (error_code != JsErrorCode::JsNoError) {
			throw std::exception("JsErrorCode: "+error_code);
		}

		// Create an execution context. 
		JsCreateContext(runtime, &context);

		// Now set the current execution context.
		JsSetCurrentContext(context);

		//Setup native functions
		/*
		std::pair<JsValueRef, JsPropertyIdRef> imgui_obj = create_object("ImGui", nullptr);
		create_function("Begin", &imgui_obj.first, b2c_js::JsGui::Begin);
		create_function("End", &imgui_obj.first, b2c_js::JsGui::End);*/
		create_function("print", nullptr, b2c_js::print);
		create_function("sleep", nullptr, b2c_js::sleep);
		create_function("on_render", nullptr, b2c_js::on_render);

		//Load scripts
		/*
		try {
			std::string scriptsDir = "js/";
			for(const auto& js : std::filesystem::directory_iterator(scriptsDir)) {
				run_file(js.path().string());
			}
		} catch(std::exception ex) {
			MessageBoxA(nullptr, ex.what(), "Error (Missing 'loaders' folder?)", MB_OK);
		}
		*/
	}
	void destroy_runtime() {
		// Dispose runtime
		js_windows.clear();
		JsSetCurrentContext(JS_INVALID_REFERENCE);
		JsDisposeRuntime(runtime);
	}
	std::wstring run_file(std::string filename) {
		std::ifstream i_src(filename);
		std::string s_src(std::istreambuf_iterator<char>{i_src}, {});
		return run_script(s_src);
	}
	std::wstring run_script(std::string script) {
		return run_script(strconvert::s_to_ws(script));
	}
	std::wstring run_script(std::wstring script) {
		
		// Convert your script result to String in JavaScript; redundant if your script returns a String
		JsValueRef resultJSString;

		// Run the script.
		if(JsRunScript(script.c_str(), currentSourceContext++, L"", &result) != JsNoError) {
			JsGetAndClearException(&error);
			JsConvertValueToString(error, &resultJSString);
		} else {
			JsConvertValueToString(result, &resultJSString);
		}

		// Project script result back to C++.
		const wchar_t *resultWC;
		size_t stringLength;
		JsStringToPointer(resultJSString, &resultWC, &stringLength);

		std::wstring resultW(resultWC);
		return resultW;
	}
};
