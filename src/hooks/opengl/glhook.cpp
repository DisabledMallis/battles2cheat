#include "glhook.h"

#include <Windows.h>
#include <GL/gl3w.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_win32.h>
#include <logger.h>
#include <ShlObj_core.h>
#include "../../render/render_mgr.h"
#include <memory.h>

uintptr_t oglSwapBuffers;

std::map<int, HGLRC> contexts;
bool initialized_imgui = false;
HWND hGameWindow;
GLuint shaderProg;
GLuint  txr_img, txr_scr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

PLH::x64Detour* plhWndProc = nullptr;
uint64_t oWndProc = 0;
extern bool toggled;
LRESULT APIENTRY hkWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!oWndProc) {
        return 0;
    }
    ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
    if (ImGui::GetIO().WantCaptureMouse) {
        return true;
    }
    return PLH::FnCast(oWndProc, hkWndProc)(hwnd, uMsg, wParam, lParam);
}

auto oglHook(HDC hdc, int b) -> bool {
	if(!initialized_imgui) {
        if (gl3wInit() == GL3W_OK) {
            ilog("Gl3w successfully initialized");
        }
        else {
            ilog("Gl3w failed to initialize! Expect a crash!!!");
        };
		initialized_imgui = true;

        // Get the game's window from it's handle
        hGameWindow = WindowFromDC(hdc);
        uint64_t pWinProc = memory::FindSig("48 8B ?? 55 53 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? ?? 29 ?? ?? ?? 29 ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? 49");
        plhWndProc = new PLH::x64Detour(pWinProc, (uint64_t)hkWndProc, &oWndProc);
        plhWndProc->hook();

        // Init glew, create imgui context, init imgui
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hGameWindow);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();
        ImGui::GetStyle().AntiAliasedFill = false;
        ImGui::GetStyle().AntiAliasedLines = false;
        ImGui::CaptureMouseFromApp();
        ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        char fontsPath[MAX_PATH];
        SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, NULL, fontsPath);
        std::string arial_path = std::string(std::string(fontsPath) + "/Calibril.ttf");
        io.Fonts->AddFontFromFileTTF(arial_path.c_str(), 20);
        io.Fonts->AddFontFromFileTTF(arial_path.c_str(), 72);
        io.Fonts->Build();

        ImGui::StyleColorsCustom();
	}

    ImGuiIO& io = ImGui::GetIO();

    auto checkErrors = [](int ln) {
        GLint glge = glGetError();
        if (glge) {
            ilog("%d-GLGE: %d", ln, glge);
        }
    };

    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

    render_mgr::dispatchRender();
	
	ImGui::Render();

	// Draw the overlay
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//wglMakeCurrent(oldhdc, oldctx);
    return PLH::FnCast(oglSwapBuffers, oglHook)(hdc, b);
}

// this hook works across all versions cuz its hooking the native opengl dll, which is in all versions of the game
auto glhook::apply() -> bool {
    if (!this) {
        ilog(LogLevel::Error, "Fatal: GLHook has no 'this' pointer?");
    }

    //First try hooking Opengl's swapbuffers
    {
        HMODULE oGlHandle = GetModuleHandleA("OPENGL32.dll");
        uintptr_t fp = (uintptr_t)GetProcAddress(oGlHandle, "wglSwapBuffers");

        if (fp == 0) {
            ilog(LogLevel::Error, "wglSwapBuffers couldn't be found");
        }
        else {
            this->create_detour(fp, (uint64_t)&oglHook, (uint64_t*)&oglSwapBuffers);
            if (!this->get_detour()) {
                ilog(LogLevel::Error, "Fatal: GLHook detour wasn't created?");
            }
            if (this->get_detour()->hook()) {
                return true;
            }
        }
    }

    //If it fails, try gdi's
    {
        HMODULE oGlHandle = GetModuleHandleA("gdi32full.dll");
        uintptr_t fp = (uintptr_t)GetProcAddress(oGlHandle, "SwapBuffers");

        if (fp == 0) {
            ilog(LogLevel::Error, "gdi32 SwapBuffers couldn't be found");
        }
        else {
            this->create_detour(fp, (uint64_t)&oglHook, (uint64_t*)&oglSwapBuffers);
            if (!this->get_detour()) {
                ilog(LogLevel::Error, "Fatal: GLHook detour wasn't created?");
            }
            if (this->get_detour()->hook()) {
                return true;
            }
        }
    }
    ilog(LogLevel::Error, "Failed to hook renderer");
	return false;
}

auto glhook::remove() -> bool {
	SetWindowLongPtr(hGameWindow, GWLP_WNDPROC, (LONG_PTR)oWndProc);
	return this->get_detour()->unHook();
}